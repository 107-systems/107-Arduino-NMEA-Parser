/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "GxRMC.h"

#include <string.h>

#ifdef ARDUINO_ARCH_ESP32
#  include <stdlib_noniso.h>
#endif

#ifdef ARDUINO_ARCH_RENESAS
extern "C" char * _EXFUN(strsep,(char **, const char *));
#endif

#include "util/rmc.h"
#include "util/common.h"
#include "util/checksum.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

/**************************************************************************************
 * CONSTEXPR
 **************************************************************************************/

constexpr float kts_to_m_per_s(float const v) { return (v / 1.9438444924574f); }

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void GxRMC::parse(char * gxrmc, RmcData & data)
{
  ParserState state = ParserState::MessageId;

  /* Replace the '*' sign denoting the start of the checksum
   * with a ',' in order to be able to tokenize all elements
   * including the one before the checksum.
   */
  *strchr(gxrmc, '*') = ',';

  for (char * token = strsep(&gxrmc, ",");
       token != nullptr;
       token = strsep(&gxrmc, ","))
  {
    ParserState next_state = state;

    switch(state)
    {
    case ParserState::MessageId:                  next_state = handle_MessageId                (token, data.source);             break;
    case ParserState::UTCPositionFix:             next_state = handle_UTCPositionFix           (token, data.time_utc);           break;
    case ParserState::Status:                     next_state = handle_Status                   (token, data.is_valid);           break;
    case ParserState::LatitudeVal:                next_state = handle_LatitudeVal              (token, data.latitude);           break;
    case ParserState::LatitudeNS:                 next_state = handle_LatitudeNS               (token, data.latitude);           break;
    case ParserState::LongitudeVal:               next_state = handle_LongitudeVal             (token, data.longitude);          break;
    case ParserState::LongitudeEW:                next_state = handle_LongitudeEW              (token, data.longitude);          break;
    case ParserState::SpeedOverGround:            next_state = handle_SpeedOverGround          (token, data.speed);              break;
    case ParserState::TrackAngle:                 next_state = handle_TrackAngle               (token, data.course);             break;
    case ParserState::Date:                       next_state = handle_Date                     (token, data.date);               break;
    case ParserState::MagneticVariation:          next_state = handle_MagneticVariation        (token, data.magnetic_variation); break;
    case ParserState::MagneticVariationEastWest:  next_state = handle_MagneticVariationEastWest(token, data.magnetic_variation); break;
    case ParserState::Checksum:                   next_state = handle_Checksum                 (token);                          break;
    case ParserState::Done:                                                                                                      break;
    };

    state = next_state;
  }
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

GxRMC::ParserState GxRMC::handle_MessageId(char const * token, RmcSource & source)
{
  if (util::rmc_isGPRMC(token))
    source = RmcSource::GPS;
  else if (util::rmc_isGLRMC(token))
    source = RmcSource::GLONASS;
  else if (util::rmc_isGARMC(token))
    source = RmcSource::Galileo;
  else if (util::rmc_isGNRMC(token))
    source = RmcSource::GNSS;
  else if (util::rmc_isBDRMC(token))
    source = RmcSource::BDS;

  return ParserState::UTCPositionFix;
}

GxRMC::ParserState GxRMC::handle_UTCPositionFix(char const * token, Time & time_utc)
{
  if (strlen(token))
    util::parseTime(token, time_utc);
  else
    time_utc = INVALID_TIME;

  return ParserState::Status;
}

GxRMC::ParserState GxRMC::handle_Status(char const * token, bool & is_valid)
{
  if(strlen(token) > 0 && !strncmp(token, "A", 1))
    is_valid = true;
  else
    is_valid = false;

  return ParserState::LatitudeVal;
}

GxRMC::ParserState GxRMC::handle_LatitudeVal(char const * token, float & latitude)
{
  if (strlen(token))
    latitude = util::parseLatitude(token);
  else
    latitude = NAN;

  return ParserState::LatitudeNS;
}

GxRMC::ParserState GxRMC::handle_LatitudeNS(char const * token, float & latitude)
{
  if (strlen(token) > 0  && !strncmp(token, "S", 1))
    latitude *= (-1.0f);

  return ParserState::LongitudeVal;
}

GxRMC::ParserState GxRMC::handle_LongitudeVal(char const * token, float & longitude)
{
  if (strlen(token))
    longitude = util::parseLongitude(token);
  else
    longitude = NAN;

  return ParserState::LongitudeEW;
}

GxRMC::ParserState GxRMC::handle_LongitudeEW(char const * token, float & longitude)
{
  if (strlen(token) > 0 && !strncmp(token, "W", 1))
    longitude *= (-1.0f);

  return ParserState::SpeedOverGround;
}

GxRMC::ParserState GxRMC::handle_SpeedOverGround(char const * token, float & speed)
{
  if (strlen(token))
    speed = kts_to_m_per_s(atof(token));
  else
    speed = NAN;

  return ParserState::TrackAngle;
}

GxRMC::ParserState GxRMC::handle_TrackAngle(char const * token, float & course)
{
  if (strlen(token))
    course = atof(token);
  else
    course = NAN;

  return ParserState::Date;
}

GxRMC::ParserState GxRMC::handle_Date(char const * token, Date & date)
{
  if (strlen(token))
    util::rmc_parseDate(token, date);
  else
    date = INVALID_DATE;

  return ParserState::MagneticVariation;
}

GxRMC::ParserState GxRMC::handle_MagneticVariation(char const * token, float & magnetic_variation)
{
  if (strlen(token) > 0)
    magnetic_variation = atof(token);
  else
    magnetic_variation = NAN;

  return ParserState::MagneticVariationEastWest;
}

GxRMC::ParserState GxRMC::handle_MagneticVariationEastWest(char const * token, float & magnetic_variation)
{
  if(!strncmp(token, "W", 1))
    magnetic_variation *= (-1.0f);

  return ParserState::Checksum;
}

GxRMC::ParserState GxRMC::handle_Checksum(char const * /* token */)
{
  return ParserState::Done;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */
