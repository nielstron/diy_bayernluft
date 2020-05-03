const int ITC_ADDRESS_RED = 0x44;
const int ITC_ADDRESS_GREEN = 0x45;

// Compile code for the red (upper) sensor
// or for the green (lower) sensor
const int ITC_ADDRESS = ITC_ADDRESS_GREEN;

// Dump received bytes to the serial port
#define VERBOSE false