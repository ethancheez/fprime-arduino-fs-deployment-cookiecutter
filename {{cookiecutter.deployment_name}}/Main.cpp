// ======================================================================
// \title  Main.cpp
// \brief main program for the F' application. Intended for Arduino-based systems
//
// ======================================================================
// Used to access topology functions
#include <{{cookiecutter.deployment_name}}/Top/{{cookiecutter.deployment_name}}TopologyAc.hpp>
#include <{{cookiecutter.deployment_name}}/Top/{{cookiecutter.deployment_name}}Topology.hpp>

// Used for Baremetal TaskRunner
#include <fprime-baremetal/Os/TaskRunner/TaskRunner.hpp>

// Used for logging
#include <Arduino/Os/Console.hpp>

// Used for SD File System
#include <SD.h>

/**
 * \brief setup the program
 *
 * This is an extraction of the Arduino setup() function.
 * 
 */
void setup() {
    // Initialize OSAL
    Os::init();

    // Setup Serial and Logging
    Serial.begin(115200);
    static_cast<Os::Arduino::StreamConsoleHandle*>(Os::Console::getSingleton().getHandle())->setStreamHandler(Serial);

    // Setup SD Card.
    // If not using the built-in SD card (or if there is no built-in SD card slot),
    // change BUILTIN_SDCARD to the SPI chip select pin of the SD card.
    SD.begin(BUILTIN_SDCARD);

    // Object for communicating state to the reference topology
    {{cookiecutter.deployment_name}}::TopologyState inputs;
    inputs.uartNumber = 0;
    inputs.uartBaud = 115200;

    // Setup topology
    {{cookiecutter.deployment_name}}::setupTopology(inputs);

    Fw::Logger::log("Program Started\n");
}

/**
 * \brief run the program
 *
 * This is an extraction of the Arduino loop() function.
 * 
 */
void loop() {
#ifdef USE_BASIC_TIMER
    rateDriver.cycle();
#endif
    Os::Baremetal::TaskRunner::getSingleton().run();
}