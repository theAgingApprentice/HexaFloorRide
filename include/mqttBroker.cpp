/*******************************************************************************
 * @file mqttBroker.cpp  
 * @brief File containing all MQTT functions.
 *******************************************************************************/
#ifndef connectToMqttBroker_h // Start of precompiler check to avoid dupicate inclusion of this code block.
#define connectToMqttBroker_h // Precompiler macro used for precompiler check.

#include <main.h>    // Header file for all header files plus global_variables.cpp
// main.h is the master header file. Other .cpp files should include main.h as the only included header file

// TODO #7 : A pingable but non MQTT IP address crash loops code.
/** 
 * @brief Establish connect to the the MQTT broker.
 * @details Retrieve the MQTT broker IP address from Flash memory and ping that 
 *          address to see if there is a responsive device on the network. If there 
 *          is then publish a health message noting that end-to-end network services 
 *          are working. Note that upon connecting to the broker the MQTT library 
 *          automatically subscribes to the <unique name>/commands topic.  
 * =================================================================================*/
bool connectToMqttBroker(aaNetwork &network)
{
   network.getUniqueName(uniqueNamePtr); // Puts unique name value into uniqueName[]
   Log.noticeln("<connectToMqttBroker> Hexbot unique network name = %s.", uniqueName);
   Log.noticeln("<connectToMqttBroker> Health topic = %s.", HEALTH_MQTT_TOPIC);
   strcpy(healthTopicTree, uniqueName);
   strcat(healthTopicTree, HEALTH_MQTT_TOPIC);
   Log.noticeln("<connectToMqttBroker> Full health topic tree = %s (length = %d).", healthTopicTree, strlen(healthTopicTree));
   Log.noticeln("<connectToMqttBroker> Help topic = %s.", HELP_MQTT_TOPIC);
   strcpy(helpTopicTree, uniqueName);
   strcat(helpTopicTree, HELP_MQTT_TOPIC);
   Log.noticeln("<connectToMqttBroker> Full help topic tree = %s (length = %d).", helpTopicTree, strlen(helpTopicTree));
   brokerIP = flashReadBrokerIP(); // Retrieve MQTT broker IP address from NV-RAM.
   Log.noticeln("<connectToMqttBroker> MQTT broker IP believed to be %p.", brokerIP);
   bool tmpPingResult = network.pingIP(brokerIP, 5);
   String tmpResult[2];
   tmpResult[0] = "Not found - invalid address";
   tmpResult[1] = "Found - valid address";
   Log.noticeln("<connectToMqttBroker> Ping of broker at %p resulted in %T.", brokerIP, tmpPingResult);
   if (tmpPingResult == true)
   {
      mqtt.connect(brokerIP, uniqueName);
      bool x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(healthTopicTree, "End-to-end network services estabished");
         delay(1);
      } //while
   }    //if
   else
   {
      Log.errorln("<connectToMqttBroker> Cannot reach MQTT broker.");
      return false;
   } //else
   return true;
} //connectToMqttBroker()

/**
 * @brief Check is a string variable is a valid number.
 * @return true if it is a valid number and false if it is not.
 * =================================================================================*/
boolean isValidNumber(String str)
{
   for (byte i = 0; i < str.length(); i++)
   {
      if (!isDigit(str.charAt(i)))
      {
         return false;
      } //if
   }    // for
   return true;
} // isValidNumber()

/**
 * @brief Converts a string variable to a unit32_t variable.
 * @param var is the string to be converted.
 * @return The convereted value. 
 * =================================================================================*/
uint32_t convertStrToUint32_t(String var)
{
   unsigned long numToConvert = strtoul(var.c_str(), NULL, 10);
   return (uint32_t)numToConvert;
} // convertStrToUint32_t()

/**
 * @brief Converts a string variable to a uint8_t variable.
 * @param var is the string to be converted.
 * @return The convereted value. 
 * =================================================================================*/
uint8_t convertStrToUint8_t(String var)
{
   unsigned long numToConvert = strtoul(var.c_str(), NULL, 10);
   return (uint8_t)numToConvert;
} // convertStrToUint8_t()

/**
 * @brief Converts a string variable to a int8_t variable.
 * @param var is the string to be converted.
 * @return The convereted value. 
 * =================================================================================*/
int8_t convertStrToInt8_t(String var)
{
   unsigned long numToConvert = strtoul(var.c_str(), NULL, 10);
   return (int8_t)numToConvert;
} // convertStrToInt8_t()

/**
 * @brief Check if command arguments are valid numbers.
 * @param numArgumentsRequired how many numbers are expected.
 * @param argN number of arguments passed by command.
 * @param *arg pointer to array containing the arguments to be checked.
 * @return true if there are the right number of arguments and they are all numbers,
 * false if the number of arguments is wrong or if one of the arguments is not a 
 * number. 
 * =================================================================================*/
bool checkNumArg(int8_t numArgumentsRequired, int argN, String *arg)
{
   if (argN == numArgumentsRequired) // Right number of arguments?
   {
      Log.verboseln("<checkNumArg> Correct number of arguments received.");
      for (int8_t i = 1; i <= numArgumentsRequired; i++) // Are the three arguments valid numbers.
      {
         if (isValidNumber(arg[i])) // If current argument is a valid number.
         {
            Log.verboseln("<checkNumArg> arg[%d] = %s and is a valid number.", i, arg[i]);
         }    // if
         else //  If current argument is not a valid number.
         {
            Log.warningln("<checkNumArg> Ignoring command. arg[%d] = %s and is not a valid number.", i, arg[i]);
            return false;
         } // if
      }    // for
   }       // if
   else    // Wrong number of arguments
   {
      Log.warningln("<checkNumArg> Ignoring command. SET_CUST_RGB_CLR requires %d arguments but received %i.", numArgumentsRequired, argN);
      return false;
   } // else
   return true;
} // checkNumArg()

