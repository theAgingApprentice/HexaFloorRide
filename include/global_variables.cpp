// this is the file global_variables.cpp which lives in the /include folder
// it contains all global variables, const and non-const
// (syntax declarations are required, value definition is optional)
// and global macros (#define)

// quick and dirty easily typed debug commands

// the macro name is composed as follows:
//  sp   which is short for Serial.print
//   n   a digit indicating how many values need to be printed
//   s   an optional "s" meaning add a space between and after values
//   l   an optional "l" meaning do a newline after all values are printed
//
// note there are 2 other short form print macros:
//   nl  print a newline
//   sp  print a space
//
//  example: sp3sl("ankle coords are:",Ax,Ay);

#define sp1(_y) Serial.print(_y);

#define sp1l(_z) Serial.println(_z);

#define sp1s(_b)     \
   Serial.print(_b); \
   Serial.print(" ");

#define sp2(x, y)   \
   Serial.print(x); \
   Serial.print(y);

#define sp2s(x, y)    \
   Serial.print(x);   \
   Serial.print(" "); \
   Serial.print(y);   \
   Serial.print(" ");

#define sp2l(x, y)  \
   Serial.print(x); \
   Serial.println(y);

#define sp2sl(x, y)   \
   Serial.print(x);   \
   Serial.print(" "); \
   Serial.println(y);

#define sp3s(a,b,c)   \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.print(c);   \
   Serial.print(" ");

#define sp3sl(a,b,c)  \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.println(c);

#define sp4s(a,b,c,d)   \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.print(c);   \
   Serial.print(" "); \
   Serial.print(d);   \
   Serial.print(" ");

#define sp4sl(a,b,c,d)   \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.print(c);   \
   Serial.print(" "); \
   Serial.println(d);

#define sp5s(a,b,c,d,e)   \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.print(c);   \
   Serial.print(" "); \
   Serial.print(d);   \
   Serial.print(" "); \
   Serial.print(e);   \
   Serial.print(" ");

#define sp5sl(a,b,c,d,e)   \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.print(c);   \
   Serial.print(" "); \
   Serial.print(d);   \
   Serial.print(" "); \
   Serial.println(e);

#define sp6s(a,b,c,d,e,f)   \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.print(c);   \
   Serial.print(" "); \
   Serial.print(d);   \
   Serial.print(" "); \
   Serial.print(e);   \
   Serial.print(" "); \
   Serial.print(f);   \
   Serial.print(" ");

#define sp6sl(a,b,c,d,e,f)   \
   Serial.print(a);   \
   Serial.print(" "); \
   Serial.print(b);   \
   Serial.print(" "); \
   Serial.print(c);   \
   Serial.print(" "); \
   Serial.print(d);   \
   Serial.print(" "); \
   Serial.print(e);   \
   Serial.print(" "); \
   Serial.println(f);

#define sp Serial.print(" ");
#define nl Serial.println();

// specify how much detail we want when identifying source location of a trace.
// uncomment just one of the following lines
//#define t$SFolderFileFuncLine 1   // most detailed, but longest trace source location info
//#define t$SFileFuncLine 1         // like above, with folder, if present, suppressed
//#define t$SFuncLine 1             // function name <id-number>, and line number
#define t$SFunc 1                   // just function name <id-number>

#define t$timestamps 1            // if defined, adds timestamps, in millis(), to all trace messages
                                    // comment out above line to remove timestamps from trace messages

// symbols to help in constructing trace messages

// first, the message types
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

// traces are made using the macros below, like traceH, but we need to declare underlying routines.

void tracer_n(String, String, String, int, int, String);           // trace with no variable display
void tracer_s(String, String, String, int, int, String, String);   // trace with String variable display
void tracer_i(String, String, String, int, int, String, int);      // trace with Integer variable display
void tracer_f(String, String, String, int, int, String, float);    // trace with Foat variable display
                                                   // actual routines are in terminal.cpp

// speedy trace macro versions where the trace command name implies some of the arguments

//-------------------commands for type H traces - High Level info/status
//  traceH  - trace type is t$H, and there is no variable to be displayed
//          - form: traceH(labelText), 
//          - example: traceH("I2C initialization successful")
#define traceH(LabelText) tracer_n(__FILE__,__func__,String(__LINE__),localRNum,t$H,LabelText)

