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

void onRmcUpdate(nmea::RmcData const);
void onGgaUpdate(nmea::GgaData const);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoNmeaParser parser(onRmcUpdate, onGgaUpdate);

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

void onRmcUpdate(nmea::RmcData const rmc)
{
  Serial.print("RMC ");

  if      (rmc.source == nmea::RmcSource::GPS)     Serial.print("GPS");
  else if (rmc.source == nmea::RmcSource::GLONASS) Serial.print("GLONASS");
  else if (rmc.source == nmea::RmcSource::Galileo) Serial.print("Galileo");
  else if (rmc.source == nmea::RmcSource::GNSS)    Serial.print("GNSS");
  else if (rmc.source == nmea::RmcSource::BDS)     Serial.print("BDS");

  Serial.print(" ");
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

void onGgaUpdate(nmea::GgaData const gga)
{
  Serial.print("GGA ");

  if      (gga.source == nmea::GgaSource::GPS)     Serial.print("GPS");
  else if (gga.source == nmea::GgaSource::GLONASS) Serial.print("GLONASS");
  else if (gga.source == nmea::GgaSource::Galileo) Serial.print("Galileo");
  else if (gga.source == nmea::GgaSource::GNSS)    Serial.print("GNSS");
  else if (gga.source == nmea::GgaSource::BDS)     Serial.print("BDS");

  Serial.print(" ");
  Serial.print(gga.time_utc.hour);
  Serial.print(":");
  Serial.print(gga.time_utc.minute);
  Serial.print(":");
  Serial.print(gga.time_utc.second);
  Serial.print(".");
  Serial.print(gga.time_utc.microsecond);

  if (gga.fix_quality != nmea::FixQuality::Invalid)
  {
    Serial.print(" : LON ");
    Serial.print(gga.longitude);
    Serial.print(" ° | LAT ");
    Serial.print(gga.latitude);
    Serial.print(" ° | Num Sat. ");
    Serial.print(gga.num_satellites);
    Serial.print(" | HDOP =  ");
    Serial.print(gga.hdop);
    Serial.print(" m | Altitude ");
    Serial.print(gga.altitude);
    Serial.print(" m | Geoidal Separation ");
    Serial.print(gga.geoidal_separation);
    Serial.print(" m");
  }

  Serial.println();
}