/**
 * @brief Process the incoming command.
 * @param payload Command received from MQTT broker.
 * @return True is the command is known. False if the cmmand is unknown.
 * =================================================================================*/
bool processCmd(String payload)
{
   // prepare for traces
   #undef localRNum
   #define localRNum 6

   aaFormat format;                       // instantiate the format object?
   String ucPayload = format.stringToUpper(payload);
   const int8_t maxArg = 30;                      // Allow 1 cmd and up to 29 args in an MQTT message.
   String arg[maxArg];                            // arg[0] = cmd, arg[1] = 1st argument, arg[2] = second ...
   int argN = 0;                                  // argument number that we're working on
   int argStart = 0;                              // character number where current argument starts
   int argEnd = ucPayload.indexOf(",", argStart); // position of comma at end of cmd
   // Parse comma delimited message into array elements
   while (argEnd >= 0) // .indexOf returns -1 if no string found
   {
      arg[argN] = ucPayload.substring(argStart, argEnd); // extract the current argument
      argN++;                                            // advance the argument counter
      argStart = argEnd + 1;                             // next arg starts after previous arg's delimiting comma
      argEnd = ucPayload.indexOf(",", argStart);         // find next arg's delimiting comma
   }                                                     // while
   arg[argN] = ucPayload.substring(argStart, argEnd);    // last argument has no comma
                                                         // delimiter. argN ends up as a
                                                         // count of the number of
                                                         // arguments, excluding the command
   String cmd = arg[0];                                  // first comma separated value in payload is the command

   // debug
   traceHs("command received:", cmd);

   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: TEST
   
   // TEST command.
   if (cmd == "TEST")
   {
      //Log.noticeln("<processCmd> Received test command.");
      traceH("TEST command execution: code compiled on: " + String(__DATE__) + ", " + String(__TIME__) );
      return true;
   } // if
   
   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: HELP
   
   // HELP command.
   if (cmd == "HELP")
   {
      Log.noticeln("<processCmd> Received help command.");
      sp2sl("helpTopicTree= ",helpTopicTree);
      bool x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "TEST - Issues test message to terminal.");
         delay(1);
      } //while
      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "HELP - Lists valid commands to help topic tree.");
         delay(1);
      } //while
      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "SET_CUST_RGB_CLR,red,green,blue - Custom colour for RGB LED. (arg values 0-256).");
         delay(1);
      } //while
      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "SET_STD_RGB_CLR,colour - Standard colour for RGB LED. (arg values 0-8).");
         delay(1);
      } //while
      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "ROTATE_OLED,orientation - Orient text on OLED. (arg values 0-3).");
         delay(1);
      } //while

      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "FLOW,time,opcode,10,0,0,0, 6 * (x, y, z) // several format variations" );
         delay(1);
      } //while

       x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "   - see ...hexbot/docs/MQTT scripts/creating-flows.odt, section6 for details" );
         delay(1);
      } //while


      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "NEW_FLOW  //start of new flow sequence. No args");
         delay(1);
      } //while

      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "DO_FLOW,toe-action, frame-interval // end of a flow sequence");
         delay(1);
      } //while

      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "SERVO_TEST_PWM,<side (0-1)>,<port(0-15)>,<PWM (100-500?)> // set single servo PWM");
         delay(1);
      } //while

      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "SSD, degrees, servo#, (optional end of range servo#) // set servo degrees");
         delay(1);
      } //while

      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "SSP, PWM, servo#, (optional end of range servo#) // set servo PWM");
         delay(1);
      } //while


      x = false;
      while (x == false)
      {
         x = mqtt.publishMQTT(helpTopicTree, "SSO, servo#, (optional end of range servo#) // set servo off");
         delay(1);
      } //while



      Log.noticeln("<processCmd> List of valid MQTT commands sent to MQTT broker.");
      return true;
   } // if
 
   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: SET_CUST_RGB_CLR
   
   // SET_CUST_RGB_CLR command.
   if (cmd == "SET_CUST_RGB_CLR")
   {
      const int8_t numArgumentsRequired = 3; // How many arguments expected?
      if (checkNumArg(numArgumentsRequired, argN, &arg[0]))
      {
         uint32_t red = convertStrToUint32_t(arg[1]);
         uint32_t green = convertStrToUint32_t(arg[2]);
         uint32_t blue = convertStrToUint32_t(arg[3]);
         Log.noticeln("<processCmd> Set RGB LED values R = %u, G = %u, B = %u.", red, green, blue);
         setCustRgbColour(red, green, blue);
         return true;
      } // if
      else
      {
         return false;
      } // else
   }    // if
   
   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: SET_STD_RGB_CLR
   
   // SET_STD_RGB_CLR command.
    if (cmd == "SET_STD_RGB_CLR")
   {
      const int8_t numArgumentsRequired = 1; // How many arguments expected?
      if (checkNumArg(numArgumentsRequired, argN, &arg[0]))
      {
         uint8_t colour = convertStrToUint8_t(arg[1]);
         setStdRgbColour(colour);
         return true;
      } // if
      else
      {
         return false;
      } // else
   }    // if
   
   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: ROTATE_OLED
   
   // ROTATE_OLED command.
   if (cmd == "ROTATE_OLED")
   {
      const int8_t numArgumentsRequired = 1; // How many arguments expected?
      if (checkNumArg(numArgumentsRequired, argN, &arg[0]))
      {
         int8_t tmp = convertStrToInt8_t(arg[1]);
         rotateDisplay(tmp);
         displaySplashScreen("");
         return true;
      } // if
      else
      {
         return false;
      } // else
   }    // if cmd = "ROTATE_OLED"

      // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: FLOW

   // FLOW command for storing next position in a smooth motion flow between multiple positions
   // the flow command builds arrays describing the desired movement
   // the flow_go command starts the movement and controls repetitions, resets, etc

   if (cmd == "FLOW" || cmd == "FL")

   // ==== need a lockout so flow can't be redefined while executing

   // see flows.h for definition of flow arrays and parameters
   //
   // flow command format
   // FL,msec,operation,lShape1,lS2,lS3,lS4, L1X,L1Y,L1Z, L2X,L2Y,L2Z, L3X,L3Y,L3Z, L4X,L4Y,L4Z, L5X,L5Y,L5Z, L6X,L6Y,L6Z
     // example: // move all toes to to 1 cm above home position, i.e. slightly squatting from neutral
   // FL,1000,2,10,0,0,0, 0,0,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1
   // action: - save given data into next position in the flow arrays, to be part of the motion initiated by flow_go command
   //
   // some specialized commands (cycleStart, cycleEnd, executeCycle, newHomexxx, toeSafetyX...) don't use the last 18 numbers, 
   //   and are in the form
   // FL,msec,operation, param1-int, param2-float, param3-float, param4-float
   // example: FL, 0, 9, 7
   // action: next flow command is defined as he start of cycle number 7
   // the short flow commands are the ones with f_operation codes that are:
   // >= fo_newHomeHere  and  < 100
   {
   //   Serial.println("start processing flow command");

      // copy position description from MQTT flow command to next flow row
      // f_count starts at 0, which we use to store first position in row 0
      f_msecs[f_count] = arg[1].toInt();     // arg[0] is the FL command, arg[1] is msec time ...
      // arg[2] is the operation code. If it's alphabetic, convert to equivalent number
      if( !isValidNumber(arg[2]) )   // if it's a number, leave it, but if not, do a lookup translation
      {  // wrap dashes around the given operation mnemonic, in upper case, to ensure lookup works
         String op = arg[2];     // need a shorter non indexed variable to work with
         if(op.substring(0,1) == " ") { op = op.substring(1,99);} // if there's a leading space, remove it
         if(op.substring(0,1) == " ") { op = op.substring(1,99);} // if there's another leading space, remove it
         if(op.substring(0,1) == " ") { op = op.substring(1,99);} // if there's another leading space, remove it
         String opCode = "-" + format.stringToUpper(op) + "-"; // standardize non numeric code to upper case
         // sp2sl("opCode=",opCode);
         String fcmds="-MGC--MLC--MGRH-MLRH-MGRL-MLRL-NHGC-NHLC-NHH--NHR--CPF--MCS--MCE--DC---TSX--TSY--TSZ--TSR--DM---BOM--EOM--SM---";
         int cmdNum = (fcmds.indexOf(opCode) +5) /5; // look up command in above string, and convert to command number
         arg[2] = f_flowOps[cmdNum];  //look up associated numeric operations code, and substitute it in for mnemonic
      }
      int tmpOp = arg[2].toInt();   // temporary easy to reference scalar
      f_operation[f_count] = tmpOp; // code for operation in this row of the flow, eg fo_moveAbs

      // argument after op code is usually the type of line. initially, always fl_straight
      // except in the DM or SM (Do or Save Macro) opcode, where this arg is an alpha macro name.
      // In this case, we decode the name, and overwrite it into arg[3]

      if(tmpOp == fo_doMacro || tmpOp == fo_saveMacro)      // are we doing SM or DM?
      {
         char char1 = arg[3].charAt(0);
         if(isAlpha(char1) == true)
            {
               // processCmd has already converted command buffer to upper case
               int charnum = char1 - 64;  // convert to an int between 1 - 26 for macro name
               arg[3] = charnum;          // arg[3] is a float, but we'll ignore that
            }
         else
            {
               arg[3] = 26;      // non-alpha gets forced to "Z"
            }
      }  // if(tmpOp == fo_doMacro ...)

      if(argN>2) 
      {     // argument after op code is usually the type of line. initially, always fl_straight
            // except or the DM (Do Macro) opcode, where this arg is an alpha argument name

            f_lShape1[f_count] = arg[3].toInt();  // copy (possibly modified) arg[3] into the appropriate sequence row array
                                                  // its nominally the type of line. initially, always fl_straight
      }  // if(argN>2) 
      if(argN>3) {f_lShape2[f_count] = arg[4].toFloat();} // parameter to define line when it's a parabola, ellipse,...
                                                          // for doMacro, arg[4] is the repeat count
      if(argN>4) {f_lShape3[f_count] = arg[5].toFloat();} // more line parameters
      if(argN>5) {f_lShape4[f_count] = arg[6].toFloat();}

      if(argN > 6)   // if more comma separated values were given, move 18 into the flow row
      {
         f_legX[f_count][1] = arg[7].toFloat(); // X coordinate or delta value for leg 1
         f_legY[f_count][1] = arg[8].toFloat(); // Y coordinate or delta value for leg 1
         f_legZ[f_count][1] = arg[9].toFloat(); // Z coordinate or delta value for leg 1

         f_legX[f_count][2] = arg[10].toFloat(); // X c}oordinate or delta value for leg 2
         f_legY[f_count][2] = arg[11].toFloat(); // Y coordinate or delta value for leg 2
         f_legZ[f_count][2] = arg[12].toFloat(); // Z coordinate or delta value for leg 2

         f_legX[f_count][3] = arg[13].toFloat(); // X coordinate or delta value for leg 3
         f_legY[f_count][3] = arg[14].toFloat(); // Y coordinate or delta value for leg 3
         f_legZ[f_count][3] = arg[15].toFloat(); // Z coordinate or delta value for leg 3

         f_legX[f_count][4] = arg[16].toFloat(); // X coordinate or delta value for leg 4
         f_legY[f_count][4] = arg[17].toFloat(); // Y coordinate or delta value for leg 4
         f_legZ[f_count][4] = arg[18].toFloat(); // Z coordinate or delta value for leg 4

         f_legX[f_count][5] = arg[19].toFloat(); // X coordinate or delta value for leg 5
         f_legY[f_count][5] = arg[20].toFloat(); // Y coordinate or delta value for leg 5
         f_legZ[f_count][5] = arg[21].toFloat(); // Z coordinate or delta value for leg 5

         f_legX[f_count][6] = arg[22].toFloat(); // X coordinate or delta value for leg 6
         f_legY[f_count][6] = arg[23].toFloat(); // Y coordinate or delta value for leg 6
         f_legZ[f_count][6] = arg[24].toFloat(); // Z coordinate or delta value for leg 6
      }  // if(argN > 6) 

      f_count++; // advance to next entry in flow arrays
                 // f_count now contains the number of flow rows that have been defined,
                 // i.e. row 0 up to and including row (f_count -1)
                 // it's reset by the FLOW_GO command
      return true;
   } // if cmd = flow
   
   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: NEW_FLOW
   
   // NEW_FLOW command to prepare for flow commands defining a new flow
   // the FLOW command builds arrays describing the desired movement
   // the DO_FLOW command starts the movement and controls repetitions, resets, etc
   //
   // command: NEW_FLOW
   // format: NEW_FLOW
   // action: 1 - start the flow currently defined in the flow arrays
   //         0 - reset the current flow, and empty the flow arrays
   // msecPerFrame: duration of each frame (fraction of a position) in millis. this determines f_framesPerPosn
   //               - if omitted, it defaults to f_msecPerFrameDefault, which defaults to 20 msec

   if (cmd == "NEW_FLOW" || cmd == "NF")  // prep for definition of a new flow
   {     f_flowing = false;
         f_count = f_nextSeqStart;   // where we should store sequence commands, avoiding macros
         f_lastSeqStart = f_nextSeqStart;   // remember where we started this sequence
         f_active = 0;
         return true;
   }

   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: DO_FLOW

   // command: DO_FLOW
   // format: DO_FLOW,toeMoveAction, msecPerFrame
   //
   // toeMoveAction: bit encoded actions to be taken when new toe position has been calculated. defined in flows.h
   // msecPerFrame: duration of each frame (fraction of a position) in millis. this determines f_framesPerPosn
   //               - if omitted, it defaults to f_msecPerFrameDefault, which defaults to 50 msec
   // 
   if (cmd == "DO_FLOW" || cmd == "DF")  // execute flow that has just been defined
   {     
         if (f_count == 0) // is there a flow defined to run?
         {
            Serial.println("<flow_go>: tried to run flow, but none defined");
            return false;
         }
         f_flowing = 1; // we're now executing a flow
         f_active = 0;  // starting at the 0th entry in the flow arrays

         toeMoveAction = 1;   // default value if it's not in DO_FLOW command
         if (argN >= 1)
         {
            toeMoveAction = arg[1].toInt();
         } // binary coded options for toe moves

         f_msecPerFrame = f_msecPerFrameDefault; // if not given in FG command, use the default
         if (argN >= 2)                           // if there were at least 2 numeric args to FG command
         {                                       // ... 2nd one is msecPerFrame
            f_msecPerFrame = arg[2].toInt();     // 2nd number is millis per frame
            if (f_msecPerFrame < 10 || f_msecPerFrame > 200)
            {
               f_msecPerFrame = f_msecPerFrameDefault;
            }
         }
         return true;
   } // if (cmd == "DO_FLOW" || cmd == "DF")  

   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: MLA
   
   // MoveLegAngles command - move one leg to specified angles for hip, knee and ankle servos
   // format: MLA, leg-num, hip-angle, knee-angle, ankle-angle
   //   angles are expressed in degrees. 0 at centerpoint, and +ve is forward, down, and out respectively
   // use exact same method as flow execution so testing replicates script execution

   if (cmd == "MOVELEGANGLES" || cmd == "MLA")   //MoveLegAngles command
   { // command format: ML, f_operation, leg, X, Y, Z
   
      if (argN != 4)          // did we get command pls 4 arguments?
      {                       // ignore command altogether
         sp1l("******* MoveLegAngles command didn't have 4 additional arguments");
         return false;
      }
      else // convert args to appropriate format
      {
         int argLeg = arg[1].toInt();      // first arg is leg number
         f_angH = arg[2].toFloat();   // .. then hip angle
         f_angK = arg[3].toFloat();   // then knee angle
         f_angA = arg[4].toFloat();   // then ankle angle

         goToAngles(argLeg) ;     // move servos to these angles for specified leg(s)
         return true;
      } // else..if (argN != 4) 
   
   } // else... if (cmd == "MOVELEGANGLES" || cmd == "MLA")


   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: MLC
   
   // MoveLegCoords command - move one leg to specified local X, Y and Z coordinates
   // format: MLA, leg-num, X, Y, Z
      // use exact same method as flow execution so testing replicates script execution

   if (cmd == "MOVELEGCOORDS" || cmd == "MLC")   //MoveLegAngles command
   { // command format: MLC, leg, X, Y, Z


      if (argN != 4)          // did we get command pls 4 arguments?
      {                       // ignore command altogether
         sp1l("******* MoveLegCoords command didn't have 4 additional arguments");
         return false;
      }
      else // convert args to appropriate format
      {
         int argLeg = arg[1].toInt();      // first arg is leg number
         float argX = arg[2].toFloat();   // .. then hip angle
         float argY = arg[3].toFloat();   // then knee angle
         float argZ = arg[4].toFloat();   // then ankle angle

         coordsToAngles(argX, argY, argZ); // creates f_angH, f_angK, f_angA
         // now, one servo within the leg at a time, figure the pwm value, and move the servo
         // might have to temporarily negate angles, due servos mounted opposite on either side of bot

         goToAngles(argLeg);     // move servos to these angles for specified leg(s)
         return true;
      }
   } // else... if (cmd == "MOVELEGANGLES" || cmd == "MLA")

   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: MLRH
   
   // Move Leg Relative to Home command - move one leg to specified offset from home position
   // format: MLRH, leg-num, X, Y, Z
      // use exact same method as flow execution so testing replicates script execution
      // if leg-num is 0, the command is executed for all legs

   if (cmd == "MOVELEGRELHOME" || cmd == "MLRH")   //Move Leg Relative to Home command
   { // command format: MLRH, leg, X, Y, Z

      if (argN != 4)          // did we get command pls 4 arguments?
      {                       // ignore command altogether
         sp1l("******* MoveLegCoords command didn't have 4 additional arguments");
         return false;
      }
      else // convert args from local offsets to local coordinates
      {
         int argLeg = arg[1].toInt();                     // first arg is leg number
         float argX = arg[2].toFloat() + f_staticHomeX;   // .. then offset from home in X direction
         float argY = arg[3].toFloat() + f_staticHomeY;   // .. then offset from home in Y direction
         float argZ = arg[4].toFloat() + f_staticHomeZ;   // .. then offset from home in Z direction

         coordsToAngles(argX, argY, argZ); // creates f_angH, f_angK, f_angA
         // now, one servo within the leg at a time, figure the pwm value, and move the servo
         // might have to temporarily negate angles, due servos mounted opposite on either side of bot

         goToAngles(argLeg);      // move leg, or legs, to the equivalent servo angles
         return true;
      }
   } // if (cmd == "MOVELEGRELHOME" || cmd == "MLRH") 

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: QGH

   if (cmd == "QGH")   // Quick Go to Home position, for all legs
   { // command format: QGH

      if (argN != 0)          // did we get command pls 4 arguments?
      {                       // ignore command altogether
         sp1l("******* QGH should not have arguments");
         return false;
      }
      else // convert args from local offsets to local coordinates
      {
         f_angH = 0;    // zero angle for hip servo
         f_angK = 0;    // ..and for knee
         f_angA = 0;    // ..and for ankle

         goToAngles( 0 );      // move all legs to these servo angles
         return true;
      }
   } // if (cmd == "QGH") 

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: QGS

   if (cmd == "QGS")   // Quick Go to Squat position, for all legs (previously "belly on ground")
   { // command format: QGS

      if (argN != 0)          // did we get command pls 4 arguments?
      {                       // ignore command altogether
         sp1l("******* QGS should not have arguments");
         return false;
      }
      else // convert args from local offsets to local coordinates
      {  
         goToSquat();   // also need to squat before QGU - legs up in air, & QGC
         return true;
      }
   } // if (cmd == "QGS") 

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: QGU

   if (cmd == "QGU")   // Quick Go to Squat position, for all legs (previously "belly on ground")
   { // command format: QGS

      if (argN != 0)          // did we get command pls 4 arguments?
      {                       // ignore command altogether
         sp1l("******* QGU should not have arguments");
         return false;
      }
      else // convert args from local offsets to local coordinates
      {  
         goToSquat();   // also need to squat before QGU - legs up in air, & QGC

         // now safe to raise the legs overhead (only using angles, not coords)
         f_angH = 0;    // hip facing straight out
         f_angK = -80;  // knee pointing up
         f_angA = 80;   // ankle pointing up
         goToAngles( 0 );      // move all legs to these servo angles
         
         return true;
      }
   } // if (cmd == "QGU") 

////-----------------------------------------------------
   // declare variables for upcoming servo test commands
   int sr_deg, sr_pwm, sr_srv, sr_srv2, sr_side, sr_port;
   bool sr_OK;
   String sr_cause;

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: STP

// Servo_Test_PWM command to allow testing of servos using specific PWM values
//  Servo_Test_PWM <side (0-1)> <port(0-15)> <PWM (100-500?)>
//  Shortform: STP
//  Example: STP 0, 15, 300
   if(cmd == "SERVO_TEST_PWM" || cmd == "STP")
   {
      sr_cause = ""; // initially, no error message
      sr_OK = true;  // and arguments were OK
      if(argN != 3)  // need 3 args: side, port & PWM
      {
         sr_OK = false;
         sr_cause += " need 3 arguments";
      } // if(argN != 3) 
      else //  if(argN != 3)
      {
         sr_side = arg[1].toInt();  // translate 3 argument strings to integers
         sr_port = arg[2].toInt();
         sr_pwm  = arg[3].toInt();
         // range check the arguments
         if(sr_side < 0 || sr_side > 1)  {sr_OK = false; sr_cause += " bad arg 1: side value.";}
         if(sr_port < 0 || sr_port > 15) {sr_OK = false; sr_cause += " bad arg 2: port value.";}
         if(sr_pwm < 100 || sr_pwm > 500) {sr_OK = false; sr_cause += " bad arg 3: PWM value.";}

      } // else if(argN != 3)
      if(sr_OK == false) // if we've run into a problem with the command arguments, report & abort
      {
         sp2sl("<STP command> argument error:",sr_cause);
      } // if(sr_OK == false)
      else // if(sr_OK == false)
      {
         // all went well with argument processing, lets move a servo
         pwmDriver[sr_side].setPWM(sr_port, pwmClkStart, sr_pwm);
         // and brag about what we've done on the console
         sp4sl("<STP command> successful servo movement: side, port, PWM: ", sr_side, sr_port, sr_pwm);
      } // else // if(sr_OK == false)
      return sr_OK;
   } // if(cmd == "SERVO_TEST_PWM" || cmd == "STP")

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: SSO

