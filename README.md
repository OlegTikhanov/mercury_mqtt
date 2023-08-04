# Меркурий 200, 201, 203 для Home Assistant
Утилита для считывания показаний со счётчиков меркурий 200, 201, 203 и отправки mqtt брокеру для Home Assistant. Может быть установлена как демон.\
Передаёт счётчики тарифов и текущую потребляемую мощность.\
Для работы необходим rs485 преобразователь.
## Установка зависимостей.
    $ sudo apt update
    $ sudo apt install mosquitto-dev libmosquitto-dev gcc make

## Настройка параметров.
### Исправить порт преобразователя на свой.
    #define MERCURY_PORT "/dev/ttyUSB0"
### Адрес счётчика.
    #define MERCURY_ADDRESS "000000"
    #define MERCURY_ADDRESS_UINT 000000
### Количество тарифов, частота отправки тарифов и мощности.
    #define COUNTERS_NUM 2 // 1-4
    #define COUNTERS_SEND_PERIOD 5*60 // s
    #define POWER_SEND_PERIOD 5 // s
### Учётные данные для mqtt.
    #define MQTT_HOST "127.0.0.1"
    #define MQTT_USER "user"
    #define MQTT_PASSWORD "password"
## Компиляция и установка сервиса.
    $ make
    $ make install
### Удаление сервиса.
    $ make uninstall 

