#include <Wire.h>

// Problem: can only receive as one address
// at a time
// Solution: Stays at one address for 5 seconds, then switches to next address

int cur_address;

// brings the lower 8 bits of a 14 bit humidity value to the correct format
uint8_t humidity_i2c_format_low(uint16_t humidity_strechted){
    return 0xFF & humidity_strechted;
}

// brings the upper 8 bits of a 14 bit humidity value to the correct format
uint8_t humidity_i2c_format_high(uint16_t humidity_strechted){
    return 0x3F & (humidity_strechted >> 8);
}

// brings the lower 8 bits of a 14 bit temperature value to the correct format
uint8_t temperature_i2c_format_low(uint16_t temperature_strechted){
    return (0x3F & temperature_strechted) << 2;
}

// brings the upper 8 bits of a 14 bit temperature value to the correct format
uint8_t temperature_i2c_format_high(uint16_t temperature_strechted){
    return 0xFF & (temperature_strechted >> 6);
}

// Convert tenth percents to 14 bit representation
uint16_t humidity_to_14_bit(double humidity_tenth_percent){
    return (humidity_tenth_percent / 1000) * (1<<14);
}

// Convert tenth percents to 14 bit representation
uint16_t humidity_to_14_bit(int humidity_tenth_percent){
    // h * 2^14 / 1000
    // no info lost since 14 < 32/2, so no bits are shoved "out"
    return (humidity_tenth_percent << 14)/1000;
}

// Convert tenth of temperature between -40 and 125 deg C to 14 bit representation
uint16_t temperature_to_14_bit(double humidity_tenth_percent){
    return (humidity_tenth_percent / 1650) * (1<<14);
}

// Convert tenth of temperature between -40 and 125 deg C to 14 bit representation
uint16_t temperature_to_14_bit(int humidity_tenth_percent){
    // h * 2^14 / 1650
    // no info lost since 14 < 32/2, so no bits are shoved "out"
    return (humidity_tenth_percent << 14)/1650;
}

void setup(){
    
    Serial.begin(9600);

}

void loop(){
    // emulates being hyt for any address for 5 seconds
    for(int address = 0x0; address < 0x7F; address++){
        Wire.begin((char) address); // join I2C bus
        Wire.onRequest(emulate_hyt313);
        cur_address = address;
        Serial.println(cur_address);

        // wait for bayernlüfter to ask for values
        for(int i = 0; i < 20; i++){
            delay(100);
        }
    }
}

/* From JeeLib
void HYT131::reading (int& temp, int& humi, byte (*delayFun)(word ms)) {
    // Start measurement
    send();
    stop();
    
    // Wait for completion (using user-supplied (low-power?) delay function)
    if (delayFun)
        delayFun(100);
    else
        delay(100);
    
    // Extract readings
    receive();
    uint16_t h = (read(0) & 0x3F) << 8;
    h |= read(0);
    uint16_t t = read(0) << 6;
    t |= read(1) >> 2;
    
    // convert 0..16383 to 0..100% (*10)
    humi = (h * 1000L >> 14);
    // convert 0..16383 to -40 .. 125 (*10)
    temp = (t * 1650L >> 14) - 400;
}
*/

void emulate_hyt313(){
    // First print our address if we receive something
    Serial.println(cur_address);
    Serial.println("");
    // Print whatever we receive
    while(1 < Wire.available()) // loop through all but the last
    {
        char c = Wire.read(); // receive byte as a character
        Serial.print(c);         // print the character
    }
    int x = Wire.read();    // receive byte as an integer
    Serial.println(x);
    Serial.println("");
    // TODO

    uint16_t humid_v = humidity_to_14_bit(cur_address); //TODO
    uint16_t temp_v = temperature_to_14_bit(cur_address); // TODO
    //switch(howMany){
    //    case 1:
        // One byte is for MR
        // start measuring cycle
    //    break;
    //    case 2:
     //   case 3:
        // Two bytes is for capacitance = humidity data only
        // Three bytes is for capacitance & temperature data
        // TODO what value should the two status bits have? both one?
        Wire.write(humidity_i2c_format_high(0xC0 | humid_v));
        Wire.write(humidity_i2c_format_low(humid_v));
        Wire.write(temperature_i2c_format_high(humid_v));
        Wire.write(temperature_i2c_format_low(humid_v));
    //}
}
