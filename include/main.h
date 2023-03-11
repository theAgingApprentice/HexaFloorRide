/*******************************************************************************
 * @file main.h  
 *******************************************************************************/
#ifndef main_h // Start of precompiler check to avoid dupicate inclusion of this code block.
#define main_h // Precompiler macro used for precompiler check.

// Arduino.h needs to precede reading .h files because they use types that it defines
#include <Arduino.h> // Arduino Core for ESP32. Comes with PlatformIO.

// include all libraries ============================================================================
// libraries that we've modified
#include <aaNetwork.h> // Wifi functions. 
#include <aaWebService.h> // Realtime web-based network config and OTA code updates.
#include <aaFlash.h> // Use Flash memory to store values that persist past reboot.
#include <aaMqtt.h> // Use MQTT for remote management and monitoring.

// libraries used as supplied by external sources
#include <Wire.h> // Required for I2C communication.
#include <Adafruit_PWMServoDriver.h> // https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log.
#include <Adafruit_GFX.h> // OLED graphics
#include <Adafruit_SH110X.h> // OLED text

// header files from other files/bags
#include <huzzah32_gpio_pins.h> // Map pins on Adafruit Huzzah32 dev board to friendly names.
#include <hexbot_gpio_pins.h> // Map Hexbot specific pin naming to generic development board pin names. 
#include <known_networks.h> // String arrays of known Access Points and their passwords.
#include <flows.h>      // parameter definitions and global storage for flows that define leg movement

// to keep linter happy: define global variables in main.h, include main.h in all .cpp files
#include <global_variables.cpp>  // global variable declarations and global macros

/************************************************************************************
 * @section mainDeclare Declare functions in main.cpp.
 ************************************************************************************/
void tracer(int subID, int functionID, int tType, int tLevel, String dataLabel, String sVar);
// void setup(); // Arduino mandatory function #1. Runs once at boot. 
// void loop(); // Arduino mandatory function #2. Runs continually.

#endif // End of precompiler protected code block