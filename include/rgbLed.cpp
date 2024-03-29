/*******************************************************************************
 * @file rgbLed.cpp  
 * @brief File containing all reset button RGB LED functions.
 *******************************************************************************/
#ifndef rgbLed_cpp // Start of precompiler check to avoid dupicate inclusion of this code block.
#define rgbLed_cpp // Precompiler macro used for precompiler check.

#include <main.h>    // Header file for all header files plus global_variables.cpp
// main.h is the master header file. Other .cpp files should include main.h as the only included header file

/**
 * @brief Set up a structure array of pre-defined colours for RGB LED.
 * @details The rest button has an embedded RGB LED that contains three
 * seperate red, green and blue LEDs. These three LEDs can be set to different
 * duty cycles in order to create a wide range of colours that the RGB LED can
 * display. This function creates an array of known colours in a structure 
 * that we can use to makes it easy to set the RGB LED to one of our
 * predefined colours. Use this tool for colour values: 
 * https://www.w3schools.com/colors/colors_picker.asp. The RGB LEDs on the
 * switch we use (https://www.adafruit.com/product/3350) are common anode
 * meaning that you turn them on by pulling them low, as opposed to common 
 * cathode RGB LEDs which you turn on by pulling them high. 
 * ==========================================================================*/
void createPredefinedColours()
{
   #undef localRNum
   #define localRNum 11
   //Log.traceln("<createPredefinedColours> Creating array of colours.");
   traceL("Creating array of colours");
   statusColour[RED].name = "RED";
   statusColour[RED].redDutyCycle = 256;
   statusColour[RED].greenDutyCycle = 0;
   statusColour[RED].blueDutyCycle = 0;
   
   //Log.verboseln("<createPredefinedColours> Red (%d) settings  - red = %d, green = %d, blue = %d.", RED, statusColour[RED].redDutyCycle, statusColour[RED].greenDutyCycle, statusColour[RED].blueDutyCycle);
   String rstatus = String(RED) +",  "+String(statusColour[RED].redDutyCycle) +", "+ String(statusColour[RED].greenDutyCycle) +", "+ String(statusColour[RED].blueDutyCycle);
   traceLs("for Red:","ID,RGB= "+ rstatus);

   statusColour[GREEN].name = "GREEN";
   statusColour[GREEN].redDutyCycle = 0;
   statusColour[GREEN].greenDutyCycle = 256;
   statusColour[GREEN].blueDutyCycle = 0;
   Log.verboseln("<createPredefinedColours> Green (%d) settings  - red = %d, green = %d, blue = %d.", GREEN, statusColour[GREEN].redDutyCycle, statusColour[GREEN].greenDutyCycle, statusColour[GREEN].blueDutyCycle);

   statusColour[BLUE].name = "BLUE";
   statusColour[BLUE].redDutyCycle = 0;
   statusColour[BLUE].greenDutyCycle = 0;
   statusColour[BLUE].blueDutyCycle = 256;
   Log.verboseln("<createPredefinedColours> Blue (%d) settings  - red = %d, green = %d, blue = %d.", BLUE, statusColour[BLUE].redDutyCycle, statusColour[BLUE].greenDutyCycle, statusColour[BLUE].blueDutyCycle);

   statusColour[YELLOW].name = "YELLOW";
   statusColour[YELLOW].redDutyCycle = 128; // 128
   statusColour[YELLOW].greenDutyCycle = 210; // 256
   statusColour[YELLOW].blueDutyCycle = 0; // 0
   Log.verboseln("<createPredefinedColours> YELLOW (%d) settings  - red = %d, green = %d, blue = %d.", YELLOW, statusColour[YELLOW].redDutyCycle, statusColour[YELLOW].greenDutyCycle, statusColour[YELLOW].blueDutyCycle);

   statusColour[ORANGE].name = "ORANGE";
   statusColour[ORANGE].redDutyCycle = 220; // 200
   statusColour[ORANGE].greenDutyCycle = 256; // 256
   statusColour[ORANGE].blueDutyCycle = 0; // 0
   Log.verboseln("<createPredefinedColours> ORANGE (%d) settings  - red = %d, green = %d, blue = %d.", ORANGE, statusColour[ORANGE].redDutyCycle, statusColour[ORANGE].greenDutyCycle, statusColour[ORANGE].blueDutyCycle);

   statusColour[PINK].name = "PINK";
   statusColour[PINK].redDutyCycle = 128; // 128
   statusColour[PINK].greenDutyCycle = 0; // 0
   statusColour[PINK].blueDutyCycle = 256; // 230
   Log.verboseln("<createPredefinedColours> PINK (%d) settings  - red = %d, green = %d, blue = %d.", PINK, statusColour[PINK].redDutyCycle, statusColour[PINK].greenDutyCycle, statusColour[PINK].blueDutyCycle);

   statusColour[AQUA].name = "AQUA";
   statusColour[AQUA].redDutyCycle = 0; // 0
   statusColour[AQUA].greenDutyCycle = 256; // 128
   statusColour[AQUA].blueDutyCycle = 256; // 256
   Log.verboseln("<createPredefinedColours> Aqua (%d) settings  - red = %d, green = %d, blue = %d.", AQUA, statusColour[AQUA].redDutyCycle, statusColour[AQUA].greenDutyCycle, statusColour[AQUA].blueDutyCycle);

   statusColour[WHITE].name = "WHITE";
   statusColour[WHITE].redDutyCycle = 40; // 64
   statusColour[WHITE].greenDutyCycle = 128; // 128
   statusColour[WHITE].blueDutyCycle = 128; // 128
   Log.verboseln("<createPredefinedColours> White (%d) settings  - red = %d, green = %d, blue = %d.", WHITE, statusColour[WHITE].redDutyCycle, statusColour[WHITE].greenDutyCycle, statusColour[WHITE].blueDutyCycle);

   statusColour[BLACK].name = "BLACK";
   statusColour[BLACK].redDutyCycle = 0;
   statusColour[BLACK].greenDutyCycle = 0;
   statusColour[BLACK].blueDutyCycle = 0;
   Log.verboseln("<createPredefinedColours> Black (%d) settings  - red = %d, green = %d, blue = %d.", BLACK, statusColour[BLACK].redDutyCycle, statusColour[BLACK].greenDutyCycle, statusColour[BLACK].blueDutyCycle);
} // createPredefinedColours()