//  traceHs - trace type is t$H, and there is a String variable to be displayed after the lebelText string
//          - form: traceHs(labelText, stringVariable), 
//          - example: traceHs("network setup complete on Network ", SSID)
#define traceHs(LabelText,sVar) tracer_s(__FILE__,__func__,String(__LINE__),localRNum,t$H,LabelText,sVar)

//  traceHi - trace type is t$H, and there is an integer variable to be displayed after the labelText string
//          - form: traceHi(labelText,intVariable), 
//          - example: traceHi("I2C init complete, device count= ", numDevFound)
#define traceHi(LabelText,iVar) tracer_i(__FILE__,__func__,String(__LINE__),localRNum,t$H,LabelText,iVar)

//  traceHf - trace type is t$Hm and here is a float variable to be displayed after the labelText string
//          - form: traceHf(labelText,floatVariable), 
//          - example: traceHf("Flash memory percent used= ", FlashPercent)
#define traceHf(LabelText,fVar) tracer_f(__FILE__,__func__,String(__LINE__),localRNum,t$H,LabelText,fVar)


//--------------------commands for type M traces - Medium level info/status, similar variations
//  traceM  - trace type is t$M, and there is no variable to be displayed
#define traceM( LabelText ) tracer_n(__FILE__,__func__,String(__LINE__),localRNum, t$M, LabelText )

//  traceMs - trace type is t$M, and there is a String variable to be displayed after the lebelText string
#define traceMs(LabelText,sVar) tracer_s(__FILE__,__func__,String(__LINE__),localRNum,t$M,LabelText,sVar)

//  traceMi - trace type is t$M, and there is an integer variable to be displayed after the labelText string
#define traceMi(LabelText,iVar) tracer_i(__FILE__,__func__,String(__LINE__),localRNum,t$M,LabelText,iVar)

//  traceMf - trace type is t$Mm and there is a float variable to be displayed after the labelText string
#define traceMf(LabelText,fVar) tracer_f(__FILE__,__func__,String(__LINE__),localRNum,t$M,LabelText,fVar)


//--------------------commands for type L traces - Low level info/status, similar variations
//  traceL  - trace type is t$H, and there is no variable to be displayed
#define traceL(LabelText) tracer_n(__FILE__,__func__,String(__LINE__),localRNum,t$L,LabelText)

//  traceLs - trace type is t$H, and there is a String variable to be displayed after the lebelText string
#define traceLs(LabelText,sVar) tracer_s(__FILE__,__func__,String(__LINE__),localRNum,t$L,LabelText,sVar)

//  traceLi - trace type is t$H, and there is an integer variable to be displayed after the labelText string
#define traceLi(LabelText,iVar) tracer_i(__FILE__,__func__,String(__LINE__),localRNum,t$L,LabelText,iVar)

//  traceLf - trace type is t$Hm and there is a float variable to be displayed after the labelText string
#define traceLf(LabelText,fVar) tracer_f(__FILE__,__func__,String(__LINE__),localRName,localRNum,t$L,LabelText,fVar)


//----------------------commands for type W traces - Warnings, similar variations
//  traceW  - trace type is t$H, and there is no variable to be displayed
#define traceW(LabelText) tracer_n(__FILE__,__func__,String(__LINE__),localRNum,t$W,LabelText)

//  traceWs - trace type is t$H, and there is a String variable to be displayed after the lebelText string
#define traceWs(LabelText,sVar) tracer_s(__FILE__,__func__,String(__LINE__),localRNum,t$W,LabelText,sVar)

//  traceWi - trace type is t$H, and there is an integer variable to be displayed after the labelText string
#define traceWi(LabelText,iVar) tracer_i(__FILE__,__func__,String(__LINE__),localRNum,t$W,LabelText,iVar)

//  traceWf - trace type is t$Hm and there is a float variable to be displayed after the labelText string
#define traceWf(LabelText,fVar) tracer_f(__FILE__,__func__,String(__LINE__),localRNum,t$W,LabelText,fVar)


//-------------------------commands for type E traces - Errors, similar variations
//  traceE  - trace type is t$H, and there is no variable to have it's value displayed
#define traceE(LabelText) tracer_n(__FILE__,__func__,String(__LINE__),localRNum,t$E,LabelText)

