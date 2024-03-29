/*******************************************************************************
 * @file configDetails.cpp  
 * @brief setup & display config parameters and boot status
 *******************************************************************************/
#ifndef showCfgDetails_cpp // Start of precompiler check to avoid dupicate inclusion of this code block.
#define showCfgDetails_cpp // Precompiler macro used for precompiler check.

#include <main.h>    // Header file for all header files plus global_variables.cpp
// main.h is the master header file. Other .cpp files should include main.h as the only included header file

/**
 * @brief Do configuration set up that is unique to each robot, including:
 * - define servo calibration offset values
 * called from setup() in main.cpp
 * tried to put this routine in hexBot/lib/aaWeb-1.0.0/lib/aaNetwork-main/src/aaNetwork.cpp
 *    but couldn't get it to work, due to object structure issues I suspect
 =============================================================================*/
 void setupPerBotConfig()     // do the robot-specific setup
 {
    #undef localRNum
    #define localRNum 9
    String macAdd = WiFi.macAddress(); // Get MAC address as String
    //sp2("<setupPerBotConfig> MAC Address: ", macAdd);  // see if we can read this variable
    traceLs("MAC Address: ", macAdd);

    // handle the case where Doug's using newest ESP32 in new robot,
    //   and the case where CPU from old robot is swapped into new robot
    // if you're running this code, it must be a release 2 robot supporting soft servo offsets
    if(macAdd == "3C:61:05:4A:DD:98" || macAdd == "94:B9:7E:5F:48:B8")  // Doug's 2 ESP32 MAC addresses
    {
       //sp1l("  Doing bot-specific setup for one of Doug's MAC addresses");
       traceMs("Config setup for Doug's MAC address:",String(macAdd));
       // set up the servo calibration offsets
       // PWM value used = (PWM calculated from angle) + (calibration offset for this servo) - 299
       // note that servoOffset[0] is not used
       // updated for config with HiTek HS-485-HB servos 2-23-02-12
       // Updated again on October 3, 2023
       servoOffset[ 1] = 295; // Hip leg 1, was 295 
       servoOffset[ 2] = 295; // Knee leg 1, was 290
       servoOffset[ 3] = 310; // Ankle leg 1, was 300
       servoOffset[ 4] = 304; // Hip leg 2, was 304
       servoOffset[ 5] = 295; // Knee leg 2, was 295
       servoOffset[ 6] = 310; // Ankle leg 2, was 310
       servoOffset[ 7] = 319; // Hip leg 3, was 316
       servoOffset[ 8] = 285; // Knee leg 3, was 285
       servoOffset[ 9] = 307; // Ankle leg 3, was 307
       servoOffset[10] = 297; // Hip leg 4, was 315
       servoOffset[11] = 277; // Knee leg 4, was 297 
       servoOffset[12] = 278; // Ankle leg 4, was 298
       servoOffset[13] = 289; // Hip leg 5, was 310
       servoOffset[14] = 288; // Knee leg 5, was 306
       servoOffset[15] = 276; // Ankle leg 5, was 292
       servoOffset[16] = 280; // Hip leg 6, was 298
       servoOffset[17] = 300; // Knee leg 6, was 315
       servoOffset[18] = 294; // Ankle leg 6, was 293

    }
    
    // handle the case where Andrew's using newest ESP32 in new robot,
    //   and the case where CPU from old robot is swapped into new robot
    // if you're running this code, it must be a release 2 robot supporting soft servo offsets
    else if(macAdd == "94:B9:7E:5F:4A:40" || macAdd == "94:B9:7E:5F:52:DC")  // Andrew's MAC addresses
    {
       //sp1l("  Doing bot-specific setup for one of Andrew's MAC addresses");
       traceMs("Config setup for Andrew's MAC address:",macAdd);
       // set up the servo calibration offsets
       // PWM value used = (PWM calculated from angle) + (calibration offset for this servo)
       // note that servoOffset[0] is not used. Updated Jan 31, 2023.
       // Updated October 3, 2023
       servoOffset[ 1] = 299; // Hip leg 1, was 293
       servoOffset[ 2] = 283; // Knee leg 1, was  
       servoOffset[ 3] = 294; 
       servoOffset[ 4] = 301; // Hip leg 2, was 300
       servoOffset[ 5] = 285; // Knee leg 2, was 279
       servoOffset[ 6] = 275;
       servoOffset[ 7] = 290; // Hip leg 3, was 290
       servoOffset[ 8] = 275;
       servoOffset[ 9] = 275;
       servoOffset[10] = 275; // Hip leg 4, was 290
       servoOffset[11] = 302;
       servoOffset[12] = 306;
       servoOffset[13] = 294; // Hip leg 5, was 306
       servoOffset[14] = 299;
       servoOffset[15] = 295;
       servoOffset[16] = 286; // Hip leg 6, was 286
       servoOffset[17] = 312;
       servoOffset[18] = 316;
    }
    else    // neither MAC address matched
    {
       nl;
       sp2l("<setupPerBotConfig> Unrecognized MAC address. Per Bot configuration was bypassed", macAdd);
       traceEs("Unrecognized MAC, bypassing per bot setup: ",macAdd);
    }
 } // void setupPerBotConfig()