// SSD command = Set  Servo(s) to angle, expressed in degrees, zero is center of range
   if(cmd == "SSD")     // set servo(s) to degrees
   // command format: SSD, degrees, servo#, (optional end of range servo#)
   //    -90 <= degrees <= 90
   //    1 <= servo# <= 18
   //    servo #1 is hip for leg 1, the front right one

   {  sr_OK = true;           // optimistically assume no problems with the command
      if(argN!=2 && argN!=3) {sr_OK = false;sr_cause="bad arg count";}         // wrong number of args for command
      else
      {  sr_deg = arg[1].toInt();      // first arg is angle for servo
         if(sr_deg < -90 || sr_deg > 90 ) {sr_OK = false;sr_cause="bad degree value";}
         else
         {  sr_srv = arg[2].toInt();   // second arg is servo #
            sr_srv2 = sr_srv;          // assume no 3rd arg, and its a one servo range
            if(sr_srv < 1 || sr_srv > 18) {sr_OK = false;sr_cause="bad first servo #";}
            else
            {  if(argN == 3) {sr_srv2 = arg[3].toInt();}    // there was a second servo#
               if(sr_srv2 < sr_srv || sr_srv2 > 18 ) {sr_OK = false;sr_cause="bad second servo #";}
               else
               {  for(int srv=sr_srv2; srv>=sr_srv; srv--)
                  // now set the servo "srv" to an angle of sr_deg degrees
                  //use integer division ("/") and modulus ("%") to directly calculate driver#, and pin#
                  {
                  pwmDriver[(srv-1) / 9].setPWM((srv-1) % 9, pwmClkStart, mapDegToPWM(sr_deg, srv)); 
                  sp2s("sdd) argN, srv,srv2,sr_deg, index= ",argN);sp;sp2s(sr_srv,sr_srv2);sp;sp2sl(sr_deg,srv);
                  sp2s((srv-1)/9, (srv-1)%9);sp;sp1l(mapDegToPWM(sr_deg, srv));
                  delay(10);     // see if giving PWM drivers some breathing room helps
                  }
               }
            }
         }
      }
      if(sr_OK == false) { sp2sl("********* invalid SSD command: ",sr_cause);}
      return sr_OK;
   }  //if(cmd == "SSD") 

   // <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: SSP

   // SSP command - Set Servo(s) to PWM value
   if(cmd == "SSP")     // set servo(s) to PWM value
   // command format: SSP, PWM, servo#, (optional end of range servo#)
   //    110 <= PWM <= 490
   //    1 <= servo# <= 18
   //    servo #1 is hip for leg 1, the front right one

   {  sr_OK = true;           // optimistically assume no problems with the command
      if(argN!=2 && argN!=3) {sr_OK = false;sr_cause="bad arg count";}         // wrong number of args for command
      else
      {  sr_pwm = arg[1].toInt();      // first arg is angle for servo
         if(sr_pwm < 110 || sr_pwm > 490 ) {sr_OK = false;sr_cause="bad PWM value";}
         else
         {  sr_srv = arg[2].toInt();   // second arg is servo #
            sr_srv2 = sr_srv;          // assume no 3rd arg, and its a one servo range
            if(sr_srv < 1 || sr_srv > 18) {sr_OK = false;sr_cause="bad first servo #";}
            else
            {  if(argN == 3) {sr_srv2 = arg[3].toInt();}    // there was a second servo#
               if(sr_srv2 < sr_srv || sr_srv2 > 18 ) {sr_OK = false;sr_cause="bad second servo #";}
               else
               {  for(int srv=sr_srv; srv<=sr_srv2; srv++)
                  // now set the servo "srv" to PWM value in sr_pwm
                  //use integer division ("/") and modulus ("%") to directly calculate driver#, and pin#
                  {  pwmDriver[(srv-1) / 9].setPWM((srv-1) % 9, pwmClkStart, sr_pwm);
                  }
               }
            }
         }
      }
      if(sr_OK == false) { sp2sl("********* invalid SSP command: ",sr_cause);}
      return sr_OK;
   } // if(cmd == "SSP")

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: SSO

   // SSO command - Set Servo(s) to "OFF"
   if(cmd == "SSO")     // set servo(s) to OFF, where servo goes limp and draws no current
   // command format: SSO, servo#, (optional end of range servo#)
   //    1 <= servo# <= 18
   //    servo #1 is hip for leg 1, the front right one
   {  sr_OK = true;           // optimistically assume no problems with the command
      if(argN!=1 && argN!=2) {sr_OK = false;sr_cause="bad arg count";}         // wrong number of args for command
      else
      {  sr_srv = arg[1].toInt();   // first arg is servo #
         sr_srv2 = sr_srv;          // assume no 2nd arg, and its a one servo range
         if(sr_srv < 1 || sr_srv > 18) {sr_OK = false;sr_cause="bad first servo #";}
         else
         {  if(argN == 2) {sr_srv2 = arg[2].toInt();}    // there was a second servo#
            if(sr_srv2 < sr_srv || sr_srv2 > 18 ) {sr_OK = false;sr_cause="bad second servo #";}
            else
            {  for(int srv=sr_srv; srv<=sr_srv2; srv++)
               // now set the servo "srv" to OFF - servo should go to limp, and current to zero
               //use integer division ("/") and modulus ("%") to directly calculate driver#, and pin#
               {  pwmDriver[(srv-1) / 9].setPWM((srv-1) % 9, 4096, 0);
               }
            }
         }
         
      }
      if(sr_OK == false) { sp2sl("********* invalid SSO command: ",sr_cause);}
      return sr_OK;
   } // if(cmd == "SSO")
// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: TR
   if(cmd == "TR")      // Dynamic Trace control
   {
      // TR,routine,bits
      // where TR is the command, always TR or tr
      // routine is the routine number, which is same as index in $traceTab.
      //    example: The routine setupFlows has routine number 3, due to this line in configDetails.cpp
      //             $traceTab[3] = t$L + t$W + t$E ;  // routine 3 = setupFlows() in flows.cpp
      // NOTE: there are 2 special routine numbers:
      //       routine 1, or the first offset in $traceTab, offset t$global, represents the global trace enable bits
      //       routine 2, or the second offset is $traceTab, offset t$routing, directs traces to console, MQTT or both
      // bits is the decimal value of the combination of bits to be stored into the corresponding $traceTab entry.
      //      This is some combination of the bits representing trace types:
      //          t$H - High level info/status
      //          t$M - Medium level info/status
      //          t$L - Low level info/status
      //          t$W - Warning of an unexpected but tolerable occurrence
      //          t$E - Error / Exception message. Something went wrong
      //
      // The MQTT TR command allows $traceTab entries to be overwritten at run time, changing how tracing is don
      // If routine is 0, the non-zero entries in $traceTab are displayed (in a series of routable trace commands?)
 
      int rout = arg[1].toInt();    // first arg is routine #
      int bits = arg[2].toInt();    // second argument is the bit combination to be stored
      if ( rout == 0 )              // request to dispay the $traceTab
      {
         traceM("Trace Control table entries, in form: index, decimal value");
         char buffer [50];
         for(int ti = 1; ti < int(maxTraceCount); ti++ )    // step through the trace control table
         {  if($traceTab[ti] != 0 )           // skip over boring zero entries
            {  int t = sprintf(buffer, "%3d  %3d",ti,$traceTab[ti]) ;
               traceM(buffer);
            }
         }
         return sr_OK;
      }
      if ( rout>0 && rout <= maxTraceCount)  // if we have a valid index into $traceTab..
      {  $traceTab[rout] = bits ;            // write value from TR command into the table
      }
      return sr_OK;

   } //  if(cmd == "TR")

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: REPLAY

   if(cmd == "REPLAY" || cmd == "R")      // request to re-execute the last MQTT.fx script loaded &  still in memory 
   //  Format: REPLAY, <toeAction>
   //     (Note there are REPLAYSM and REPLAYMQ commands that allow a replay with single stepping through a flow script)
   //     where <toeaction> is the bit battern that determines what is done one a new set of toe positions is calculated
   //                         (bits are defined in flows.h - search for toeMoveAction )
   {
      // stealing some code from DO_FLOW handler...     
      if (f_count == 0) // is there a flow defined to run?
      {
         Serial.println("replay: tried to run flow, but none defined");
         return false;
      }

      
      // default value for toeMoveAction is whatever the script left it at
      // if REPLAY command has an argument, it's a replacement toeMoveAction
      if (argN >= 1)  { toeMoveAction = arg[1].toInt(); }

      f_active = 0;  // starting at the 0th entry in the flow arrays
      f_flowing = 1; // we're now executing a flow
      return true;

   }  // if(cmd == "REPLAY") 

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: STEPSM
   if(cmd == "STEPSM")      // request to single step flow execution via the serial monitor 
   //  Format: STEPSM, value
   //          (Note there is also a STEPMQ command for stepping control via MQTT)
   //     where <value> is 0 to turn off single stepping
   //                      1 to turn on single stepping controlled by the serial monitor
   //                      absent to get current value of stepMode displayed
   {

      if (argN == 0) { sp2sl("stepMode (0-disabled, 1-serial monitor, 2-MQTT)",stepMode);}  
      else { stepMode = arg[1].toInt(); }  // probably should validate arg
  
      return true;

   }  // if(cmd == "STEPSM") 

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: STEPMQ
   if(cmd == "STEPMQ")      // request to single step flow execution via MQTT commands 
   //  Format: STEPMQ, value
   //          (Note there is also a STEPMQ command for stepping control via MQTT)
   //     where <value> is 0 to turn off single stepping
   //                      1 to turn on single stepping controlled by MQTT commands
   //                      absent to get current value of stepMode displayed
   {
      if (argN = 0) { sp2sl("stepMode (0-disabled, 1-serial monitor, 2-MQTT)",stepMode);}  
      else { stepMode = 2 * arg[1].toInt(); }  // probably should validate arg
  
      return true;

   }  // if(cmd == "STEPMQ") 


// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> start of new command: New Cmd Here
// add new commands above this comment, in this form, with one tab before "if"
//    if ( cmd == "COMMAND" || cmd == "SHORT-FORM")
//    {
//       code
//       return false;  // command rejected
//       code
//       return true;   // command succeeded
//    }    
      Log.warningln("<processCmd> Warning - unrecognized command.");
      sp1l(cmd);
      return false;
} // processCmd()

void goToAngles(int argLeg)    // inner core of MLA, MLC, MLRH commands, and soon: QGH, QGS, QGU, QGC
{        // desired angles already set up in f_angH, f_angK, f_angA
         int lowLeg = argLeg;     // set up for a loop with only one iteration
         int hiLeg = argLeg;
         if(argLeg == 0)         // if leg  was 0, do command for all 6 legs
         {  lowLeg = 1;
            hiLeg = 6;
         }
         for(int LargLeg=lowLeg; LargLeg<=hiLeg; LargLeg++)
         {
            float LargH = -1 * f_angH;    // matching empiracal testing
            float LargK = -1 * f_angK;
            float LargA = -1 * f_angA;
            
            if(LargLeg >= 4)            // might need to negate some angles...
            {  LargK = -1 * LargK;   //... because servos are mounted opposite ways on opposite sides of bot
               LargH = -1 * LargH;
               LargA = -1 * LargA;
            }  // if LargLeg>=4
         
            // the per servo calibration is done inside the mapDegToPWM function 

            // starting with the hip...
            int servoNum = ((LargLeg - 1) * 3) +1;   // servo numbers go from 1 to 9. This is hip servo for leg argLeg
            int PWM = mapDegToPWM(LargH,servoNum);
            //sp3sl("  Hip angle, PWM=",LargH,PWM);
            pwmDriver[legIndexDriver[LargLeg]].setPWM(legIndexHipPin[LargLeg],  pwmClkStart, PWM);

            // then the knee
            servoNum = ((LargLeg - 1) * 3) +2;   // servo numbers go from 1 to 9. This is knee servo for leg argLeg
            PWM = mapDegToPWM(LargK,servoNum);
            //sp3sl(" Knee angle, PWM=",LargK,PWM);
            pwmDriver[legIndexDriver[LargLeg]].setPWM(legIndexHipPin[LargLeg]+1,pwmClkStart, PWM);

            //then the ankle
            servoNum = ((LargLeg - 1) * 3) +3;   // servo numbers go from 1 to 9. This is ankle servo for leg argLeg
            PWM = mapDegToPWM(LargA,servoNum);
            //sp3sl("Ankle angle, PWM=",LargA,PWM);
            pwmDriver[legIndexDriver[LargLeg]].setPWM(legIndexHipPin[LargLeg]+2,pwmClkStart, PWM);
         }
} // void goToAngles(argLeg)

