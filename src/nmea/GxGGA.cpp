/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "GxGGA.h"

#include <string.h>

#ifdef ARDUINO_ARCH_ESP32
#  include <stdlib_noniso.h>
#endif

#ifdef ARDUINO_ARCH_RENESAS
extern "C" char * _EXFUN(strsep,(char **, const char *));
#endif

#include "util/gga.h"
#include "util/common.h"
#include "util/checksum.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void GxGGA::parse(char * gxgga, GgaData & data)
{
  ParserState state = ParserState::MessageId;

  /* Replace the '*' sign denoting the start of the checksum
   * with a ',' in order to be able to tokenize all elements
   * including the one before the checksum.
   */
  *strchr(gxgga, '*') = ',';

  for (char * token = strsep(&gxgga, ",");
       token != nullptr;
       token = strsep(&gxgga, ","))
  {
    ParserState next_state = state;

    switch(state)
    {
    case ParserState::MessageId:                     next_state = handle_MessageId                    (token, data.source);             break;
    case ParserState::UTCPositionFix:                next_state = handle_UTCPositionFix               (token, data.time_utc);           break;
    case ParserState::LatitudeVal:                   next_state = handle_LatitudeVal                  (token, data.latitude);           break;
    case ParserState::LatitudeNS:                    next_state = handle_LatitudeNS                   (token, data.latitude);           break;
    case ParserState::LongitudeVal:                  next_state = handle_LongitudeVal                 (token, data.longitude);          break;
    case ParserState::LongitudeEW:                   next_state = handle_LongitudeEW                  (token, data.longitude);          break;
    case ParserState::FixQuality:                    next_state = handle_FixQuality                   (token, data.fix_quality);        break;
    case ParserState::NumberSatellites:              next_state = handle_NumberSatellites             (token, data.num_satellites);     break;
    case ParserState::HorizontalDilutionOfPrecision: next_state = handle_HorizontalDilutionOfPrecision(token, data.hdop);               break;
    case ParserState::Altitude:                      next_state = handle_Altitude                     (token, data.altitude);           break;
    case ParserState::AltitudeUnit:                  next_state = handle_AltitudeUnit                 (token, data.altitude);           break;
    case ParserState::GeoidalSeparation:             next_state = handle_GeoidalSeparation            (token, data.geoidal_separation); break;
    case ParserState::GeoidalSeparationUnit:         next_state = handle_GeoidalSeparationUnit        (token, data.geoidal_separation); break;
    case ParserState::DGPSAge:                       next_state = handle_DGPSAge                      (token, data.dgps_age);           break;
    case ParserState::DGPSId:                        next_state = handle_DGPSId                       (token, data.dgps_id);            break;
    case ParserState::Checksum:                      next_state = handle_Checksum                     (token);                          break;
    case ParserState::Done:                                                                                                             break;
    };

    state = next_state;
  }
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

GxGGA::ParserState GxGGA::handle_MessageId(char const * token, GgaSource & source)
{
  if (util::gga_isGPGGA(token))
    source = GgaSource::GPS;
  else if (util::gga_isGLGGA(token))
    source = GgaSource::GLONASS;
  else if (util::gga_isGAGGA(token))
    source = GgaSource::Galileo;
  else if (util::gga_isGNGGA(token))
    source = GgaSource::GNSS;
  else if (util::rmc_isBDGGA(token))
    source = GgaSource::BDS;

  return ParserState::UTCPositionFix;
}

GxGGA::ParserState GxGGA::handle_UTCPositionFix(char const * token, Time & time_utc)
{
  if (strlen(token))
    util::parseTime(token, time_utc);
  else
    time_utc = INVALID_TIME;

  return ParserState::LatitudeVal;
}

GxGGA::ParserState GxGGA::handle_LatitudeVal(char const * token, float & latitude)
{
  if (strlen(token))
    latitude = util::parseLatitude(token);
  else
    latitude = NAN;

  return ParserState::LatitudeNS;
}

GxGGA::ParserState GxGGA::handle_LatitudeNS(char const * token, float & latitude)
{
  if (strlen(token) > 0  && !strncmp(token, "S", 1))
    latitude *= (-1.0f);

  return ParserState::LongitudeVal;
}

GxGGA::ParserState GxGGA::handle_LongitudeVal(char const * token, float & longitude)
{
  if (strlen(token))
    longitude = util::parseLongitude(token);
  else
    longitude = NAN;

  return ParserState::LongitudeEW;
}

GxGGA::ParserState GxGGA::handle_LongitudeEW(char const * token, float & longitude)
{
  if (strlen(token) > 0 && !strncmp(token, "W", 1))
    longitude *= (-1.0f);

  return ParserState::FixQuality;
}

GxGGA::ParserState GxGGA::handle_FixQuality(char const * token, FixQuality & fix_quality)
{
  if      (strlen(token) > 0 && !strncmp(token, "1", 1))
    fix_quality = FixQuality::GPS_Fix;
  else if (strlen(token) > 0 && !strncmp(token, "2", 1))
    fix_quality = FixQuality::DGPS_Fix;
  else
    fix_quality = FixQuality::Invalid;

  return ParserState::NumberSatellites;
}

GxGGA::ParserState GxGGA::handle_NumberSatellites(char const * token, int & num_satellites)
{
  if (strlen(token))
    num_satellites = atoi(token);
  else
    num_satellites = -1;

  return ParserState::HorizontalDilutionOfPrecision;
}

GxGGA::ParserState GxGGA::handle_HorizontalDilutionOfPrecision(char const * token, float & hdop)
{
  if (strlen(token))
    hdop = atof(token);
  else
    hdop = NAN;

  return ParserState::Altitude;
}

GxGGA::ParserState GxGGA::handle_Altitude(char const * token, float & altitude)
{
  if (strlen(token))
    altitude = atof(token);
  else
    altitude = NAN;

  return ParserState::AltitudeUnit;
}

GxGGA::ParserState GxGGA::handle_AltitudeUnit(char const * token, float & altitude)
{
  if (!strlen(token))
    altitude = NAN;

  return ParserState::GeoidalSeparation;
}

GxGGA::ParserState GxGGA::handle_GeoidalSeparation(char const * token, float & geoidal_separation)
{
  if (strlen(token))
    geoidal_separation = atof(token);
  else
    geoidal_separation = NAN;

  return ParserState::GeoidalSeparationUnit;
}

GxGGA::ParserState GxGGA::handle_GeoidalSeparationUnit(char const * token, float & geoidal_separation)
{
  if (!strlen(token))
    geoidal_separation = NAN;

  return ParserState::DGPSAge;
}

GxGGA::ParserState GxGGA::handle_DGPSAge(char const * token, int & dgps_age)
{
  if (strlen(token))
    dgps_age = atoi(token);
  else
    dgps_age = -1;

  return ParserState::DGPSId;
}

GxGGA::ParserState GxGGA::handle_DGPSId(char const * token, char * dgps_id)
{
  if (strlen(token))
    memcpy(dgps_id, token, sizeof(GgaData::dgps_id));
  else
    memset(dgps_id, 0, sizeof(GgaData::dgps_id));

  return ParserState::Checksum;
}

GxGGA::ParserState GxGGA::handle_Checksum(char const * /* token */)
{
  return ParserState::Done;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */
