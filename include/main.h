/*******************************************************************************
 * @file main.h  
 *******************************************************************************/
#ifndef main_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define main_h // Precompiler macro used for precompiler check.

//#include <huzzah32_gpio_pins.h> // Mapping of ESP32 pins to Huzzah32 development board pins
#include <huzzah32_gpio_pins.h> // Map pins on Adafruit Huzzah32 dev board to friendly names.
#include <hexbot_gpio_pins.h> // Map Hexbot specific pin naming to generic development board pin names. 
#include <known_networks.h> // String arrays of known Access Points and their passwords.
#include <flows.h>      // parameter definitions and global storage for flows that define leg movement


/*******************************************************************************
 * @section mainIncludes Included libraries.
 *******************************************************************************/


/*******************************************************************************
 * @section mainVars Global variable definitions.
 * @details In order to get rid of "error: 'xxx' was not declared in this scope"
 * compiler errors list a function from any of the include files here. 
 *******************************************************************************/
// Config related variables


#endif // End of precompiler protected code block