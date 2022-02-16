/**
 **************************************************
 *
 * @file        Example1_GetPosition.ino
 *
 * @brief       Reading lat and long via UBX binary commands over SPI
 * By: Andrew Berridge
 * Date: 27th June 2021
 * License: MIT. See license file for more information but you can
 * basically do whatever you want with this code.
 *
 * This example shows how to query a u-blox module for its lat/long/altitude over SPI. We also
 * turn off the NMEA output on the SPI port. This decreases the amount of SPI traffic 
 * dramatically.
 *
 * Note: Long/lat are large numbers because they are * 10^7. To convert lat/long
 * to something google maps understands simply divide the numbers by 10,000,000. We 
 * do this so that we don't have to use floating point numbers.
 *
 * Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!
 *
 * Feel like supporting open source hardware?
 * Buy a board from SparkFun!
 * ZED-F9P RTK2: https://www.sparkfun.com/products/15136
 * NEO-M8P RTK: https://www.sparkfun.com/products/15005
 * SAM-M8Q: https://www.sparkfun.com/products/15106
 *
 * Hardware Connections:
 * Connect the U-Blox serial port to Serial1
 * If you're using a Uno or don't have a 2nd serial port (Serial1), use SoftwareSerial instead (see below)
 * Open the serial monitor at 115200 baud to see the output
 *
 *
 *              product : www.soldered.com/333099
 *              
 *              Modified by soldered.com
 * 
 * @authors     SparkFun
 ***************************************************/

#include <SPI.h> //Needed for SPI to GNSS

#include <GNSS-ZOE-M8B-SOLDERED.h> 
SFE_UBLOX_GNSS myGNSS;

// #########################################

// Instantiate an instance of the SPI class. 
// Your configuration may be different, depending on the microcontroller you are using!

#define spiPort SPI // This is the SPI port on standard Ardino boards. Comment this line if you want to use a different port.

//SPIClass spiPort (HSPI); // This is the default SPI interface on some ESP32 boards. Uncomment this line if you are using ESP32.

// #########################################

const uint8_t csPin = 10; // On ATmega328 boards, SPI Chip Select is usually pin 10. Change this to match your board.

// #########################################

long lastTime = 0; //Simple local timer. Limits amount of SPI traffic to u-blox module.

void setup()
{
  Serial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  Serial.println(F("SparkFun u-blox Example"));

  spiPort.begin(); // begin the SPI port

  //myGNSS.enableDebugging(); // Uncomment this line to see helpful debug messages on Serial

  // Connect to the u-blox module using SPI port, csPin and speed setting
  // ublox devices generally work up to 5MHz. We'll use 4MHz for this example:
  if (myGNSS.begin(spiPort, csPin, 4000000) == false) 
  {
    Serial.println(F("u-blox GNSS not detected on SPI bus. Please check wiring. Freezing."));
    while (1);
  }
  
  //myGNSS.factoryDefault(); delay(5000); // Uncomment this line to reset the module back to its factory defaults

  myGNSS.setPortOutput(COM_PORT_SPI, COM_TYPE_UBX); //Set the SPI port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
}

void loop()
{
  //Query module only every second. Doing it more often will just cause SPI traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer
    
    long latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    long altitude = myGNSS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));

    byte SIV = myGNSS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);

    Serial.println();
  }
}
