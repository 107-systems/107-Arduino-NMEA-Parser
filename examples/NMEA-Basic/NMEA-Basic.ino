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

void onPositionUpdate(nmea::Time const &, float const latitude, float const longitude, float const speed, float const course);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoNmeaParser parser(onPositionUpdate);

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

void onPositionUpdate(nmea::Time const & time, float const latitude, float const longitude, float const speed, float const course)
{
  Serial.print(time.hour);
  Serial.print(":");
  Serial.print(time.minute);
  Serial.print(":");
  Serial.print(time.second);
  Serial.print(".");
  Serial.print(time.microsecond);

  Serial.print(" : LON ");
  Serial.print(longitude);
  Serial.print(" ° | LAT ");
  Serial.print(latitude);
  Serial.print(" ° | VEL ");
  Serial.print(speed);
  Serial.print(" m/s | HEADING ");
  Serial.print(course);
  Serial.print(" °");

  Serial.println();
}
