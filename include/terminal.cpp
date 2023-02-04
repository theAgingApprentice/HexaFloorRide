/*******************************************************************************
 * @file terminal.cpp  
 * @brief File containing all terminal functions.
 *******************************************************************************/
#ifndef terminal_cpp // Start of precompiler check to avoid dupicate inclusion of this code block.

#define terminal_cpp // Precompiler macro used for precompiler check.

/**
 * @brief Initialize the serial output.
 * ==========================================================================*/
void setupSerial()
{
   Serial.begin(serialBaudRate); // Open a serial at specified baud rate. 
   while(!Serial); // Wait for Serial port to be ready.
} //setupSerial()


// trace message generation function
//   -calls to this function are done as a result of encountering the trace macro
//   -operation of this function is controlled in real time by entries in the traceTable tTrce[maxTraceNum]
//   -traceTable can be modified in real time by an MQTT command
//   -documentation for tracing is in docs/trace-design.pd
void tracer(String name, int functionID, int tType, String dataLabel, float var)
{
   // crude initial implementation unconditionaly prints a formatted message
   // check to see if this trace is enabled.
   // if either the global enables, or the enables for this routine enable the bit
   //   which identifes this routines type, then generate the trace
   // bit parallel logic: (global enables .or. routine enables ) .and. (routine type) non-zero means do it 

   /*
   sp2sl("FunctionID= ",functionID);
   sp2sl("global= ",$traceTab[t$global]);
   sp2sl("routine= ",$traceTab[functionID]);
   sp2sl("tType= ",tType);
   int bits = ($traceTab[t$global] | $traceTab[functionID]) & tType;
   sp2sl("bit result= ",bits);
   */
   if((($traceTab[t$global] | $traceTab[functionID]) & tType) != 0)
   {
      String type = "?";    // translate type to letter that starts message
      if(tType == t$H) {type = "H";}
      if(tType == t$M) {type = "M";}
      if(tType == t$L) {type = "L";}
      if(tType == t$W) {type = "W";}
      if(tType == t$E) {type = "E";}

      // build message in a string that can go to console and/or MQTT
      String tMsg = type + ") "+ name +"["+ functionID +"]:" +dataLabel +" "+ var ;   // build the trace message

      if(($traceTab[t$routing] & t$SM) != 0)    // if we're routing traces to the serial monitor...
      {sp1l(tMsg) ;     // print the string followed by a newline
      }
      if(($traceTab[t$routing] & t$MQ) != 0)    // if we're routing traces to MQTT...
      {   // need to work on MQTT output     // send the string to MQTT, followed by a newline
      }

   }
   return ;
}

#endif // End of precompiler protected code block