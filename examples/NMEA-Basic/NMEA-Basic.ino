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

#include <Parser.h>

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

void onPositionUpdate(float const last_fix_utc_s, float const latitude, float const longitude, float const speed, float const course);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

nmea::Parser parser(onPositionUpdate);

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

void onPositionUpdate(float const last_fix_utc_s, float const latitude, float const longitude, float const speed, float const course)
{
  char msg[64] = {0};
  snprintf(msg, 64, "[%f] %f LON | %f LAT | %d m/s | %d °", last_fix_utc_s, latitude, longitude, speed, course);
  Serial.println(msg);
}