//  traceEs - trace type is t$H, and there is a String variable to be displayed after the lebelText string
#define traceEs(LabelText,sVar) tracer_s(__FILE__,__func__,String(__LINE__),localRNum,t$E,LabelText,sVar)

//  traceEi - trace type is t$H, and there is an integer variable to be displayed after the labelText string
#define traceEi(LabelText,iVar) tracer_i(__FILE__,__func__,String(__LINE__),localRNum,t$E,LabelText,iVar)

//  traceEf - trace type is t$Hm and there is a float variable to be displayed after the labelText string
#define traceEf(LabelText,fVar) tracer_f(__FILE__,__func__,String(__LINE__),localRNum,t$E,LabelText,fVar)


int8_t displayPage = 1;

// physical dimensions - key measurements. All other measurements are based on these


#define d$cornerX 8.901    // centimeters from center of a corner hip servo to lateral center line

// centimeters from center of a corner hip servo to the front/back center line
#define d$cornerY 5.090

//centometers from side hip servo center to lateral center line
#define d$sideX 0.000

//centimeters from side hip servo center to front/back center line
#define d$sideY 6.535

//centimeters vertical distance from chassis top surface (extended) to center of knee servo
#define d$kneeW 3.683

//centimeters horizontal distance from hip servo center to knee servo center
#define d$thighL 4.702

//centimeters from knee servo center to ankle servo center
#define d$shinL 7.620

//centimeters from ankle servo center to tip of toe
#define d$footL 11.059

// variables are initialize in configDetails.cpp:

// now define the global home position of toes when all servos are centered
// starting with leg 1, at front right
float d$leg1X = d$cornerX;
float d$leg1Y = -d$cornerY;
float d$leg1Z = d$kneeW - 0;

const float f_staticHomeX = d$thighL + d$shinL;    // home position in local coordinates
const float f_staticHomeY = -1. * d$footL;         // (same for all legs) origin is at knee servo level
const float f_staticHomeZ = 0.0; 

// variables related to flow processing
int stepMode;    // control parameter for single stepping throgh slow scripts
#define stepMode_SM 1      // we are doing single stepping controlled by serial monitor
#define stepMode_MQ 2      // we are doing single stepping controlled by mMQTT commands STEPSM and STEPMQ

// start of big xfer from main.h =============================
const char* HOST_NAME_PREFIX = "hfr"; // concise prefix for our unique network name. LC to contrast with hex MAC suffix.

bool networkConnected = false;
bool mqttBrokerConnected = false;
bool oledConnected = false;
bool motorController1Connected = false;
bool motorController2Connected = false;
bool mobilityStatus = false;

// variables and declarations related to keeping Broker IP adress in NVM
IPAddress brokerIP; // IP address of the MQTT broker.
const char* DEFAULT_MQTT_IP = "192.168.0.99";
const char* FLASH_APP_NAME = "my_app";
IPAddress flashReadBrokerIP();
void flashWriteBrokerIP(IPAddress address);


// MQTT related variables.
char uniqueName[HOST_NAME_SIZE]; // Character array that holds unique name for Wifi network purposes. 
char *uniqueNamePtr = &uniqueName[0]; // Pointer to first address position of unique name character array.
char healthTopicTree[50] = ""; // Char array to hold full health topic tree name.
char helpTopicTree[50] = ""; // Char array to hold full health topic tree name.
String result[2] = {"false","true"}; // Provide english lables for true and flase return codes.
// RGB OLED related variabes.
const uint8_t numColoursSupported = 9; // Number of colours LED can be set to.
const bool commonAnode = true; // Set to true if RGB LED has a common anode, false for common cathode.
const uint8_t RED = 0; // Reset button LED red. 
const uint8_t GREEN = 1; // Reset button LED green. 
const uint8_t BLUE = 2; // Reset button LED blue. 
const uint8_t YELLOW = 3; // Reset button LED YELLOW. 
const uint8_t ORANGE = 4; // Reset button LED PINK.
const uint8_t PINK = 5; // Reset button LED PINK.
const uint8_t AQUA = 6; // Reset button LED aqua. 
const uint8_t WHITE = 7; // Reset button LED white. 
const uint8_t BLACK = 8; // Reset button LED white. 
const double_t PWM_FREQ = 500; // 500Hz.
const uint8_t PWM_RESOLUTION = 8; // ESP32 can go up to 8 bit PWM resolution.
#define PWM_RED_CHANNEL 0 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 0 for RGB red.
#define PWM_GREEN_CHANNEL 1 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 1 for RGB green.
#define PWM_BLUE_CHANNEL 2 // ESP32 has 16 channels which can generate 16 independent waveforms. Use 2 for RGB blue.
typedef struct
{
   String name; // Name associated with colour properties
   uint32_t redDutyCycle; // Up time of the PWM signal. Ranges from 0-256. 127 is a 50% duty cycle for example.
   uint32_t greenDutyCycle; // Up time of the PWM signal. Ranges from 0-256. 127 is a 50% duty cycle for example.
   uint32_t blueDutyCycle; // Up time of the PWM signal. Ranges from 0-256. 127 is a 50% duty cycle for example.
}struct_Colour; 
struct_Colour statusColour[numColoursSupported]; // Array of colours.
struct_Colour memColour; // Used to switch back RGB LED colour when it is temporarily changed.

