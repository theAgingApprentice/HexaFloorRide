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
   String type = "?";    // translate type to letter that starts message
   if(tType == t$H) {type = "H";}
   if(tType == t$M) {type = "M";}
   if(tType == t$L) {type = "L";}
   if(tType == t$W) {type = "W";}
   if(tType == t$E) {type = "E";}

   // build message in a string that can go to console and/or MQTT
   String tMsg = type + ") "+ name +"["+ functionID +"]:" +dataLabel +" "+ var ;   // build first part of message
   sp1l(tMsg) ;     // print the string followed by a newline
   return ;
}

#endif // End of precompiler protected code block