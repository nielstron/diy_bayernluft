/*
 * Constant strings used for the serial communication
 * between module and controller
 */

// Ask for data preparation
// Average polling cycle ~ 1/s
// First sends poll1, then poll2
const String POLL1 = "02494B";
const String POLL2 = "02ACAE";

// Turn device on/off
const String POWER_ON = "03B900BA";
const String POWER_OFF = "03B900BA";

// Set speed of fans to value from 1-10
// or reset to speed before timer press for 0
const String SPEED[] = {
    "03D200D1",
    "03D201D0",
    "03D202D3",
    "03D203D2",
    "03D204D5",
    "03D205D4",
    "03D206D7",
    "03D207D6",
    "03D208D7",
    "03D209D8",
    "03D20ADB",
};

// Emulate pressing the respective buttons
// Hence this toggles the device power
const String BUTTON_POWER = "03BA00B9";
// And this simulates a timer press
const String BUTTON_TIMER = "02D3D1";