// I2C related variables.
#define I2C_BUS0_SPEED 400000 // Define speed of I2C bus 2. Note 400KHz is the upper speed limit for ESP32 I2C
#define I2C_BUS1_SPEED 100000 // Define speed of I2C bus 2. Note 100KHz is the upper speed limit for ESP32 I2C
#define MPU6050_I2C_ADD 0x68 // GY521 I2C address.
#define leftOLED_I2C_ADD 0x3D // OLED used for robot's left eye I2C adddress.
#define rightOLED_I2C_ADD 0x3C // OLED used for robot' right eye I2C address.
#define dcMotorController 0xB0 >> 1 // Wire Library only uses 7 bit addresses so you need to shift address one bit to the right.
#define LCD16x2 0x3F // Liquid Crystal Display.
#define PCA9685ServoDriverAllCall 0x70 // Global I2C address for all servo drivers.
#define PCA9685ServoDriver1 0x40 // I2C address for first servo driver.
#define PCA9685ServoDriver2 0x41 // I2C address for second servo driver.
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates.
uint32_t oscFreq = 27000000; // Frequency of oscilator on motor driver. 
//#define PCA9685ServoDriver3 0x42 // I2C address for third servo driver.
//#define PCA9685ServoDriver4 0x43 // I2C address for fourth servo driver.
//#define PCA9685ServoDriver5 0x44 // I2C address for fifth servo driver.
//#define PCA9685ServoDriver6 0x45 // I2C address for sixth servo driver.
//#define PCA9685ServoDriver7 0x46 // I2C address for seventh servo driver.

// Define variables related to Onboard LED
int OnboardLED = 13;       // GPio 13 is the onboard LED
void setupOnboardLED();    // prepare do GPio 13 can control Huzzah32 onboard LED
int capstate;              // capacitive sensor state in oneSec.

// Define OLED related variables.
bool buttonA_flag = false; // Flag used by hardware ISR for button A.
bool buttonB_flag = false; // Flag used by hardware ISR for button B.
bool buttonC_flag = false; // Flag used by hardware ISR for button C.
uint8_t oledX = 128; // Screen width in pixels.
uint8_t oledY = 64; // Screen height in pixels.
uint8_t textBaseX = 6; // Smallest font width in pixels.
uint8_t textBaseY = 8; // Smallest font height in pixels.
uint8_t oledOrientation = 3; // Orientation of OLED. 

// Define servoLegs related variables that are still used by other code
#define numDrivers 2 // Number of servo motor drivers robot has.
uint16_t pwmClkStart = 0; // Start value of count-up PWM high signal.

// Define terminal related variables.
unsigned long serialBaudRate = 115200; // Serial terminal baud rate.

// Define local web server related variables.
bool isWebServer; // True is web server running.
const char* WEB_APP_TITLE = "Hexbot"; // App name for web page titles.

/************************************************************************************
 * @section funcDeclare Declare functions found in the include files.
 * @details In order to get rid of "error: 'xxx' was not declared in this scope"
 * compiler errors list a function from any of the include files here. 
 ************************************************************************************/
void rotateDisplay(int8_t); 
void displaySplashScreen(String);
// Config ralted functions
void showCfgDetails();
void displayCfgDetails(int8_t);
void cfgToConsole() ;       // from /lib/aaChip-1.0.1 conversion to configDetails.cpp::cfgConsole()
void checkBoot();
// MQTT related functions
bool connectToMqttBroker(aaNetwork &);
boolean isValidNumber(String);
uint32_t convertStrToUint32_t(String);
uint8_t convertStrToUint8_t(String);
int8_t convertStrToInt8_t(String);
bool checkNumArg(int8_t, String *);
bool processCmd(String);
//IPAddress getMqttBrokerIP();
void checkMqtt();
// RGB LED related functions.
void createPredefinedColours();
void saveRgbColour();
void loadRgbColour();
void setCustRgbColour(uint32_t, uint32_t, uint32_t);
void setStdRgbColour(uint8_t);
void setupStatusLed();

// I2C related functions.
void identifyDevice(int);
void scanBus0();
void scanBus1();
// Define OLED related functions.
void IRAM_ATTR ButtonA_ISR();
void IRAM_ATTR ButtonB_ISR();
void IRAM_ATTR ButtonC_ISR();
void placeTextVHcentre(String, uint8_t, uint16_t);
void placeTextHcentre(String, uint8_t, uint16_t);
void rotateDisplay(int8_t);
void displaySplashScreen(String);
void displayStatusScreen();
void checkOledButtons();
void initOled();

// Define servoLegs related functions. <<<< no longer used
//void startPositionLegs(int8_t, int8_t);
//void initServos();
//void initLegs();
//bool moveLeg(int8_t, int8_t, float, float, float);
//void setupMobility();  

//define functions related to device setup
void setupPwmdriver() ;  // in deviceSupport.cpp, called from main.cpp:setup()

// Define terminal related functions.
void setupSerial();

// Define local web server related functions.
void monitorWebServer();
void startWebServer();

// variables related to the tasks called from loop), triggered by either timers or orconditions
// and the once per second calculations of CPU performance numbers

// handy macro to get all the needed CPU usage/dispatching variables defined consistently:
// arguments are:
//   taskID - a 4 to 8 character abbreviation of the routine name
//   period - the number of milliseconds between scheduled executions of the routine

      #define loopTaskVars(taskID,period)  \
      int sched_##taskID##_mills ; \
      unsigned long next_##taskID##_mills ; \
      unsigned long cum_##taskID##_delay ; \
      int max_##taskID##_delay ; \
      int lates_##taskID ; \
      unsigned long time_##taskID##_mics ; \
      int max_##taskID##_mics ; \
      const int period_##taskID##_mills = period;

// now generate the variables for our routines in loop() dispatched by millis() timers
loopTaskVars(webMon,200);     // the monitorWebServer() task runs every 200 milleseonds
void monitorWebServer();   // in web.cpp

loopTaskVars(checkOled,201);  // check Oled buttons. intentionally desynchronized from webMon
void checkOledButtons();   // in Oled.cpp for now, moving to deviceSupport.cpp

loopTaskVars(checkMqtt,202);  // checkMqtt() in mqttBroker.cpp
void checkMqtt();

loopTaskVars(flow,203);       // do_flow, condition call from loop
void do_flow();

loopTaskVars(oneSec,1000);    // the once per second routine that does CPU usage displays and other things
void oneSec();                // ... which is itself a timer driven task.

bool firstOneSec;    // flag to identify first entry to oneSec, which does initialization
int mills;           // timestamp in millis()
int mics;            // timestamp in micros()
unsigned long deltaSched;     // diference between scheduled millis() time and when routine actually ran
unsigned long deltaTime;      // elapsed time = time after take away time before

/*
// webmonitoring is now dispatched on a timer rather than a loop() mainline function call
int nextMonWebMillis;         // timestamp in msec to next do monitorWebServer task
const int monWebTime = 200;   // number of milleseconds between monitorWebServer calls in loop()

// checkOledButtons is now dispatched via a timer
int nextCheckOledMillis;      // timestamp in msec to next do checkOledButtons() task
const int checkOledTime = 201;// number of milleseconds between checkOledButtons calls in loop()
                              // intentionally offset from other timers to even out task execution
// checkMqtt is now dispatched via a timer
int nextCheckMqttMillis;      // timestamp in msec to next do checkMqtt() task
const int checkMqttTime = 202;// number of milleseconds between checkMqtt calls in loop()
*/
/*******************************************************************************
 * @section codeModules Functions put into files according to function.
 * @details Order functions here in a way that ensures that variables get 
 * declared in one function prior to being referenced by another function. 
 *******************************************************************************/



