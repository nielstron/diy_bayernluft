/*
 * Configuration and settings for the combiled code
 * that may change from flash to flash
 */

const String WIFI_SSID = "SSID";
const String WIFI_PASS = "PASSWORD";

const String MQTT_HOST = "HOST_ADDRESS";
const int MQTT_PORT = 1883; 
const String MQTT_PUBLISH_BASE_TOPIC = "/pub";
const String MQTT_CONTROL_BASE_TOPIC = "/control";
const String MQTT_CLIENT = "CLIENT_ID";
const String MQTT_USER = "USERNAME";
const String MQTT_PASS = "PASSWORD";

// Milliseconds between publishing data
const int PUBLISH_FREQ = 1000;

// Dump received bytes to the serial port
// not recommended for deployed module since the serial port
// is used for communication with the controller
#define VERBOSE false