/**
 * @brief Save the current RGB values of the reset button LED to memory.
 * @details Useful when you want to be able to temporarily change the colour
 * of the RGB LED on the reset button but want to be able to set it back to
 * the same colour afterwards.
 * ==========================================================================*/
void saveRgbColour()
{
   Log.verboseln("<saveRgbColour> Save Red = %d, Green = %d, Blue = %d.", ledcRead(PWM_RED_CHANNEL), ledcRead(PWM_GREEN_CHANNEL), ledcRead(PWM_BLUE_CHANNEL));
   memColour.redDutyCycle = ledcRead(PWM_RED_CHANNEL);
   memColour.greenDutyCycle = ledcRead(PWM_GREEN_CHANNEL);
   memColour.blueDutyCycle = ledcRead(PWM_BLUE_CHANNEL);
} // saveRgbColour()

/**
 * @brief Set the RGB values of the reset button LED from memory.
 * @details Set the value of the RGB LED inside the reset button using the 
 * value stored in memory by the last call t the function rememberCurRgbColour.
 * ==========================================================================*/
void loadRgbColour()
{
   Log.verboseln("<loadRgbColour> Load Red = %d, Green = %d, Blue = %d.", memColour.redDutyCycle, memColour.greenDutyCycle, memColour.blueDutyCycle);
   ledcWrite(PWM_RED_CHANNEL, memColour.redDutyCycle);
   ledcWrite(PWM_GREEN_CHANNEL, memColour.greenDutyCycle);
   ledcWrite(PWM_BLUE_CHANNEL, memColour.blueDutyCycle);
} // loadRgbColour()

/**
 * @brief Allows you to create a custom colour for the reset button RGB LED.
 * @param red Duty cycle of red LED inside the reset button RGB LED. 
 * @param green Duty cycle of green LED inside the reset button RGB LED. 
 * @param blue Duty cycle of blue LED inside the reset button RGB LED. 
 * @details Set the duty cycle for the three RGB channels which determines 
 * what colour the LED appears to be. 
 * ==========================================================================*/
void setCustRgbColour(uint32_t red, uint32_t green, uint32_t blue)
{
   uint32_t redDC;
   uint32_t greenDC;
   uint32_t blueDC;
   if(commonAnode) // RGB LED has commmon anode. 
   {
      Log.verboseln("<setStdRgbColour> RGB LED has common anode. Inverting duty cycle values.");
      redDC = 256 -  red; // Invert bits.
      greenDC = 256 - green; // Invert bits.
      blueDC = 256 - blue; // Invert bits.
   } //if
   else // RGB LED has commmon cathode.
   {
   redDC = red; // Use bits as defined.
   greenDC = green; // Use bits as defined.
   blueDC = blue; // Use bits as defined.
   } //else
   ledcWrite(PWM_RED_CHANNEL, redDC); // Set RGB LED red duty cycle.
   ledcWrite(PWM_GREEN_CHANNEL, greenDC); // Set RGB LED red green duty cycle.
   ledcWrite(PWM_BLUE_CHANNEL, blueDC); // Set RGB LED red blue duty cycle.
   Log.verboseln("<setCustRgbColour> Red = %d, Green = %d, Blue = %d.", redDC, greenDC, blueDC);   
} // setCustRgbColour()

/**
 * @brief Set status LED to one of the pre-defined colours we have set up.
 * @param ledColour a value assigned to a predefined colour (i.e. 0 = red). 
 * ==========================================================================*/
void setStdRgbColour(uint8_t ledColour)
{
   uint32_t redDC;
   uint32_t greenDC;
   uint32_t blueDC;
   if(numColoursSupported <= ledColour) // 8 predefined colours (7 and below valid).
   {
      uint8_t subColour = YELLOW;
      Log.warningln("<setStdRgbColour> Requested colour %d unknown. Setting RGB LED to %s.", ledColour, statusColour[subColour].name.c_str());
      ledColour = subColour;
   } // if
   else
   {
      Log.verboseln("<setStdRgbColour> Set status RGB LED to %s.", statusColour[ledColour].name.c_str());
   } // else
   if(commonAnode) // RGB LED has commmon anode. 
   {   
      Log.verboseln("<setStdRgbColour> RGB LED has common anode. Inverting duty cycle values.");
      redDC = 256 - statusColour[ledColour].redDutyCycle; // Invert red duty cycle.
      greenDC = 256 - statusColour[ledColour].greenDutyCycle; // Invert green duty cycle.
      blueDC = 256 - statusColour[ledColour].blueDutyCycle; // Invert blue duty cycle.
   } //if
   else // RGB LED has commmon cathode.
   {
      Log.verboseln("<setStdRgbColour> RGB LED has common cathode. Use unaltered duty cycle values.");
      redDC = statusColour[ledColour].redDutyCycle; 
      greenDC = statusColour[ledColour].greenDutyCycle;
      blueDC = statusColour[ledColour].blueDutyCycle;
   } // else
   ledcWrite(PWM_RED_CHANNEL, redDC); // Set RGB LED red duty cycle.
   ledcWrite(PWM_GREEN_CHANNEL, greenDC); // Set RGB LED red green duty cycle.
   ledcWrite(PWM_BLUE_CHANNEL, blueDC); // Set RGB LED red blue duty cycle.
   Log.verboseln("<setStdRgbColour> Red = %d, Green = %d, Blue = %d.", redDC, greenDC, blueDC);
} // setStdRgbColour()

/**
 * @brief Initialize the RGB LED embedded inside of the reset button.
 * @details Configure PWM frequency and resolution parameters for three
 * PWM channels then assign one channel to each of the primary colour LEDs 
 * embedded inside of the RGB LED ring located on the reset button.  
 * ==========================================================================*/
void setupStatusLed()
{
   createPredefinedColours(); // Create predefined colour settings.
   Log.traceln("<setupStatusLed> Initialize status RGB LED on reset button.");
   pinMode(G_RESET_RED_LED, OUTPUT); // Set GPIO pin connected to red LED inside of the reset button RGB LED to output.
   pinMode(G_RESET_GREEN_LED, OUTPUT); // Set GPIO pin connected to green LED inside of the reset button RGB LED to output.
   pinMode(G_RESET_BLUE_LED, OUTPUT); // Set GPIO pin connected to blue LED inside of the reset button RGB LED to output.
   ledcSetup(PWM_RED_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure red LED PWM properties.
   ledcAttachPin(G_RESET_RED_LED, PWM_RED_CHANNEL); // Attach PWM channel to pin connected to red LED on reset button.
   ledcSetup(PWM_GREEN_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure green LED PWM properties.
   ledcAttachPin(G_RESET_GREEN_LED, PWM_GREEN_CHANNEL); // Attach PWM channel to pin connected to green LED on reset button.
   ledcSetup(PWM_BLUE_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure blue LED PWM properties.
   ledcAttachPin(G_RESET_BLUE_LED, PWM_BLUE_CHANNEL); // Attach PWM channel to pin connected to blue LED on reset button.
} //setupStatusLed()

#endif // End of precompiler protected code block