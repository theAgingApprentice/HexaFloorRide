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