#include <Wire.h>

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
uint16_t humidity_to_14_bit(uint32_t humidity_tenth_percent){
    // h * 2^14 / 1000
    // no info lost since 14 < 32/2, so no bits are shoved "out"
    return (humidity_tenth_percent << 14)/1000;
}

// Convert tenth of temperature between -40 and 125 deg C to 14 bit representation
uint16_t temperature_to_14_bit(double humidity_tenth_percent){
    return (humidity_tenth_percent / 1650) * (1<<14);
}

// Convert tenth of temperature between -40 and 125 deg C to 14 bit representation
uint16_t temperature_to_14_bit(uint32_t humidity_tenth_percent){
    // h * 2^14 / 1650
    // no info lost since 14 < 32/2, so no bits are shoved "out"
    return (humidity_tenth_percent << 14)/1650;
}
