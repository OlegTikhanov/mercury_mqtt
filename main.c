#include <stdio.h>
#include "mercury.h"
#include <unistd.h>
#include "settings.h"
#include "mqtt.h"
#include <pthread.h>


void sendConfig(void)
{
    switch (COUNTERS_NUM)
    {
    case 4:
        mqtt_send(CONFIG_CNT4, CNT4_CONFIG_TOPIC);
    case 3:
        mqtt_send(CONFIG_CNT3, CNT3_CONFIG_TOPIC);
    case 2:
        mqtt_send(CONFIG_CNT2, CNT2_CONFIG_TOPIC);
    case 1:
        mqtt_send(CONFIG_CNT1, CNT1_CONFIG_TOPIC);
    }

    mqtt_send(CONFIG_POWER, POWER_CONFIG_TOPIC);
}

int sendCounters(void)
{
    mercury_counters counter;
    if (getCounters(MERCURY_ADDRESS_UINT, &counter) != 0)
        return -1;
    char cnt1[12], cnt2[12], cnt3[12], cnt4[12];

    sprintf(cnt1, "%.2f", counter.cnt1 / 1000.);
    sprintf(cnt2, "%.2f", counter.cnt2 / 1000.);
    sprintf(cnt3, "%.2f", counter.cnt3 / 1000.);
    sprintf(cnt4, "%.2f", counter.cnt4 / 1000.);

    switch (COUNTERS_NUM)
    {
    case 4:
        mqtt_send(cnt4, CNT4_TOPYC);
    case 3:
        mqtt_send(cnt3, CNT3_TOPYC);
    case 2:
        mqtt_send(cnt2, CNT2_TOPYC);
    case 1:
        mqtt_send(cnt1, CNT1_TOPYC);
    }
    return 0;
}

int sendPower(void)
{
    int power = getPower(MERCURY_ADDRESS_UINT);
    if (power < 0)
        return -1;
    char power_str[12];
    sprintf(power_str, "%.2f", power / 1000.);
    mqtt_send(power_str, POWER_TOPYC);
    return 0;
}

static void *countersSendler(void *data)
{
    while (1)
    {
        if (sendCounters() == 0)
            sleep(COUNTERS_SEND_PERIOD);
    }
}

int main(void)
{
    mqtt_init();
    sendConfig();
    static pthread_t  pthr_cnt_sendler;
    pthread_create(&pthr_cnt_sendler, NULL, countersSendler, NULL);
    pthread_detach(pthr_cnt_sendler);
    while (1)
    {
        sleep(POWER_SEND_PERIOD);
        sendPower();
    }
}