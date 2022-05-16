/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_NMEA_GXGGA_H_
#define ARDUINO_NMEA_GXGGA_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <stdint.h>

#include "Types.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class GxGGA
{

public:

  static void parse(char * gxgga, GgaData & data);

private:

  GxGGA() { }
  GxGGA(GxGGA const &) { }

  enum class ParserState : int
  {
    MessageId,
    UTCPositionFix,
    LatitudeVal,
    LatitudeNS,
    LongitudeVal,
    LongitudeEW,
    FixQuality,
    NumberSatellites,
    HorizontalDilutionOfPrecision,
    Altitude,
    AltitudeUnit,
    GeoidalSeparation,
    GeoidalSeparationUnit,
    DGPSAge,
    DGPSId,
    Checksum,
    Done
  };

  static ParserState handle_MessageId                    (char const * token, GgaSource & source);
  static ParserState handle_UTCPositionFix               (char const * token, Time & time_utc);
  static ParserState handle_LatitudeVal                  (char const * token, float & latitude);
  static ParserState handle_LatitudeNS                   (char const * token, float & latitude);
  static ParserState handle_LongitudeVal                 (char const * token, float & longitude);
  static ParserState handle_LongitudeEW                  (char const * token, float & longitude);
  static ParserState handle_FixQuality                   (char const * token, FixQuality & fix_quality);
  static ParserState handle_NumberSatellites             (char const * token, int & num_satellites);
  static ParserState handle_HorizontalDilutionOfPrecision(char const * token, float & hdop);
  static ParserState handle_Altitude                     (char const * token, float & altitude);
  static ParserState handle_AltitudeUnit                 (char const * token, float & altitude);
  static ParserState handle_GeoidalSeparation            (char const * token, float & geoidal_separation);
  static ParserState handle_GeoidalSeparationUnit        (char const * token, float & geoidal_separation);
  static ParserState handle_DGPSAge                      (char const * token, int & dgps_age);
  static ParserState handle_DGPSId                       (char const * token, char * dgps_id);
  static ParserState handle_Checksum                     (char const * token);
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_NMEA_GXGGA_H_ */
