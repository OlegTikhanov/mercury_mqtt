#ifndef SETTINGS_H
#define SETTINGS_H

#define MERCURY_PORT "/dev/ttyUSB0"
#define MERCURY_ADDRESS 000000
#define COUNTERS_NUM 2 // 1-4

#define MQTT_HOST "127.0.0.1"
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

#define CONFIG_TOPIC "homeassistant/sensor/mercury/"
#define CNT1_CONFIG_TOPIC CONFIG_TOPIC "counter1_" MERCURY_ADDRESS "/config"
#define CNT2_CONFIG_TOPIC CONFIG_TOPIC "counter2_" MERCURY_ADDRESS "/config"
#define CNT3_CONFIG_TOPIC CONFIG_TOPIC "counter3_" MERCURY_ADDRESS "/config"
#define CNT4_CONFIG_TOPIC CONFIG_TOPIC "counter4_" MERCURY_ADDRESS "/config"

#define CONFIG_TEMPLATE "\
\"unit_of_measurement\":\"kWh\",\
\"device_class\":\"energy\",\
\"state_class\":\"total\",\
\"device\":{\
    \"identifiers\":\"mercury_counter\",\
    \"name\":\"Электрический счётчик Меркурий 200\",\
    \"model\":\"Mercury_200\"\
}\
"

#define CONFIG_CNT1 "\
{\
	\"name\":\"counter1\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter1\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter1\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter1/state\",\
    " CONFIG_TEMPLATE "\
}"
#define CONFIG_CNT2 "\
{\
	\"name\":\"counter2\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter2\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter2\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter2/state\",\
    " CONFIG_TEMPLATE "\
}"
#define CONFIG_CNT3 "\
{\
	\"name\":\"counter3\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter3\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter3\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter3/state\",\
    " CONFIG_TEMPLATE "\
}"
#define CONFIG_CNT4 "\
{\
	\"name\":\"counter3\",\
    \"unique_id\":\"mercury_" MERCURY_ADDRESS "_counter3\",\
	\"object_id\":\"mercury_" MERCURY_ADDRESS "_counter3\",\
    \"state_topic\":\"mercury_" MERCURY_ADDRESS "/counter1/state\",\
    " CONFIG_TEMPLATE "\
}"

#endif