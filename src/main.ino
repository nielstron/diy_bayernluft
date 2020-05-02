#include <Wire.h>

#ifdef SENSOR_RED
    #define ITC_ADRESS 0x44
#else
    #define ITC_ADDRESS 0x44
#endif

int cur_address;

const int polynomial = 0x131;
const int poly_degree = 8;
// init = 0xFF;

uint8_t Crc8_2byte(uint16_t data) {
    uint32_t cur_data = ((uint16_t) data) << poly_degree; // 16 bit data, padded with 8 zeroes
    // init = 0xFF <=> invert first 8 bit
    cur_data ^= 0xFF0000;
    // shift polynomial left s.t. leftmost bit (at pos poly_degree ) is at position 23 (0-indexed)
    uint32_t cur_poly = (polynomial) << (23 - poly_degree);
    for(int i = 23; i > poly_degree; i--){
        // if leftmost bit set
        if(cur_data >> i){
            cur_data ^= cur_poly;
        }
        cur_poly = cur_poly >> 1;
    }
    // return remaining 8 bits of data
    return (uint8_t) cur_data;
}

// Convert percents to 16 bit representation
uint16_t humidity_to_16_bit(double humidity_percent){
    return (humidity_percent / 100) * (1<<16 - 1);
}

// Convert percents to 16 bit representation
uint16_t humidity_to_16_bit(uint32_t humidity_percent){
    // h * (2^16-1) / 100
    // no information lost since shifting left by 16 causes no overflow
    // (bc log2(100) << 16)
    return (humidity_percent*((1<< 16) - 1))/100;
}

// Convert temperature between -45 and 125 deg C to 16 bit representation
uint16_t temperature_to_16_bit(double temperature){
    return ((temperature +45) / 175) * (1<<16 - 1);
}

// Convert temperature between -40 and 125 deg C to 16 bit representation
uint16_t temperature_to_16_bit(uint32_t temperature){
    // (t+45) * (2^16-1) / 175
    return ((temperature+45) *((1<< 16) - 1))/175;
}

void setup(){
    
    Serial.begin(9600);
    // emulates being sht31
    cur_address = ITC_ADDRESS;
    Wire.begin((char) cur_address); // join I2C bus
    Wire.onRequest(emulate_sht31);
    Serial.println(cur_address);
}

void loop(){
    delay(100);
}


void emulate_sht31(){
    Serial.println("====================");
    // Print whatever we receive
    while(0 < Wire.available()) // loop through all but the last
    {
        char c = Wire.read(); // receive byte as a character
        Serial.print(c, HEX);         // print the data in hex
    }
    Serial.println("");
    Serial.println("====================");

    uint16_t hum = humidity_to_16_bit((uint32_t) 50);
    uint32_t temp = temperature_to_16_bit((uint32_t) 20);
    Wire.write((uint8_t)(hum >> 8));
    Wire.write((uint8_t)(hum && 0x00FF));
    Wire.write(Crc8_2byte(hum));
    Wire.write((uint8_t)(temp >> 8));
    Wire.write((uint8_t)(temp && 0x00FF));
    Wire.write(Crc8_2byte(temp));
}
