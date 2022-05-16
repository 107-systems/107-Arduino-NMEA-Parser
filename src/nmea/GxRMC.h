/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_MTK3333_NMEA_GPRMC_PARSER_H_
#define ARDUINO_MTK3333_NMEA_GPRMC_PARSER_H_

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

class GxRMC
{

public:

  static void parse(char * gxrmc, RmcData & data);

private:

  GxRMC() { }
  GxRMC(GxRMC const &) { }

  enum class ParserState : int
  {
    MessageId,
    UTCPositionFix,
    Status,
    LatitudeVal,
    LatitudeNS,
    LongitudeVal,
    LongitudeEW,
    SpeedOverGround,
    TrackAngle,
    Date,
    MagneticVariation,
    MagneticVariationEastWest,
    Checksum,
    Done
  };

  static ParserState handle_MessageId                (char const * token, RmcSource & source);
  static ParserState handle_UTCPositionFix           (char const * token, Time & time_utc);
  static ParserState handle_Status                   (char const * token, bool & is_valid);
  static ParserState handle_LatitudeVal              (char const * token, float & latitude);
  static ParserState handle_LatitudeNS               (char const * token, float & latitude);
  static ParserState handle_LongitudeVal             (char const * token, float & longitude);
  static ParserState handle_LongitudeEW              (char const * token, float & longitude);
  static ParserState handle_SpeedOverGround          (char const * token, float & speed);
  static ParserState handle_TrackAngle               (char const * token, float & course);
  static ParserState handle_Date                     (char const * token, Date & date);
  static ParserState handle_MagneticVariation        (char const * token, float & magnetic_variation);
  static ParserState handle_MagneticVariationEastWest(char const * token, float & magnetic_variation);
  static ParserState handle_Checksum                 (char const * token);
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_MTK3333_NMEA_GPRMC_PARSER_H_ */
