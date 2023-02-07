#include "mercury.h"
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int fd;
static int openPort(char *port)
{
    fd = open(port, O_RDWR | O_NOCTTY); // O_RDWR - Read/Write , O_NOCTTY - No terminal will control the process

    if (fd < 0)
        return -1;

    fcntl(fd, F_SETFL, FNDELAY); // не блокирующий режим

    struct termios SerialPortSettings;

    tcgetattr(fd, &SerialPortSettings); // Получение текущих настроек порта
    // Установка скорости передачи
    cfsetispeed(&SerialPortSettings, B9600); // Приём 9600 бод
    cfsetospeed(&SerialPortSettings, B9600); // Передача 9600 бод

    SerialPortSettings.c_cflag &= ~PARENB; // Без контроля чётности
    SerialPortSettings.c_cflag &= ~CSTOPB; // 1 стоп бит
    SerialPortSettings.c_cflag &= ~CSIZE;  // Очистка битов скорости
    SerialPortSettings.c_cflag |= CS8;     // 8 бит

    SerialPortSettings.c_cflag &= ~CRTSCTS;         // Без аппаратного управления потоком
    SerialPortSettings.c_cflag |= CREAD | CLOCAL;   // Включение приёмника, игнорировать управление линиями
    SerialPortSettings.c_iflag &= ~(IXOFF | IXANY); // Отключение управления потоком данных XON/XOFF
    SerialPortSettings.c_oflag &= ~OPOST;           // Не обрабатывать вывод
    cfmakeraw(&SerialPortSettings);                 // Raw mode

    SerialPortSettings.c_cc[VMIN] = 24;
    SerialPortSettings.c_cc[VTIME] = 1; // Межбайтовый таймаут 100ms

    // Установка параметров
    if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0)
        return -1;
    return 0;
}

uint16_t crc16(uint8_t *data, uint16_t size)
{
    uint16_t crc = 0xFFFF;

    for (uint16_t pos = 0; pos < size; pos++)
    {
        crc ^= (uint16_t)data[pos];
        for (int i = 8; i != 0; i--)
        { // Loop over each bit
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
                crc >>= 1;
        }
    }
    return (crc >> 8) + ((crc << 8) & (0xff00));
}

void writeCommand(uint32_t address, uint8_t cmd)
{
    uint8_t write_buffer[7];
    int bytes_write;
    for (uint8_t i = 0; i < 4; i++)
    { // запись адреса
        write_buffer[i] = (uint8_t)(address >> 8 * (3 - i));
    }
    write_buffer[4] = cmd;                 // запись команды
    uint16_t crc = crc16(write_buffer, 5); // расчёт и запись контрольной суммы
    write_buffer[5] = (uint8_t)(crc >> 8);
    write_buffer[6] = (uint8_t)crc;

    bytes_write = write(fd, &write_buffer, sizeof(write_buffer) / sizeof(uint8_t)); // Запись в порт
#if DEBUG > 1
    printf("Отправлено: %d байт\n", bytes_write);
    for (int i = 0; i < 7; i++)
        printf("%02X ", write_buffer[i]);
    printf("\n");
#endif
}

uint32_t convertToInt(uint8_t *data, uint8_t length)
{
    char temp_arr[length * 2 + 1];
    uint8_t index = 0;
    uint32_t result;
    for (uint8_t i = 0; i < length; i++)
    {
        index += sprintf(temp_arr + index, "%02X", data[i]);
    }
    sscanf(temp_arr, "%u", &result);
    return result;
}

static int readData(uint32_t dev_addr, uint8_t *buffer)
{
    uint32_t timeout = READ_TIMEOUT * (CLOCKS_PER_SEC / 1000);
    uint8_t read_buffer[24];
    int bytes_read = 0;

    tcflush(fd, TCIFLUSH);
    clock_t ticks = clock();
    while ((bytes_read < 1) && (clock() - ticks < timeout))
    {
        bytes_read = read(fd, &read_buffer, 24);
    }
    if (bytes_read < 1)
    {
#if DEBUG > 0
        printf("Таймаут чтения данных.\n");
#endif
        return -1;
    }
#if DEBUG > 1
    printf("Получено: %d байт.\n", bytes_read); // сырые данные
    for (int i = 0; i < bytes_read; i++)
        printf("%02X ", read_buffer[i]);
    printf("\n");
#endif
    //------------------Проверка контрольной суммы------------------
    uint16_t crc_calc, crc_read;
    crc_calc = crc16(read_buffer, bytes_read - 2);
    crc_read = ((uint16_t)read_buffer[bytes_read - 2] << 8) + read_buffer[bytes_read - 1];
    if (crc_calc != crc_read)
    {
#if DEBUG > 0
        printf("Неверная контрольная сумма:%x != %x\n", crc_read, crc_calc);
#endif
        close(fd);
        return -1;
    }
#if DEBUG > 1
    else
        printf("Контрольная сумма верна:%x == %x\n", crc_read, crc_calc);
#endif
    //------------------Проверка адреса------------------
    uint32_t address = 0;
    for (uint8_t i = 0; i < 4; i++)
    { // преобразование адреса
        address += (uint32_t)read_buffer[i] << 8 * (3 - i);
    }
    if (address != dev_addr)
    {
#if DEBUG > 1
        printf("Неверный адрес в ответе:%u != %u\n", dev_addr, address);
#endif
        return -1;
    }
    else
#if DEBUG > 1
        printf("Адрес в ответе:%u\n", address);
#endif
    //------------------Получение данных------------------

    memcpy(buffer, read_buffer + 5, bytes_read - 2 - 5);
    return 0;
}

static int get(uint32_t address, uint8_t command, uint8_t *buffer)
{
    if (openPort("/dev/ttyUSB0") < 0)
    {
#if DEBUG > 0
        printf("Ошибка открытия порта.\n");
#endif
        uint32_t timeout = OPEN_PORT_TIMEOUT * (CLOCKS_PER_SEC / 1000);
        clock_t ticks = clock();
        int open;
        while (((open = openPort("/dev/ttyUSB0")) < 0) && (clock() - ticks < timeout))
            ;
        if (open < 0)
            return -1;
    }
    writeCommand(address, command);
    if (readData(address, buffer) < 0)
    {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int getCounters(uint32_t address, mercury_counters *counters)
{
    uint8_t data[4 * 4];
    if (get(address, 0x27, data) < 0)
        return -1;
    counters->cnt1 = convertToInt(data, 4) * 10;
    counters->cnt2 = convertToInt(data + 4, 4) * 10;
    counters->cnt3 = convertToInt(data + 4 * 2, 4) * 10;
    counters->cnt4 = convertToInt(data + 4 * 3, 4) * 10;
    return 0;
}

int getPower(uint32_t address)
{
    uint8_t data[4];
    if (get(address, 0x26, data) < 0)
        return -1;
    return convertToInt(data, 1) * 1000 + convertToInt(data + 1, 1) * 10;
}
