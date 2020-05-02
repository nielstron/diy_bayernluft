#include <stdio.h>
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

const int polynomial = 0x131;
const int poly_degree = 8;
// init = 0xFF;

// Convert percents to 16 bit representation
uint16_t humidity_to_16_bit(double humidity_percent){
    return (humidity_percent / 100) * (1<<16);
}

// Convert temperature between -45 and 125 deg C to 16 bit representation
uint16_t temperature_to_16_bit(double temperature){
    return ((temperature +45) / 175) * (1<<16);
}

int main(int argc, char const *argv[])
{
    uint32_t hum_conv = (100*humidity_to_16_bit(50))/((1<<16) -1);
    printf("%d\n", hum_conv);
    uint32_t temp_conv = -45+(175* (temperature_to_16_bit(20)))/((1<<16)-1);
    printf("%d\n", temp_conv);
    return 0;
}
