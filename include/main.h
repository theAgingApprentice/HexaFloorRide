/*******************************************************************************
 * @file main.h  
 *******************************************************************************/
#ifndef main_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define main_h // Precompiler macro used for precompiler check.

// header files from other files/bags
#include <huzzah32_gpio_pins.h> // Map pins on Adafruit Huzzah32 dev board to friendly names.
#include <hexbot_gpio_pins.h> // Map Hexbot specific pin naming to generic development board pin names. 
#include <known_networks.h> // String arrays of known Access Points and their passwords.
#include <flows.h>      // parameter definitions and global storage for flows that define leg movement

/************************************************************************************
 * @section mainDeclare Declare functions in main.cpp.
 ************************************************************************************/
void tracer(String name, int subID, int functionID, int tType, int tLevel, String dataLabel, float var);
// void setup(); // Arduino mandatory function #1. Runs once at boot. 
// void loop(); // Arduino mandatory function #2. Runs continually.



#endif // End of precompiler protected code block