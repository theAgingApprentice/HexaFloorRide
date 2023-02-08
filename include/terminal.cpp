/*******************************************************************************
 * @file terminal.cpp  
 * @brief File containing all terminal functions.
 *******************************************************************************/
#ifndef terminal_cpp // Start of precompiler check to avoid dupicate inclusion of this code block.

#define terminal_cpp // Precompiler macro used for precompiler check.

/**
 * @brief Initialize the serial output.
 * ==========================================================================*/
#include <iostream>
#include <string>
void setupSerial()
{
   Serial.begin(serialBaudRate); // Open a serial at specified baud rate. 
   while(!Serial); // Wait for Serial port to be ready.
} //setupSerial()


// trace message generation function
//   -calls to this function are done as a result of encountering the trace macro
//   -there are macros for various types of variable data that can be included in th etracemessage
//     these other macros populate the string varData appropriately, and then call tracer to generate the trace
//   -operation of this function is controlled in real time by entries in the trace Table $traceTab[maxTraceNum]
//   -traceTable can be modified in real time by the MQTT  TR command
//   -documentation for tracing is in docs/trace-design.pd
void tracer(String file, String func, String line,int functionID, int tType, String dataLabel, String varData)
{
   // check to see if this trace is enabled.
   // if either the global enables, or the enables for this routine enable the bit
   //   which identifes this routines type, then generate the trace
   // bit parallel logic: (global enables .or. routine enables ) .and. (routine type) non-zero means do it 

   if((($traceTab[t$global] | $traceTab[functionID]) & tType) != 0)
   {
      String type = "?";    // translate type to letter that starts message
      if(tType == t$H) {type = "H";}
      if(tType == t$M) {type = "M";}
      if(tType == t$L) {type = "L";}
      if(tType == t$W) {type = "W";}
      if(tType == t$E) {type = "E";}

      // build message in a string that can go to console and/or MQTT
      // format of the location of the trace is controlled by parameters starting t$S... global_variables

      //example: L) include/configDetails.cpp:setupPerBotConfig{9}-L21> MAC Address:  3C:61:05:4A:DD:98
      #ifdef t$SFolderFileFuncLine        // folder, file, function, line#
      String t_source = file + "\\" + func +"{" +functionID + "}-L" + line ;
      #endif

      //example: L) configDetails.cpp:setupPerBotConfig{9}-L21> MAC Address:  3C:61:05:4A:DD:98
      #ifdef t$SFileFuncLine              //file, function, line#
      String fil = file.substring((file.indexOf("/")+1),99);
      String t_source = fil + "\\" + func + "{" +functionID + "}-L" + line ;
      #endif

      //example: L) configDetails.cpp:setupPerBotConfig{9}-L21> MAC Address:  3C:61:05:4A:DD:98
      #ifdef t$SFuncLine        //function, line#      #endif
      String t_source = func +"{" +functionID + "}-L" + line ;
      #endif

      #ifdef t$SFunc              // just function
      String t_source = func + "{" +functionID + "}" ;
      #endif

      String tMsg = type + ") "+ t_source +"> " +dataLabel +" "+ varData ;   // build the trace message

      if(($traceTab[t$routing] & t$SM) != 0)    // if we're routing traces to the serial monitor...
      {sp1l(tMsg) ;     // print the string followed by a newline
      }
      if(($traceTab[t$routing] & t$MQ) != 0)    // if we're routing traces to MQTT...
      {   // need to work on MQTT output     // send the string to MQTT, followed by a newline
      }

   }
   return ;
}  // voi tracer

// trace variation where there is no variable, i.e. traceH, traceM, traceL, traceW and traceE
void tracer_n(String file, String func, String line, int functionID, int tType, String dataLabel)
{
   String varData = " " ;    // we'll just pretend we have an invisible variable with one space
   tracer(file, func, line, functionID, tType, dataLabel, varData);
   return ; 
}

// trace variation where there is a string variable, i.e. traceHs, traceMs, traceLs, traceWs and traceEs
void tracer_s(String file, String func, String line, int functionID, int tType, String dataLabel, String varData)
{
   // this is an easy case - building an output string from an input that is a string. Poof! done
   //String varDataS = varData ;    // we'll just pretend we have an invisible variable with one space
   tracer(file, func, line, functionID, tType, dataLabel, varData);
   return ;
}

// trace variation where there is an integer variable, i.e. traceHi, traceMi, traceLi, traceWi and traceEi
void tracer_i(String file, String func, String line, int functionID, int tType, String dataLabel, int varData)
{
   // need to create a string representing the integer variable
   char buffer [40];
   int t = sprintf(buffer, " %d",varData);   // convert integer to string with leading space
   String varDataS = buffer ;                // change it to the style of string we're used to
   tracer(file, func, line, functionID, tType, dataLabel, varDataS); // and use it in the trace message
   return ;
}

// trace variation where there is a float variable, i.e. traceHf, traceMf, traceLf, traceWf and traceEf
void tracer_f(String file, String func, String line, int functionID, int tType, String dataLabel, float varData)
{
   // need to create a string representing the float variable
   char buffer [40];
   int t = sprintf(buffer, " %f",varData);   // convert integer to string with leading space
   String varDataS = buffer ;                // change it to the style of string we're used to
   tracer(file, func, line, functionID, tType, dataLabel, varDataS); // and use it in the trace message
   return ;
}


#endif // End of precompiler protected code block
