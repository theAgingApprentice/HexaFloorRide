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

// symbols to help in constructing trace messages
// first, the message type
const int tStatus = 1;
const int tWarn = 2;
const int tError = 3;

//next the message level
const int tTop = 1;
const int tMed = 2;
const int tLow = 3;

// define the table that defines, on the fly, how individual traces are to be processed and routed

// index 1 in this table is a special case, and defines the global trace enabling, using bit definitions below
   const int t_global = 1;        // symbol for this special index
// index 2 is a special case that defines where tracemessages are sent, using these bit definitions
   const int t_routing = 2;       // symbol for this special index

   const int tb_toConsole = 1;    // if this bit is set, trace messages will go to console = serial monitor
   const int tb_toMQTT = 2;       // if this bit is set, trace messages will go to MQTT topic ??? TBD
                        // it is OK to set both these bits, which will cause messages to go to both destinations

// The remaining indices, starting at 3 define trace handling for a particular function.
// The index number appears in the trace command that triggers the trace message
// There is also a sub index, so you can have multiple traces in a function that can be differentiated
// Used indices are documented in the comments next to the table entry initialization
// This table initialization is done at compile time, and is the default tracing configuration
// However, an MQTT command can be used to overwrite the numbers in the table, and to display it.
//
// definition of bits used to build table values for each function
   const int tb_all =       1;    // all traces are enabled
   const int tb_allTop =    2;    // all traces at Top level
   const int tb_allMed =    4;    // all traces at Medium level
   const int tb_allLow =    8;    // all traces at Low level
   const int tb_allStatus =16;    // all traces with status info
   const int tb_allWarn =  32;    // all traces with warning info
   const int tb_allError = 64;    // all traces with error info
   const int tb_allTS    =128;    // all traces with top level status info
   const int tb_allTW    =256;    // all traces with top level warning info
   const int tb_allTE    =512;    // all traces with top level error info
   const int tb_allMS    =128;    // all traces with medium level status info
   const int tb_allMW    =256;    // all traces with medium level warning info
   const int tb_allME    =512;    // all traces with medium level error info
   const int tb_allLS    =128;    // all traces with low level status info
   const int tb_allLW    =256;    // all traces with low level warning info
   const int tb_allLE    =512;    // all traces with low level error info

   #define maxTraceCount 100       // support for up to 100 trace messages

   //int tTable[maxTraceCount];    // reserve space for the trace control table
   uint16_t tTable[100];

   // compiler won't let me populate the table here, so I'm going to try in configDetails.cpp
void tracer(String, int, int, int, int, String, float);
#define trace(name,subID,functionID,tType,tLevel,labelText,var)  \
   tracer(name, subID, functionID, tType, tLevel, labelText, var);

int8_t displayPage = 1;

// start of big xfer from main.h =============================
const char* HOST_NAME_PREFIX = "HexaFloorRide"; // Prefix for our unique network name.

bool networkConnected = false;
bool mqttBrokerConnected = false;
bool oledConnected = false;
bool motorController1Connected = false;
bool motorController2Connected = false;
bool mobilityStatus = false;
//int8_t displayPage = 1;
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
// DAE Inverted kinematic related variables.
const float footLen = 11; // Distance from ankle to toe (foot) is 11cm. 
const float shinLen = 7.5; // Distance from knee to ankle (shin) is 7.5cm.
const float toeOffset = 17; // Angle that toe is offset from 90deg of ankle joint is 17 degrees.
// const uint8_t origXOffset = 2.92; // Distance the knee is offset from the origin along the x axis.
const float origXOffset = 2.5; // Distance the knee is offset from the origin along the x axis.
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
#define PCA9685ServoDriver3 0x42 // I2C address for third servo driver.
#define PCA9685ServoDriver4 0x43 // I2C address for fourth servo driver.
#define PCA9685ServoDriver5 0x44 // I2C address for fifth servo driver.
#define PCA9685ServoDriver6 0x45 // I2C address for sixth servo driver.
#define PCA9685ServoDriver7 0x46 // I2C address for seventh servo driver.
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
void checkBoot();
// MQTT related functions
bool connectToMqttBroker(aaNetwork &);
boolean isValidNumber(String);
uint32_t convertStrToUint32_t(String);
uint8_t convertStrToUint8_t(String);
int8_t convertStrToInt8_t(String);
bool checkNumArg(int8_t, String *);
bool processCmd(String);
IPAddress getMqttBrokerIP();
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
// Define servoLegs related functions.
void startPositionLegs(int8_t, int8_t);
void initServos();
void initLegs();
bool moveLeg(int8_t, int8_t, float, float, float);
void setupMobility();  
// Define terminal related functions.
void setupSerial();
// Define local web server related functions.
void monitorWebServer();
void startWebServer();

/*******************************************************************************
 * @section codeModules Functions put into files according to function.
 * @details Order functions here in a way that ensures that variables get 
 * declared in one function prior to being referenced by another function. 
 *******************************************************************************/



