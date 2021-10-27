/*******************************************************************************
 * @file main.h  
 *******************************************************************************/
#ifndef main_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define main_h // Precompiler macro used for precompiler check.

/*******************************************************************************
 * @section mainIncludes Included libraries.
 *******************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with PlatformIO.
#include <aaChip.h> // Core (CPU) details that the code running on.
#include <aaNetwork.h> // Wifi functions. 
#include <aaWebService.h> // Realtime web-based network config and OTA code updates.
#include <aaFlash.h> // Use Flash memory to store values that persist past reboot.
#include <aaMqtt.h> // Use MQTT for remote management and monitoring.
#include <known_networks.h> // String arrays of known Access Points and their passwords.
#include <InverseK.h> // https://github.com/cgxeiji/CGx-InverseK.
#include <Wire.h> // Required for I2C communication.
#include <Adafruit_PWMServoDriver.h> // https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log.
#include <Adafruit_GFX.h> // OLED graphics
#include <Adafruit_SH110X.h> // OLED text
#include <hexbot_gpio_pins.h> // GPIO pin uses

/*******************************************************************************
 * @section mainVars Global variable definitions.
 * @details In order to get rid of "error: 'xxx' was not declared in this scope"
 * compiler errors list a function from any of the include files here. 
 *******************************************************************************/
// Config related variables
const char* HOST_NAME_PREFIX = "Hexbot"; // Prefix for our unique network name.
aaChip appCpu; // Access information about the ESP32 application microprocessor (Core1).
aaNetwork network(HOST_NAME_PREFIX); // WiFi session management.
bool networkConnected = false;
bool mqttBrokerConnected = false;
bool oledConnected = false;
bool motorController1Connected = false;
bool motorController2Connected = false;
bool mobilityStatus = false;
int8_t displayPage = 1;
// MQTT related variables.
aaFlash flash; // Non-volatile memory management. 
aaMqtt mqtt; // Publish and subscribe to MQTT broker. 
IPAddress brokerIP; // IP address of the MQTT broker.
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
// DAE Inverted kinematic related variables.
const uint8_t footLen = 11; // Distance from ankle to toe (foot) is 11cm. 
const uint8_t shinLen = 7.5; // Distance from knee to ankle (shin) is 7.5cm.
const uint8_t toeOffset = 17; // Angle that toe is offset from 90deg of ankle joint is 17 degrees.
const uint8_t origXOffset = 2.92; // Distance the knee is offset from the origin along the x axis.

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
// DAE Inverted kinematic related functions.
int32_t mapDegToPWM(float *, float *);
void anglesToCoords(float, float, float, float *, float *, float *);
void coordsToAngles(float, float, float, float *, float *, float *);  

/*******************************************************************************
 * @section codeModules Functions put into files according to function.
 * @details Order functions here in a way that ensures that variables get 
 * declared in one function prior to being referenced by another function. 
 *******************************************************************************/
#include <huzzah32_gpio_pins.h> // Map pins on Adafruit Huzzah32 dev board to friendly names.
#include <hexbot_gpio_pins.h> // Map Hexbot specific pin naming to generic development board pin names. 
#include <terminal.cpp> // Serial port management.
#include <configDetails.cpp> // Show the environment details of this application.
#include <web.cpp> // Manage locally hosted web service. 
#include <rgbLed.cpp> // Control status LEDs.
#include <network.cpp> // Control networking activities.
#include <mqttBroker.cpp> // Establish connect to the the MQTT broker.
#include <i2c.cpp> // Scan I2C buses to see what devices are present.
#include <ikLibrary.cpp> // InverseK.h inverted kinematic functions.
#include <servoLegs.cpp> // Control leg servos.
#include <oled.cpp> // Control OLED.
#include <daeIK.cpp> // Doug's inverted kinetics functions. 

/************************************************************************************
 * @section mainDeclare Declare functions in main.cpp.
 ************************************************************************************/
void setup(); // Arduino mandatory function #1. Runs once at boot. 
void loop(); // Arduino mandatory function #2. Runs continually.

#endif // End of precompiler protected code block