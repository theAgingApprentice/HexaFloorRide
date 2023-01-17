/******************************************************************************
 * @file main.cpp
 *
 * @mainpage Hexbot firmware.
 *
 * @section intro_sec Introduction
 *
 * This code is the firmware for the six legged robot called Hexbot. Full
 * details on how to get the circuit and chassis for this robot are found
 * [here](https://github.com/theAgingApprentice/HexaFloorRide).
 *
 * @section dependencies Dependencies
 *
 * This sketch depends on the libraries which are #included further down in this file
 * 
 * @section author Author(s)
 *
 * Written by Old Squire and Doug Elliott.
 *
 * @section license license
 *
 * Copyright 2023 the Aging Apprentice
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: The above copyright
 * notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

// Arduino.h needs to precede reading .h files because they use types that it defines
#include <Arduino.h> // Arduino Core for ESP32. Comes with PlatformIO.

#include <main.h>    // Header file for all libraries needed by this program.
// main.h has includes for all other .h files. No other files should include .h files


// include all libraries ============================================================================
// libraries that we've modified
#include <aaChip.h> // Core (CPU) details that the code running on.
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

// global variables ===================================================================================
#include <global_variables.cpp>  // global variable declarations and global macros ====================

//=====================================================================================================
// OOB stuff that used to be in main.h
// it's positioned here because some of it has to be compiled before code that refers to it.
// However, it needs to follow the library inclusions above
// (being called from setup() isn't sufficient, because compiler needs to see this early)

   // these seem to be calls to OOP constructors or methods, which our new header standard excludes
   aaChip appCpu; // Access information about the ESP32 application microprocessor (Core1).
   aaNetwork network(HOST_NAME_PREFIX); // WiFi session management.
   aaFlash flash; // Non-volatile memory management. 
   aaMqtt mqtt; // Publish and subscribe to MQTT broker. 
   Adafruit_PWMServoDriver pwmDriver[numDrivers]; // Servo driver object.
   aaWebService localWebService(WEB_APP_TITLE); // Webserver hosted by microcontroller.
   Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
   IPAddress brokerIP; // IP address of the MQTT broker.

// our code modules and code "bags" ===================================================================
#include <flows.cpp>          // routines relates to leg movements
#include <terminal.cpp> // Serial port management.
#include <configDetails.cpp> // Show the environment details of this application.
#include <web.cpp> // Manage locally hosted web service. 
#include <rgbLed.cpp> // Control status LEDs.
#include <network.cpp> // Control networking activities.
#include <mqttBroker.cpp> // Establish connect to the the MQTT broker.
#include <i2c.cpp> // Scan I2C buses to see what devices are present.
#include <oled.cpp> // Control OLED.

// trace message generation function
//   -calls to this function are done as a result of encountering the trace macro
//   -operation of this function is controlled in real time by entries in the traceTable tTrce[maxTraceNum]
//   -traceTable can be modified in real time by an MQTT command
//   -documentation for tracing is in docs/trace-design.pd
void tracer(String name, int subID, int functionID, int tType, int tLevel, String dataLabel, float var)
{
   // crude initial implementation unconditionaly prints a formatted message
   String type = "?";    // translate type to letter that starts message
   if(tType == tStatus) {type = "S";}
   if(tType == tWarn  ) {type = "W";}
   if(tType == tError)  {type = "E";}

   // similar for trace level
   String level = "?";
   if(tLevel == tTop) {level = "T";}
   if(tLevel == tMed) {level = "M";}
   if(tLevel == tLow) {level = "L";}

   // build message in a string that can go to console and/or MQTT
   Serial.println("===== about to print trace message ====");
   String tMsg = "~" + type + level +" "+ name +","+ subID +"("+ functionID +"):" +dataLabel +var ;   // build first part of message
   sp1l(tMsg) ;     // print the string followed by a newline
   return ;
}

/**
 * @brief Standard Arduino initialization routine.
 * ==========================================================================*/
void setup()
{
   setupSerial(); // Set serial baud rate.
   setupTracing();      //build the table that controls trace generation before we do any
   Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
   Log.traceln("<setup> Start of setup.");
   trace("Setup-trace-test",1,3,1,1,"test trace from setup()  ",0);
   Log.verboseln("<setup> Initialize I2C buses.");
   Wire.begin(G_I2C_BUS0_SDA, G_I2C_BUS0_SCL, I2C_BUS0_SPEED); // Init I2C bus0.
   scanBus0();                                                 // Scan bus0 and show connected devices.
   Log.traceln("<setup> Initialize OLED.");
   initOled();
   Log.verboseln("<setup> Initialize status RGB LED.");
   setupStatusLed();       // Configure the status LED on the reset button.
   setStdRgbColour(WHITE); // Indicates that boot up is in progress.
   Log.verboseln("<setup> Set up wifi connection.");
   setupNetwork();
   setupPerBotConfig(); // do setup unique to each hexbot.(configDetails.cpp)
   Log.traceln("<setup> Initialize servo drivers.");
   Log.verboseln("<setup> Display robot configuration in console trace.");
   showCfgDetails(); // Show all configuration details in one summary.
   Log.verboseln("<setup> Initialize flows.");
   setupFlows();
   Log.verboseln("<setup> Review status flags to see how boot sequence went.");
   checkBoot();
   Log.traceln("<setup> End of setup.");
} // setup()

/**
 * @brief Standard Arduino main loop.
 * ==========================================================================*/
void loop()
{
   // check for hi priority 20msec loop first. Should serialize other tasks too,
   //  so at most one of them is done per loop()
   // ==== track how well we're doing at running the 20msec on time.
   if (f_flowing == true) // are we executing a predefined flow between positions?
   {
      do_flow(); // yes. caclulate and do next servo commands. It's in flow.cpp module
   }
   monitorWebServer(); // Handle any pending web client requests.
   checkOledButtons(); // Check if an OLED button has been pressed.
   checkMqtt();        // Check the MQTT message queue for incoming commands.
} // loop()