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

/*
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
*/

#include <main.h>    // Header file for all header files plus global_variables.cpp
// main.h is the master header file. Other .cpp files should include main.h as the only included header file

// global variables ===================================================================================
//129 #include <global_variables.cpp>  // global variable declarations and global macros ====================

//=====================================================================================================
// OOB stuff that used to be in main.h
// it's positioned here because some of it has to be compiled before code that refers to it.
// However, it needs to follow the library inclusions above
// (being called from setup() isn't sufficient, because compiler needs to see this early)

   // these seem to be calls to OOP constructors or instantiations, methods, which our new header standard excludes
   aaNetwork network(HOST_NAME_PREFIX); // WiFi session management.
   aaMqtt mqtt; // Publish and subscribe to MQTT broker. 
   Adafruit_PWMServoDriver pwmDriver[numDrivers]; // Servo driver object.
   aaWebService localWebService(WEB_APP_TITLE); // Webserver hosted by microcontroller.
   Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
   Preferences preferences; // Save to SPI NOR flash for variables to persist past reset

// our code modules and code "bags" ===================================================================
#include <flows.cpp>          // routines relates to leg movements
#include <terminal.cpp> // Serial port management.
#include <configDetails.cpp> // Show the environment details of this application.
#include <web.cpp> // Manage locally hosted web service. 
#include <deviceSupport.cpp>  // suppport for h/w devices, initially PCA9685 PWM driver
#include <rgbLed.cpp> // Control status LEDs.
#include <network.cpp> // Control networking activities.
#include <mqttBroker.cpp> // Establish connect to the the MQTT broker.
#include <i2c.cpp> // Scan I2C buses to see what devices are present.
#include <oled.cpp> // Control OLED.


/**
 * @brief Standard Arduino initialization routine.
 * ==========================================================================*/
void setup()
{
   #undef localRNum     // avoid compiler warnings if we redefine localRNum
   #define localRNum 7

   setupSerial(); // Set serial baud rate.
   setupTracing();      //build the trace control table in configDetails.cpp before any trace usage
   Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
   traceH("Start of setup ##########################################################################");
   //Log.verboseln("<setup> Initialize I2C buses.");
   traceL("Initialize I2C buses");
   Wire.begin(G_I2C_BUS0_SDA, G_I2C_BUS0_SCL, I2C_BUS0_SPEED); // Init I2C bus0.
   scanBus0();                                                 // Scan bus0 and show connected devices.
   //Log.traceln("<setup> Initialize OLED.");
   traceL("Initialize OLED");
   initOled();
   setupOnboardLED();   // prepare LED for use, GPio symbol = OnboardLED
   //Log.verboseln("<setup> Initialize status RGB LED.");
   traceL("Initialize RGB LED");
   setupStatusLed();       // Configure the status LED on the reset button.
   setStdRgbColour(WHITE); // Indicates that boot up is in progress.
   //Log.verboseln("<setup> Set up wifi connection.");
   traceL("Setup Wifi connection");
   setupNetwork();
   setupPerBotConfig(); // do setup unique to each hexbot.(configDetails.cpp)
   //Log.traceln("<setup> Initialize servo drivers.");
   traceL("Setup servo drivers");
   setupPwmdriver();
   //Log.verboseln("<setup> Display robot configuration in console trace.");
   traceL("Display robot configuration");
   // if($traceTab[t$global] & (t$L))  // temp kludge to suppress chip detail messages
   {   showCfgDetails(); // Show all configuration details in one summary.
   }   // need to recode aaChip, or modify all embedded messges
   // need to collapse aaChip library to one subroutine for above
   //Log.verboseln("<setup> Initialize flows.");
   traceL("Initialize leg movement flows");
   setupFlows();
   //Log.verboseln("<setup> Review status flags to see how boot sequence went.");
   traceM("Review boot sequence status flags");
   checkBoot();
   //Log.traceln("<setup> End of setup.");
   setupTasks();   // initialize for millis() based and condition based task dispatches in loop() (in deviceSupport.cpp)
   
   traceM("End of setup #############################################################################");
} // setup()

/**
 * @brief Standard Arduino main loop.
 * ==========================================================================*/
