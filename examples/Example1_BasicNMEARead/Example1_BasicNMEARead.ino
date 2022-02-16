/**
 **************************************************
 *
 * @file        Example1_BasicNMEARead.ino
 *
 * @brief       Read NMEA sentences over I2C using u-blox module SAM-M8Q, NEO-M8P, ZED-F9P, etc
 * By: Nathan Seidle
 * SparkFun Electronics
 * Date: August 22nd, 2018
 * License: MIT. See license file for more information but you can
 * basically do whatever you want with this code.
 *
 * This example reads the NMEA setences from the u-blox module over I2c and outputs
 * them to the serial port
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

#include <Wire.h> //Needed for I2C to GNSS

#include <GNSS-ZOE-M8B-SOLDERED.h>
SFE_UBLOX_GNSS myGNSS;

void setup()
{
  Serial.begin(115200);
  Serial.println("SparkFun u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false)
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX | COM_TYPE_NMEA); //Set the I2C port to output both NMEA and UBX messages
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

  //This will pipe all NMEA sentences to the serial port so we can see them
  myGNSS.setNMEAOutputPort(Serial);
}

void loop()
{
  myGNSS.checkUblox(); //See if new data is available. Process bytes as they come in.

  delay(250); //Don't pound too hard on the I2C bus
}
