/*
 * This example demonstrates how to use the 107-Arduino-NMEA library
 * to parse the incoming NMEA messages and make use of the extracted
 * information.
 *
 * Hardware:
 *   - Arduino MKR Family Board, e.g. MKR VIDOR 4000
 *   - Adafruit Mini GPS PA1010D Module
 *
 * Electrical Connection:
 *   - GPS Module VIN <->      VCC MKR VIDOR 4000
 *   - GPS Module GND <->      GND MKR VIDOR 4000
 *   - GPS Module TXO <-> (13) RX  MKR VIDOR 4000
 *   - GPS Module RXI <-> (14) TX  MKR VIDOR 4000
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <ArduinoNmeaParser.h>

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

void onGprmcUpdate(nmea::RmcData const);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoNmeaParser parser(onGprmcUpdate);

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop()
{
  while (Serial1.available()) {
    parser.encode((char)Serial1.read());
  }
}

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

void onGprmcUpdate(nmea::RmcData const rmc)
{
  Serial.print(rmc.time_utc.hour);
  Serial.print(":");
  Serial.print(rmc.time_utc.minute);
  Serial.print(":");
  Serial.print(rmc.time_utc.second);
  Serial.print(".");
  Serial.print(rmc.time_utc.microsecond);

  if (rmc.is_valid)
  {
    Serial.print(" : LON ");
    Serial.print(rmc.longitude);
    Serial.print(" ° | LAT ");
    Serial.print(rmc.latitude);
    Serial.print(" ° | VEL ");
    Serial.print(rmc.speed);
    Serial.print(" m/s | HEADING ");
    Serial.print(rmc.course);
    Serial.print(" °");
  }

  Serial.println();
}
