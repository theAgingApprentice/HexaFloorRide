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
#ifndef deviceSupport_cpp // Standard include guard. (Actually unneeded)
#define deviceSupport_cpp

#include <main.h>    // Header file for all header files plus global_variables.cpp
// main.h is the master header file. Other .cpp files should include main.h as the only included header file

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
void setupTasks()   // set up the routine that runs once per second to do timer related stuff
{
   next_oneSec_mills = millis() + 1000; // first execution is 1 second (1000 millis) from this setup execution,
                                       // and this setup is intentionally called from very end of setup()
   
   firstOneSec = true;              // first execution just gets one second intervals going
   // initialize the timers
   next_webMon_mills = millis() + period_webMon_mills;
   next_checkOled_mills = millis() + period_checkOled_mills;   
   next_checkMqtt_mills = millis() + period_checkMqtt_mills;

}
void setupOnboardLED()     // prepare do GPio 13 can control Huzzah32 onboard LED
{
   pinMode(OnboardLED,OUTPUT);     // onboard LED id hardwired to GP1013
   digitalWrite(OnboardLED,LOW);   // start out with it off
}
#endif