void goToSquat()     // gently go to squat position with legs slightly of the ground
{
         // want to be gentle when we lay bottom of robot on ground, so go to home position first
         f_angH = 0;       // zero angle for hip servo
         f_angK = 0;       // ..and for knee
         f_angA = 0;       // ..and for ankle
         goToAngles( 0 );  // move all legs to these servo angles
         delay(300);       // could be coming from any position, so give it some time

         // then to almost on the ground
         coordsToAngles(12, -9, 0); // creates f_angH, f_angK, f_angA from local coords
         goToAngles(0) ;            // move all legs to this position
         delay(200);                 // give it a bit of time

         // a little closer to ground
         coordsToAngles(12, -8, 0); // creates f_angH, f_angK, f_angA from local coords
         goToAngles(0) ;            // move all legs to this position
         delay(200);                 // give it a bit of time

         // still closer, maybe on the ground by now
         coordsToAngles(12, -7, 0); // creates f_angH, f_angK, f_angA from local coords
         goToAngles(0) ;            // move all legs to this position
         delay(200);                 // give it a bit of time

         // and lift the legs above ground just a bit
         coordsToAngles(12, -6, 0); // creates f_angH, f_angK, f_angA from local coords
         goToAngles(0) ;            // move all legs to this position
         delay(200);                 // give it a bit of time
}  // goToSquat

   /** 
 * @brief Return the MQTT broker IP address.
 * =================================================================================*/
   IPAddress getMqttBrokerIP()
   {
      return brokerIP;
   } // getMqttBrokerIP()

   /** 
 * @brief Check to see if any MQTT commands have come in from the broker.
 * =================================================================================*/
   void checkMqtt()
   {
      // quickly reschedule our next check before we get busy
      next_checkMqtt_mills = millis() + period_checkMqtt_mills;
      
      // ====    either check for highest priority first (20 msec move) or use FREERTOS pre-emptive multitasking
      String cmd = mqtt.getCmd();
      if (cmd != "")
      {
         //Log.noticeln("<checkMqtt> cmd = %s.", cmd.c_str());
         bool allIsWell = processCmd(cmd);
         if (allIsWell)
         {
            //Log.noticeln("<checkMqtt> All went well.");
         } // if
         else
         {
            Log.warningln("<checkMqtt> Something went wrong.");
         } // if
      }    // if
   }       // checkMqtt()

#endif // End of precompiler protected code block