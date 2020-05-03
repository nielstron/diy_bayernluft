#include "user_config.h"
#include "serial_comm.h"
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

WiFiClient wifi;
MQTTClient mqtt;
WiFiServer server(80);

struct State{
    // TODO We don't know how to read
    // most of these values
    bool power;
    bool timer;
    int fan_speed;
    double temperature_in;
    double temperature_out;
    char short_dump[100];
    char dump[1000];
};
State controller_state;
unsigned long last_publish = 0;


void setup(){
    // Begin serial connection to controller
    Serial.begin(115200);
    // Setup wifi connection
    setupWifi();
    // Setup MQTT connection
    setupMQTT();
    setupServer();
}

void loop(){
    mqtt.loop();
    reconnectMQTT();

    handle_server_request();

    if(millis() - last_publish > PUBLISH_FREQ){
        readState();
        publishState();
        last_publish = millis();
    }

}

// Reactions to MQTT messages
void messageReceived(String &topic, String &payload) {
    #if VERBOSE
        Serial.println("Received \"" + payload + "\" on topic " + topic)
    #endif
    if(topic.equalsIgnoreCase(MQTT_CONTROL_BASE_TOPIC + "/power")){
        if(payload.equalsIgnoreCase("on")){
            Serial.println(POWER_ON);
        }
        if(payload.equalsIgnoreCase("off")){
            Serial.println(POWER_OFF);
        }
        if(payload.equalsIgnoreCase("toggle")){
            Serial.println(BUTTON_POWER);
        }
    }
    if(topic.equalsIgnoreCase(MQTT_CONTROL_BASE_TOPIC + "/speed")){
        int speed = payload.toInt();
        if(speed >= 0 && speed <= 10){
            Serial.println(SPEED[speed]);
        }
    }
    if(topic.equalsIgnoreCase(MQTT_CONTROL_BASE_TOPIC + "/timer")){
        Serial.println(BUTTON_TIMER);
    }
}

void setupWifi() {
    delay(10);
    // We start by connecting to a WiFi network
    #if VERBOSE
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(WIFI_SSID);
    #endif

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        #if VERBOSE
            Serial.print(".");
        #endif
    }

    #if VERBOSE
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    #endif
}

void setupMQTT(){
    mqtt.begin(MQTT_HOST.c_str(), MQTT_PORT, wifi);
    mqtt.onMessage(messageReceived);
    mqtt.subscribe(MQTT_CONTROL_BASE_TOPIC);
}

void setupServer(){
    server.begin();
}

void reconnectMQTT() {
    // Loop until we're reconnected
    while (!mqtt.connected()) {

        #if VERBOSE
            Serial.print("Attempting MQTT connection...");
        #endif
        // Attempt to connect
        if (mqtt.connect(MQTT_CLIENT.c_str(), MQTT_CLIENT.c_str(), MQTT_PASS.c_str())) {
            #if VERBOSE
                Serial.println("connected");
            #endif
        } 
        else {
            #if VERBOSE
                Serial.print("failed, rc=");
                Serial.print(mqtt.state());
                Serial.println(" try again in 5 seconds");
            #endif
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void readState(){
    // Read the current controller state
    // TODO as of now we have no idea what this means...
    Serial.println(POLL1);
    memset(controller_state.short_dump, 0, 100);
    delay(1);
    Serial.readBytes(controller_state.short_dump, 99);
    Serial.println(POLL2);
    memset(controller_state.dump, 0, 1000);
    delay(1);
    Serial.readBytes(controller_state.dump, 999);
}

void publishState(){
    mqtt.publish((MQTT_PUBLISH_BASE_TOPIC + "/short_dump").c_str(), controller_state.short_dump);
    mqtt.publish((MQTT_PUBLISH_BASE_TOPIC + "/dump").c_str(), controller_state.dump);
}

void handle_server_request(){
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects,
        #if VERBOSE
            Serial.println("New Client.");          // print a message out in the serial port
        #endif
        String server_header = "";
        if (client.available()) {             // if there's bytes to read from the client,
            String currentLine = client.readStringUntil('\r');             // read a byte, then
            server_header += currentLine;
            #if VERBOSE
                Serial.write(currentLine);                    // print it out the serial monitor
            #endif
            // wait for end of client's request, that is marked with an empty line
            if (currentLine.length() == 1 && currentLine[0] == '\n'){
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                // The HTTP response ends with another blank line
                client.println(controller_state.short_dump);
                client.println(); 
                client.println(controller_state.dump);
                client.println(); 
                client.println(); 
                // turns the power on and off
                if (server_header.indexOf("?power=on") >= 0) {
                    Serial.println(POWER_ON);
                }
                if (server_header.indexOf("?power=off") >= 0) {
                    Serial.println(POWER_OFF);
                }
                for(int i = 0; i < 10; i ++){
                    if(server_header.indexOf("?speed="+i) >= 0){
                        Serial.println(SPEED[i]);
                    }
                }
                if (server_header.indexOf("?button=power") >= 0) {
                    Serial.println(BUTTON_POWER);
                }
                if (server_header.indexOf("?button=timer") >= 0) {
                    Serial.println(BUTTON_TIMER);
                }
            }
        }
        // Close the connection
        client.stop();
        #if VERBOSE
            Serial.println("Client disconnected.");
            Serial.println("");
        #endif
    }
}