/** 
 * @brief Show the environment details of this application on console.
 * ============================================================================*/
void showCfgDetails()
{
    #undef localRNum     // trace prep, avoiding localRNum redefinition warnings
   #define localRNum 5
   // some test traces, one for all trace types
 
   traceH(" Robot Configuration Report");
   traceH(" ==========================");
   // next line is a call to aaChip library which can't support tracing
   cfgToConsole(); // Display core0 information on the console.
   if(networkConnected == true)
   {
      Log.verboseln("<showCfgDetails> Network connection status = TRUE");
      // next line is a call to aaNetwork library which can't support tracing
      network.cfgToConsole(); // Display network information on the console.
      if(mqttBrokerConnected == true)
      {
         traceH("Broker is connected, info follows:");
         Log.verboseln("<showCfgDetails> MQTT broker connection status = TRUE");
         Log.verbose("<showCfgDetails> MQTT broker IP address = ");
         Log.verboseln(brokerIP);
      } // if
      else
      {
         Log.verboseln("<showCfgDetails> MQTT broker connection status = FALSE");
      } // else
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> Network connection status = FALSE");
   } // else
   if(oledConnected == true)
   {
      Log.verboseln("<showCfgDetails> OLED connection status = TRUE.");
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> OLED connection status = FALSE.");
   } // else
   if(motorController1Connected == true)
   {
      Log.verboseln("<showCfgDetails> Left servo driver connection status = TRUE.");
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> Left servo driver connection status = FALSE.");
   } // else
   if(motorController2Connected == true)
   {
      Log.verboseln("<showCfgDetails> Right servo driver connection status = TRUE.");
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> Right servo driver connection status = FALSE.");
   } // else
} //showCfgDetails()

void cfgToConsole()  // migrated from /lib/aaChip-1.0.1
{
   Serial.println("<cfgToConsole> APP_CPU (core 1) settings:");  
   Serial.print("<cfgToConsole> ... Chip revision = "); Serial.println(ESP.getChipRevision()); 
   Serial.print("<cfgToConsole> ... SDK version = "); Serial.println(ESP.getSdkVersion()); 
   Serial.print("<cfgToConsole> ... Sketch size = "); Serial.print(ESP.getSketchSize()); Serial.println(" bytes");  
   Serial.print("<cfgToConsole> ... Free heap = "); Serial.print(ESP.getFreeHeap()); Serial.println(" bytes"); 
   Serial.print("<cfgToConsole> ... Serial baud rate = "); Serial.print(Serial.baudRate()); Serial.println(" Hz");
   Serial.print("<cfgToConsole> ... Arduino core = "); Serial.println(xPortGetCoreID());
   Serial.print("<cfgToConsole> ... Arduino core clock frequency = "); Serial.print(getCpuFrequencyMhz()); Serial.println(" MHz"); 
}

/** 
 * @brief Show the environment details of this application on OLED.
 * @details Uses menu system to show different information as needed.
 * ============================================================================*/
void displayCfgDetails(int8_t menuToShow)
{
   switch (menuToShow) 
   {
      case 2:
         Log.verboseln("<displayCfgDetails> Display second menu.");
         break;
      case 3:
         Log.verboseln("<displayCfgDetails> Display third menu.");
         break;
      default:
         Log.verboseln("<displayCfgDetails> Display first menu.");
         break;
   } // switch
} // displayCfgDetails()

/** 
 * @brief Check to see how the boot up process went.
 * ============================================================================*/
void checkBoot()
{
   #undef localRNum
   #define localRNum 10
   traceL("entry");
   Log.traceln("<checkBoot> Checking boot status flags."); 
   if(networkConnected == true && mqttBrokerConnected == true && oledConnected == true && mobilityStatus == true)
   {
      traceL("incomplete startup");
      Log.verboseln("<checkBoot> Bootup was normal. Set RGB LED to normal colour."); 
      setStdRgbColour(GREEN); // Indicates that bootup was normal.
   } // if
   else
   {
      traceL("complete startup");
      Log.verboseln("<checkBoot> Bootup had an issue. Set RGB LED to warning colour."); 
      setStdRgbColour(YELLOW); // Indicates that there was a bootup issue.
   } // else
} // checkBoot

