![hexaBot](img/hexaFloorRideBanner.png)

[![LICENSE](https://img.shields.io/badge/license-MIT-lightgrey.svg)](https://raw.githubusercontent.com/mmistakes/minimal-mistakes/master/LICENSE)

This repository contains the Arduino source code that runs on an embedded processor at the heart of the six legged robot we call HexaFloorRide. The goals of this project can be found [here](goals.md).  

# Getting Started

This project requires that you have a HexaFloorRide robot. The Robot is made up of a custom robot chassis and PCB as well as the code in this repository. Without a HexaFloorRide robot this code will not be very useful to you. 

# Documentation

Here is an index of links to helpful documents found in this repository.

## Constructing a HexaFloorRide Robot
* List of [hardware components](/docs/hexaFloorRideHardware.md).
* HexaFloorRide [assembly instructions](/docs/hexaFloorRideAssembly.md).
* How to [wire the motors](/physicalDesign/otherDesignInfo/legRefenceNamesAndHomePositions.pdf).

## HexaFloorRide User Guides
* HexaFloorRide's [Web interface](/docs/hexaFloorRideWebInterface.md).
* How to [configure NVRAM remotely](/docs/hexaFloorRideWebInterface.md#config-updater-screen).
* How to [perform OTA updates](/docs/hexaFloorRideWebInterface.md#ota-updater-screen).
* Available [MQTT commands](/docs/mqttCommands.md).
* [HexaFloorRide Operations manual](/docs/hexaFloorRideOperationManual.md).

## HexaFloorRide Developers Guide

### Prerequisites

In order to make use of the code in this repository you will require the following items.

* A HexaFloorRide robot made up of [this hardware](/docs/hexaFloorRideHardware.md).
* The [HexaFloorRide custom PCB](/docs/hexaFloorRideCircuit.md).
* An [MQTT broker](/docs/MQTTbroker.md) used by the robot's [MQTT web interface](/docs/hexaFloorRideWebInterface.md).  
* A [clone](/docs/cloneRepository.md) of the HexaFloorRide gitHub repository into your own local Git repository. To learn how we set up our directory structure please refer to [this document](https://va3wam.github.io/versionControl/).

### Development Software Stack

* How to [clone](/docs/cloneRepository.md) the HexaFloorRide gitHub repository.
* Sample [platformio.ini](https://github.com/theAgingApprentice/HexaFloorRide/tree/main/aaAdmin/platformioAdmin) template.
* Set up [PlatformIO](https://github.com/theAgingApprentice/HexaFloorRide/blob/main/docs/cloneRepository.md#set-up-platformio).
* Recommended Visual Studio Code [plugins](https://github.com/theAgingApprentice/HexaFloorRide/blob/main/docs/cloneRepository.md#visual-studio-code-plugins).

### Code Compatability

HexaFloorRide software runs as a monolithic firmware image. The high level logic and peripheral interactions are handled using Arduino C++. FreeRTOS is used to run a WiFi stack as well as the primitive xTaskCreatePinnedToCore() which is used to manage mutli-threaded process execution.

HexaFloorRide's firmware is intended to run on the Espressif ESP WROOM32 "Software On a Chip" (SOC). The development board used for this SOC is the [Adafruit Huzzah32 Feather Board.](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather). Note that some standard Arduino functions such as AnalogWrite() have not been implemented on the ESP32 SOC and alternate functions such as ledcSetup(), ledcAttachPin() and ledcWrite() must be used in their place. There are some additional compatability issues of note as follows:

* This code will NOT run on an ESP8266 unmodified because of both GPIO incompatability as well as some ESP32 specific libraries that are in use.

### Installing

A set of instructions on how to install the source code for theAgingApprentice projects can be found [here](https://va3wam.github.io/versionControl/).

### Testing

At this time we have no tools or standards for automated testing of Arduino code on an embedded system. 

### Deployment

Use serial over USB to load the inital code onto the embedded system. Subsequent builds can be uploaded using either serial over USB or 
a [OTA web interface](/docs/hexaFloorRideWebInterface.md#ota-updater-screen) hosted by the robot which allows you to do [OTA](https://en.wikipedia.org/wiki/Over-the-air_programming) updates.

### Built With

* [Visual Studio Code](https://code.visualstudio.com/) - Text editor.
* [PlatformIO](https://platformio.org/) - IDE for ESP32 based Arduino development.

If you are new to these tools then you may want to read [this](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/).

### Contributing

Please read [CONTRIBUTING.md](contributing.md) for details on our code
of conduct, and the process for submitting pull requests to us.

### Versioning

We use [Semantic Versioning](http://semver.org/) in naming the [releases](https://github.com/theAgingApprentice/HexaFloorRide/releases) of this code base. 

# Kinematic Model

HexaFloorRide uses an inverse kinematic model to manipulate it's legs. To learn more about our implementation of an inverse kinematic model look <a href="/docs/kinematicModel.md">here</a>.

# Authors

* **[Old Squire](https://github.com/theagingapprentice)**
* **[Doug Elliott](https://github.com/nerdoug)**

# License

This project is licensed under the [MIT License](license.md).

# Acknowledgments

* Robot chassis design inspired by [Cameron Buss' Swampy the Hexapod](https://grabcad.com/library/swampy-the-hexapod-1).
* 3 DOF Inverse Kinematic code inspired by [Avatar
Aymen Nacer](https://github.com/AymenNacer/Forward-and-Inverse-Kinematics-for-3-DOF-Robotic-arm). 
* Tony DiCola and Adafruit Industries for their MQTT QOS1 Arduino library.
* Jeff Rowberg for the MPU6050 DMP logic. ```May not use.```
* DFRobot & John Rickman for their LiquidCrystal I2C LCD display Arduino library. ```May not use.```
* The many folks involved in porting the Arduino libraries over to the ESP32.
