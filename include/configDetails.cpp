/*******************************************************************************
 * @file configDetails.cpp  
 * @brief setup & display config parameters and boot status
 *******************************************************************************/
#ifndef showCfgDetails_cpp // Start of precompiler check to avoid dupicate inclusion of this code block.

#define showCfgDetails_cpp // Precompiler macro used for precompiler check.

/**
 * @brief Do configuration set up that is unique to each robot, including:
 * - define servo calibration offset values
 * called from setup() in main.cpp
 * tried to put this routine in hexBot/lib/aaWeb-1.0.0/lib/aaNetwork-main/src/aaNetwork.cpp
 *    but couldn't get it to work, due to object structure issues I suspect
 =============================================================================*/
 void setupPerBotConfig()     // do the robot-specific setup
 {
    String macAdd = WiFi.macAddress(); // Get MAC address as String
    sp2("<setupPerBotConfig> MAC Address: ", macAdd);  // see if we can read this variable

    // handle the case where Doug's using newest ESP32 in new robot,
    //   and the case where CPU from old robot is swapped into new robot
    // if you're running this code, it must be a release 2 robot supporting soft servo offsets
    if(macAdd == "3C:61:05:4A:DD:98" || macAdd == "94:B9:7E:5F:48:B8")  // Doug's 2 ESP32 MAC addresses
    {
       sp1l("  Doing bot-specific setup for one of Doug's MAC addresses");
       // set up the servo calibration offsets
       // PWM value used = (PWM calculated from angle) + (calibration offset for this servo) - 299
       // note that servoOffset[0] is not used
       servoOffset[ 1] = 299;
       servoOffset[ 2] = 299;
       servoOffset[ 3] = 299;
       servoOffset[ 4] = 299;
       servoOffset[ 5] = 299;
       servoOffset[ 6] = 299;
       servoOffset[ 7] = 299;
       servoOffset[ 8] = 299;
       servoOffset[ 9] = 299;
       servoOffset[10] = 299;
       servoOffset[11] = 299;
       servoOffset[12] = 299;
       servoOffset[13] = 299;
       servoOffset[14] = 299;
       servoOffset[15] = 299;
       servoOffset[16] = 299;
       servoOffset[17] = 299;
       servoOffset[18] = 299;

    }
    
    // handle the case where Andrew's using newest ESP32 in new robot,
    //   and the case where CPU from old robot is swapped into new robot
    // if you're running this code, it must be a release 2 robot supporting soft servo offsets
    else if(macAdd == "94:B9:7E:5F:4A:40" || macAdd == "94:B9:7E:5F:52:DC")  // Andrew's MAC addresses
    {
       sp1l("  Doing bot-specific setup for one of Andrew's MAC addresses");
       // set up the servo calibration offsets
       // PWM value used = (PWM calculated from angle) + (calibration offset for this servo)
       // note that servoOffset[0] is not used. Updated Jan 31, 2023.
       servoOffset[ 1] = 293;
       servoOffset[ 2] = 283;
       servoOffset[ 3] = 294;
       servoOffset[ 4] = 300;
       servoOffset[ 5] = 279;
       servoOffset[ 6] = 275;
       servoOffset[ 7] = 290;
       servoOffset[ 8] = 275;
       servoOffset[ 9] = 275;
       servoOffset[10] = 290;
       servoOffset[11] = 302;
       servoOffset[12] = 306;
       servoOffset[13] = 306;
       servoOffset[14] = 299;
       servoOffset[15] = 295;
       servoOffset[16] = 286;
       servoOffset[17] = 312;
       servoOffset[18] = 316;
    }
    else    // neither MAC address matched
    {
       nl;
       sp2l("<setupPerBotConfig> Unrecognized MAC address. Per Bot configuration was bypassed", macAdd);
    }
 } // void setupPerBotConfig()

/** 
 * @brief Show the environment details of this application on console.
 * ============================================================================*/
void showCfgDetails()
{
   // tracing preparation for this routine
   #define localRName "showCfgDetails"
   #define locanRNum 5
   // some test traces, one for all trace types
   trace(t$H,"sample High level info",0) ;
   trace(t$M,"sample Medium level info",0) ;
   trace(t$L,"sample Low level info",0) ;
   trace(t$W,"sample Warning message",0) ;
   trace(t$E,"sample Error message",0) ;

   Log.verboseln("<showCfgDetails> Robot Configuration Report");
   Log.verboseln("<showCfgDetails> ==========================");
   appCpu.cfgToConsole(); // Display core0 information on the console.
   if(networkConnected == true)
   {
      Log.verboseln("<showCfgDetails> Network connection status = TRUE");
      network.cfgToConsole(); // Display network information on the console.
      if(mqttBrokerConnected == true)
      {
         Log.verboseln("<showCfgDetails> MQTT broker connection status = TRUE");
         Log.verbose("<showCfgDetails> MQTT broker IP address = ");
         Log.verboseln(getMqttBrokerIP());
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
   Log.traceln("<checkBoot> Checking boot status flags."); 
   if(networkConnected == true && mqttBrokerConnected == true && oledConnected == true && mobilityStatus == true)
   {
      Log.verboseln("<checkBoot> Bootup was normal. Set RGB LED to normal colour."); 
      setStdRgbColour(GREEN); // Indicates that bootup was normal.
   } // if
   else
   {
      Log.verboseln("<checkBoot> Bootup had an issue. Set RGB LED to warning colour."); 
      setStdRgbColour(YELLOW); // Indicates that there was a bootup issue.
   } // else
} // checkBoot

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
  // first 2 table entries are special purpose ones
  $traceTab[t$global] = t$H + t$M + t$E + t$W ;  // error and warning messages are globally enabled
  $traceTab[t$routing] = t$SM ;     // direct traces messages to serial monitor

  // following table entries specified trace enables for individua routines
  //$traceTab[3] = t$H + t$M + t$L + t$W + t$E ;  // routine 3 = setupFlows() has all traces enabled
  $traceTab[3] = t$L + t$W + t$E ;  // routine 3 = setupFlows() in flows.cpp
  $traceTab[4] = t$E + t$W;         // routine 4 = mapDegToPWM in flows.cpp
  $traceTab[5] = t$E + t$W;         // routine 4 = showCfgDetails in configDetails.cpp
  $traceTab[6] = t$M ;              // routine 6 = processCmd in mqttBroker.cpp
  
} // void setupTracing()

#endif // End of precompiler protected code block