void loop()
{
   // check for hi priority 20msec loop first. Should serialize other tasks too,
   //  so at most one of them is done per loop()

   // each routine called from loop() has its execution and dispatch timing accumulated and
   // periodically CPU performance info is processed, displayed, and reset for next period.

   // Some of these tasks are triggered by expiration of a millis() timer, and use the macro timerTask(task_call,taskID)
   // others are triggered by some logic condition becoming true, and use the macro conditionTask(task_call,taskID,condition)

   // the following macro dispatches routine based on timer expiry with metrics accumulation for named taskID
   // arguments:
   // task_call is the statement that calls the routine, with no semicolon
   //           example: process_sensors()
   // taskID is a brief name for the task, 4 - 8 letters long, which is embedded in variable names
   //           example: sensor

            #define timerTask(task_call,taskID) \
            mills = millis(); \
            mics = micros(); \
            if(mills >= next_##taskID##_mills ) \
            {  sched_##taskID##_mills = next_##taskID##_mills; \
               deltaSched = mills - sched_##taskID##_mills; \
               cum_##taskID##_delay = cum_##taskID##_delay + deltaSched; \
               if(deltaSched > max_##taskID##_delay) {max_##taskID##_delay = deltaSched;}  \
               if(deltaSched > 0 ) {lates_##taskID = lates_##taskID + 1;} \
                                 \
               task_call;  \
                                 \
               deltaTime = micros() - mics;  \
               if(deltaTime < 0) \
               {  sp2sl("----------------------------------------------------old Delta time=",deltaTime); \
                  deltaTime += 4 * 1024 * 1024; \
                  sp2sl("----------------------------------------------------new Delta time=",deltaTime); \
               } \
               time_##taskID##_mics += deltaTime;  \
               if(deltaTime > max_##taskID##_mics) { max_##taskID##_mics = deltaTime;} \
            }

   // the following macro does dispatch and metrics accumulation for routines controlled by some logic condition
   // being true, which could be a flag bit being set, or a measurement reaching a threshold
   // example conditions:
   //   (flags & dispatchBit) != 0
   //   batteryV < threshold2
   // CPU usage is measured for conditional tasks, but not dispatch delays
   // Note that the routine is responsible for clearing the condition that triggers dispatch
   // Arguments:
   // task_call is the statement that calls the routine, with no semicolon
   //           example: process_sensors()
   // taskID is a brief name for the task, 4 - 8 letters long, which is embedded in variable names
   //           example: sensor
   // condition is a logical expression that is used to trgger the event as described above
  
            #define conditionTask(task_call,taskID,condition) \
            mills = millis(); \
            mics = micros(); \
            if( condition ) \
            {  task_call;  \
               deltaTime = micros() - mics;  \
               if(deltaTime < 0) { deltaTime += 4 * 1024 * 1024; } \
               time_##taskID##_mics += deltaTime;  \
               if(deltaTime > max_##taskID##_mics) { max_##taskID##_mics = deltaTime;} \
            }

 // order of appearance determines priority (once we get so we only execute one task per loop() )

   // flow processing that does servo position changes to move legs. In flows.cpp
   conditionTask(do_flow(),flow,f_flowing == true);  // flows that control servo position changes that move legs

   // setup processing for a new flow, including initial servo positioning
   //conditionTask(flowRow_0,fl0,((f_flowing == true) && f_active == 0)); // handle flow row 0 including initial positioning

   //processing for flow row 1, first one with frames, after a 340 msec pause for servo movement after flowRow_0
   //timerTask(flowRow_1,fl1)      // check to see if 340 msec timer has expired, and if so, process flow row 1

   //processing for remaining flow rows, after the time delay between frames
   //timerTask(flowRow_x,flx)      // when the time comes, recalculate toe position for next frame, and go there

   // look for, and process any MQTT commands that came in from the MQTT broker. In mqttBroker.cpp
   timerTask(checkMqtt(),checkMqtt);               // check for MQTT work to be done

   // do the once per second processing, including calculating and dispalying CPU usage information. In main.cpp
   timerTask(oneSec(),oneSec);                     // once a second routine that does CPU usage calculations

   // check for new broker IP address, or pending file transfer (OTA?). In web.cpp
   timerTask(monitorWebServer(),webMon);           // monitor web service (new broker IP, new file transfers)

   // check for Oled button presses, and update display. In Oled.cpp
   timerTask(checkOledButtons(),checkOled);        // check Oled Buttons()

// ... and cycle around and do loop() again.
 
} // loop()


// ========================================================================================================
void oneSec()     // routine that executes once per second to display CPU performance information
                  // and maybe do other timer related sub-tasks
{
   next_oneSec_mills = millis() + period_oneSec_mills ;   //quickly schedule our next execution - 1000 msec in future
   #undef localRNum
   #define localRNum 14
   String rep;

   // next macro clears out variables at startup, and at end of once per second processing
            #define taskProcInit(taskID)  \
            cum_##taskID##_delay = 0; \
            max_##taskID##_delay = 0; \
            lates_##taskID = 0; \
            time_##taskID##_mics = 0; \
            max_##taskID##_mics = 0;


   // code execution starts here - first entry is a special case

   if(firstOneSec)      // on the first entry, just do some setup stuff
   {  
      // zero all the counters
      taskProcInit(webMon);
      taskProcInit(checkOled);
      taskProcInit(checkMqtt);
      taskProcInit(flow);
      taskProcInit(oneSec);

      // and flag that we've completed initialization and are operational
      firstOneSec = false;    // next entry will be the real thing
   }
   else; // this is the normal entry after the initial setup is done
   {  
      // when called, next macro does actual processing of accumulated data for one TIMER-driven taskID
      // label argument is the very brief text ID info that prefixes CPU usage numbers on each displayed line

      char buffer[20]; 
            #define doTimerProc(taskID,label) \
            rep = rep + label +":"; \
            dtostrf(float(time_##taskID##_mics)/10000.,4,1,buffer); \
            totCPU += float(time_##taskID##_mics)/10000. ; \
            rep = rep + buffer +" "; \
            dtostrf(float(max_##taskID##_mics)/10000.,4,1,buffer); \
            rep = rep + buffer+ " "+String(lates_##taskID) + " /";

      // when called, next macro does actual processing of accumulated data for one CONDITION-driven taskID
            #define doConditionProc(taskID,label) \
            rep = rep + label +":"; \
            dtostrf(float(time_##taskID##_mics)/10000.,4,1,buffer); \
            totCPU += float(time_##taskID##_mics)/10000. ; \
            rep = rep + buffer +" "; \
            dtostrf(float(max_##taskID##_mics)/10000.,4,1,buffer); \
            rep = rep + buffer+ " /";

      // start of actual processing that's done each second...

      float totCPU = 0.;      // accumulate total CPU usage from 1st number from each task's report
      String rep = "";        // report string is extended for each taskID
      // capstate: quick and dirty state machine to debounce capacitance input
         // 0 = output enabled
         // 1 = output disabled
         // 2 = going to enabled
         // 3 = going to disabled

      doTimerProc(checkOled,"/Oled");
      doTimerProc(checkMqtt,"MQTT");
      doTimerProc(webMon,"webMon");
      doConditionProc(flow,"Flow");    // dispatched by f_flowing bit now, but will be broken up
      doTimerProc(oneSec,"1Sec");

      // testing reading capacitive sensors as a way to stall CPU usage output
      int pinval = 1;    // assume pin is high, i.e. over 30
      int cap27 = touchRead(27);  //physical pin 25, 6th down from battery connector
      if(cap27 < 40 ) {pinval = 0;}    // 40 is the observed threshold between touched (low #) and untouched (high #)
      int touched = 0;   // pinval value that means sensor is being touched
      int untouched = 1;   // pinval value that means sensor is not being touched
      
      int newstate = capstate;        // intermediary variable to avoid unplanned change consequences in if chain below
      //do any state transitions based on current pinval and current state
      if((capstate == 0) && (pinval == touched) ) {newstate = 3; } // transition state to disabled
      else if((capstate == 1) && (pinval == touched) ) {newstate = 2; } // transition state to enabled
      else if(capstate == 2 ) 
      {   if(pinval == touched) { newstate = 0; digitalWrite(OnboardLED,LOW); } // confirmed transition
          else { newstate = 1; } //failed transition
      }
      else if(capstate == 3 )
      {   if(pinval == touched) { newstate = 1 ; digitalWrite(OnboardLED,HIGH); } // confirmed transition
          else { newstate = 0; } // failed transition
      }
      capstate = newstate;    // make the state change from the above logic

      String cpuUse;
      // rep = rep + " cap:" +String(cap27);  // append current value of capacitive sensor
      cpuUse = "CPU: " + String(totCPU) + " " + rep;
      
      if(capstate == 0)    // if capacitive sensor has put/left us in output enabled mode, do it
      {
      traceMs("",cpuUse);  // trace can be enabled/disabled on the fly, and directed to MQTT and or console
                           // hint: using MQTT avoids console's tendency to lose older output
                           // and generates interleaved timestamped CPU info, which can be useful, or messy.
      }
      if( totCPU > 90)     // if the CPU is very busy, issue a red alert
      {
         traceEf("CPU Utilization RED alert = usage% =",totCPU);
         CPULoad = CPURed ;   // note high CPU usage for load shedding purposes
         CPUOffLoad();        // try to reduce CPU usage somehow
      }
      else if (totCPU > 60)   // if the CPU is busy, issue a yellow alert
      {  
         traceEf("CPU Utilization YELLOW alert = usage% =",totCPU);
         CPULoad = CPUYellow ; // note high CPU usage for load shedding purposes
         CPUOffLoad();         // try to reduce CPU usage somehow
      }
      else
      {
         CPULoad = CPUNormal;  // we're comfortable - no action needed
      }
      
      // clean up in preparation for next second's data accumulation
      taskProcInit(checkOled);
      taskProcInit(checkMqtt);
      taskProcInit(webMon);
      taskProcInit(flow);
      taskProcInit(oneSec);

   } // if(firstOneSec) .. else

} // void oneSec()

void CPUOffLoad()       // attempt to reduce load on CPU
{
   // routines that can be delayed should be checking CPULoad variable & delaying appropriately
   // temporary CPU burst will be detected in oneSec, and may be gone before load shedding can occur
   // not quite sure what action this routine can take

   int dummy = 1; // compiler doesn't like empty functions
}