void IRAM_ATTR onTimer() { digitalWrite(OnboardLED, !digitalRead(OnboardLED));}   // very simple ISR complements LED

void setupLEDISR()
{
   LED_timer = timerBegin(3,40, true);
   timerAttachInterrupt(LED_timer, &onTimer, true);
   timerAlarmWrite(LED_timer, 1000000, true);
   timerAlarmEnable(LED_timer); //just enable

}

void setupTracing()
{
// following is extracted from global_variables.cpp for handy reference in table building
//===========================
/*
// symbols to help in constructing trace messages

// first, the message type
const int t$H = 1;      // high level info/status
const int t$M = 2;      // medium level info/status
const int t$L = 4;      // low level info/status
const int t$W = 8;      // warning
const int t$E = 16;     // error

// define the table that defines, on the fly, how individual traces are to be processed and routed

// index 1 in this table is a special case, and defines the global trace enabling, using bit definitions below
   const int t$global = 1;        // symbol for this special index
// index 2 is a special case that defines where tracemessages are sent, using these bit definitions
   const int t$routing = 2;       // symbol for this special index

   const int t$SM = 1;    // if this bit is set, trace messages will go to console = serial monitor
   const int t$MQ = 2;     // if this bit is set, trace messages will go to MQTT topic ??? TBD
                          // it is OK to set both these bits, which will cause messages to go to both destinations

// The remaining indices, starting at 3 define trace handling for a particular function.
// The index number appears in the trace command that triggers the trace message
// There is also a sub index, so you can have multiple traces in a function that can be differentiated
// Used indices are documented in the comments next to the table entry initialization
// This table initialization is done at compile time, and is the default tracing configuration
// However, an MQTT command can be used to overwrite the numbers in the table, and to display it.
//
// The message type bits like t$H defined above are used to set global enables, and per-routine enables
// (using simple numbers for routine IDs now, but can COBOLize that later)

   #define maxTraceCount 100       // support for up to 100 routines that each have multiple trace messages

   int $traceTab[maxTraceCount];    // reserve space for the trace control table
  
    // compiler won't let me populate the table here, so I'm going to try in configDetails.cpp
*/

  // define a typing shortcut for common case where everything's enabled except low level details
  #define t$allButL t$H + t$M + t$W + t$E
  #define t$all t$H + t$M + t$L + t$W + t$E

  // following table entries specify trace enables for individua routines
  // first 2 table entries are special purpose ones
  //$traceTab[t$global] = t$H + t$M + t$E + t$W ;  // error, warning, high, and medium messages are globally enabled
  $traceTab[t$global] = t$all ;  // error, warning, high, and medium messages are globally enabled
  $traceTab[t$routing] = t$SM ;     // direct traces messages to serial monitor

  // special trace table entries 1 (t$global) and 2 (t$routing) are defined above
  $traceTab[3] = t$M +t$W +t$E ;       // routine 3 = setupFlows() in flows.cpp
  $traceTab[4] = t$E + t$W;            // routine 4 = mapDegToPWM in flows.cpp
  $traceTab[5] = t$E + t$W;            // routine 4 = showCfgDetails in configDetails.cpp
  $traceTab[6] = t$M ;                 // routine 6 = processCmd in mqttBroker.cpp
  $traceTab[7] = t$allButL;            // routine 7 = setup in main.cpp
  $traceTab[8] = t$allButL;            // routine 8 = aaNetwork::_lookForAP()
  // above entry doesn't work because it's in a library, independently compiled without our tracing
  $traceTab[9] = t$all;                // routine 9 = configDetails::setupPerBotConfig
  $traceTab[10]= t$all;                // routine 10 = configDetails::checkBoot
  $traceTab[11]= t$all;                // routine 11 = rgbLed::createPredefinedColours
  $traceTab[12]= t$all;                // routine 12 = deviceSupport::setupPwmdriver
  $traceTab[13]= t$all;                // routine 13 = flows.cpp::do_flow
  $traceTab[14]= t$all;                // routine 14 = CPU usage in main.cpp  oneSec()

  return ;
} // void setupTracing()

// put setupDimensions() here, from flows
void setupDimensions()     // set up variables that depend on physical dimensions of robot
{

} // void setupDimensions()

#endif // End of precompiler protected code block