#include <Wire.h>
#include <sht31_util.h>
#include <hyt313_util.h>
#include <config.h>


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


void setup(){
    
    #if VERBOSE
        Serial.begin(9600);
        Serial.println(ITC_ADDRESS);
    #endif
    // emulates being sht31
    Wire.begin(ITC_ADDRESS); // join I2C bus
    Wire.onRequest(emulate_sht31);
}

void loop(){
    delay(100);
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


void emulate_hyt313(){
    #if VERBOSE
        Serial.println("==================================");
        // Print whatever we receive
        while(0 < Wire.available()) // loop through all but the last
        {
            char c = Wire.read(); // receive byte as a character
            Serial.print(c, HEX);         // print the character
        }
        Serial.println("==================================");
    #endif

    // untested!
    uint16_t humid_v = humidity_to_14_bit(cur_humidity);
    uint16_t temp_v = temperature_to_14_bit(cur_temperature);
    Wire.write(humidity_i2c_format_high(0xC0 | humid_v));
    Wire.write(humidity_i2c_format_low(humid_v));
    Wire.write(temperature_i2c_format_high(humid_v));
    Wire.write(temperature_i2c_format_low(humid_v));
}