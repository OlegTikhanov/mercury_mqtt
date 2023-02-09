#ifndef SETTINGS_H
#define SETTINGS_H

#define MERCURY_PORT "/dev/ttyUSB0"
#define MERCURY_ADDRESS "000000"
#define MERCURY_ADDRESS_UINT 000000

#define COUNTERS_NUM 2 // 1-4
#define COUNTERS_SEND_PERIOD 5*60 // s
#define POWER_SEND_PERIOD 5 // s

#define MQTT_HOST "127.0.0.1"
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"


#define CONFIG_TOPIC "homeassistant/sensor/mercury/"
#define CNT1_CONFIG_TOPIC CONFIG_TOPIC "counter1_" MERCURY_ADDRESS "/config"
#define CNT2_CONFIG_TOPIC CONFIG_TOPIC "counter2_" MERCURY_ADDRESS "/config"
#define CNT3_CONFIG_TOPIC CONFIG_TOPIC "counter3_" MERCURY_ADDRESS "/config"
#define CNT4_CONFIG_TOPIC CONFIG_TOPIC "counter4_" MERCURY_ADDRESS "/config"
#define POWER_CONFIG_TOPIC CONFIG_TOPIC "power_" MERCURY_ADDRESS "/config"

#define CNT1_TOPYC "mercury_" MERCURY_ADDRESS "/counter1/state"
#define CNT2_TOPYC "mercury_" MERCURY_ADDRESS "/counter2/state"
#define CNT3_TOPYC "mercury_" MERCURY_ADDRESS "/counter3/state"
#define CNT4_TOPYC "mercury_" MERCURY_ADDRESS "/counter4/state"
#define POWER_TOPYC "mercury_" MERCURY_ADDRESS "/power/state"

#define CONFIG_DEVICE "\
\"device\":{\
    \"identifiers\":\"Электросчетчик «МЕРКУРИЙ»\",\
    \"name\":\"Электрический счётчик Меркурий 200\",\
    \"model\":\"Mercury 200,201,203\",\
    \"manufacturer\":\"Инкотекс\"\
}"

#define CONFIG_TEMPLATE "\
\"unit_of_measurement\":\"kWh\",\
\"device_class\":\"energy\",\
\"state_class\":\"total\",\
" CONFIG_DEVICE

#define CONFIG_CNT1 "\
{\
	\"name\":\"Тариф-1\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter1\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter1\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter1/state\",\
    " CONFIG_TEMPLATE "\
}"
#define CONFIG_CNT2 "\
{\
	\"name\":\"Тариф-2\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter2\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter2\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter2/state\",\
    " CONFIG_TEMPLATE "\
}"
#define CONFIG_CNT3 "\
{\
	\"name\":\"Тариф-3\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter3\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter3\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter3/state\",\
    " CONFIG_TEMPLATE "\
}"
#define CONFIG_CNT4 "\
{\
	\"name\":\"Тариф-4\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter4\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter4\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter4/state\",\
    " CONFIG_TEMPLATE "\
}"

#define CONFIG_POWER "\
{\
	\"name\":\"Текущая мощность\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_power\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_power\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/power/state\",\
    \"unit_of_measurement\":\"kWh\",\
    \"device_class\":\"energy\",\
    \"state_class\":\"measurement\",\
    " CONFIG_DEVICE "\
}"

#endif