#include <Wire.h>

#include "util_sht31.h"
#include "util_hyt313.h"

#include "user_config.h"
//DHT (22)
#include <DHT.h>

// Raw temperature to be emulated comes in integer format
// in tenths of a degree Celsius
#define INT_TEMP false

// Raw humidity to be emulated comes in integer format
// in tenths of a percent relative humidity
#define INT_HUMIDITY false

#if INT_TEMP
    typedef uint32_t temp_format;
#else
    typedef double temp_format;
#endif

#if INT_HUMIDITY
    typedef uint32_t humidity_format;
#else
    typedef double humidity_format;
#endif

// Update these values to change the currently emulated values
temp_format cur_temperature = 20;
humidity_format cur_humidity = 50;

// DHT sensor connected at pin 12 and model number 22
DHT dht(12, DHT22);

void setup(){
    
    #if VERBOSE
        Serial.begin(9600);
        Serial.println(ITC_ADDRESS);
    #endif
    // emulates being sht31
    Wire.begin(ITC_ADDRESS); // join I2C bus
    Wire.onRequest(emulate_sht31);

    // Set up DHT22 sensor
    dht.begin();
}

void loop(){
    float dht_temp = dht.readTemperature();
    if (isnan(dht_temp)) {
        #if VERBOSE
            Serial.println("Error reading temperature DHT ");
        #endif
    }
    else {
        #if VERBOSE
            Serial.println("Temperature: " + String(dht_temp) + " °C");
        #endif
        cur_temperature = dht_temp;
    }

    float dht_hum = dht.readHumidity();
    if (isnan(dht_hum)) {
        #if VERBOSE
            Serial.print("Error reading humidity DHT ");
        #endif
    }
    else{
        #if VERBOSE
            Serial.println("Humidity: " + String(dht_temp) + " °C");
        #endif
        cur_humidity = dht_hum;
    }
    // One reading every second
    delay(1000);
}


void emulate_sht31(){
    #if VERBOSE
        Serial.println("====================");
        // Print whatever we receive
        while(0 < Wire.available()) // loop through all but the last
        {
            char c = Wire.read(); // receive byte as a character
            Serial.print(c, HEX);         // print the data in hex
        }
        Serial.println("");
        Serial.println("====================");
    #endif

    uint16_t hum = humidity_to_16_bit(cur_humidity);
    uint16_t temp = temperature_to_16_bit(cur_temperature);
    // Print data in big endian (while Arduino boards have little endianess internally)
    Wire.write((uint8_t)(temp >> 8));
    Wire.write((uint8_t)(temp && 0x00FF));
    Wire.write(Crc8_2byte(temp));
    Wire.write((uint8_t)(hum >> 8));
    Wire.write((uint8_t)(hum && 0x00FF));
    Wire.write(Crc8_2byte(hum));

}
