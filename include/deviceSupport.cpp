// file: deviceSupport.cpp  Support for various hardware devices
// initially PCA9685, alias pwmdriver
//    this depends of the following include, which is near the top of main.cpp:
//        #include <Adafruit_PWMServoDriver.h> // https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.
//    also depends on this instantiation, which is further down in main.cpp
//        Adafruit_PWMServoDriver pwmDriver[numDrivers]; // Servo driver object.
// may be expanded to a bag containing centralized support for I2C buses, OLED, RGBLEG, battery measurement, sensors...
//
/*
 * @file i2c.cpp  
 * @brief File containing support for various hardware devices
 * 
 *******************************************************************************/
#ifndef deviceSupport_cpp // Standard include guard
#define deviceSupport_cpp
#endif

void setupPwmdriver()       // initialization of pmdriver which handles PCA9685 PWM controllers
{                          // this code was extracted from servoLegs.cpp which is no longer used
   #undef localRNum
   #define localRNum 12
   // the argument in the next line is actually the I2C bus address of the right side controller, = 0x40
   pwmDriver[0] =  Adafruit_PWMServoDriver(PCA9685ServoDriver1); 
   pwmDriver[0].begin();
   pwmDriver[0].setOscillatorFrequency(oscFreq);
   pwmDriver[0].setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
   delay(10);
   traceL("Initialized servo driver 0 - right side");
 
   // the argument in the next line is actually the I2C bus address of the left side controller, = 0x41
   pwmDriver[1] =  Adafruit_PWMServoDriver(PCA9685ServoDriver2); 
   pwmDriver[1].begin();
   pwmDriver[1].setOscillatorFrequency(oscFreq);
   pwmDriver[1].setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
   traceL("Initialized servo driver 1 - left side");
   delay(10);
}  // void setupPmdriver()  

// support for reading and writing to flash NVM, migrated from aaFlash.cpp library
// used for storing MQTT broker's IP adress
// two routines provided:
//    flashReadBrokerIP()
//    flashWriteBrokerIP(address)

IPAddress flashReadBrokerIP()  // read Broker's IP address from NVM 
{
   bool RW_MODE = false;
   IPAddress addrIP; // Broker IP address in String format.
   String addrStr; // Broker IP address in IPAddress format.
   preferences.begin(FLASH_APP_NAME, RW_MODE); // Open flash memory in read/write mode.
   addrStr = preferences.getString("brokerIP", DEFAULT_MQTT_IP).c_str(); // Read IP address.
   preferences.end(); // Close access to flash memory.
   addrIP.fromString(addrStr); // Convert String to IPAddress format.
   return addrIP;
}  // IPAddress flashReadBrokerIP()

void flashWriteBrokerIP(IPAddress address)   // write an new IP broker address to NVM
{
   bool RW_MODE = false;
   Serial.print("<aaFlash::write> Writing this address to flash memory: ");
   Serial.println(address);
   preferences.begin(FLASH_APP_NAME, RW_MODE); // Open flash memory in read/write mode.
   preferences.putString("brokerIP", address.toString()); // Write IP address.
   preferences.end(); // Close access to flash memory.    
}