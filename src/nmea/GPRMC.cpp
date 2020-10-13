/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "GPRMC.h"

#include <math.h>
#include <string.h>

#include "util/gprmc.h"
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

bool GPRMC::isGPRMC(char const * nmea)
{
  return (strncmp(nmea, "$GPRMC", 6) == 0);
}
bool GPRMC::parse(char const * gprmc, RmcData & data)
{
  ParserState state = ParserState::MessadeId;

  for (char * token = strsep((char **)&gprmc, ",");
       token != nullptr;
       token = strsep((char **)&gprmc, ","))
  {
    /* All GPS receivers should at least implement the the fields: UTC Position Fix,
     * Status, Latitude, Longitude, Speed over ground, Track Angle. All other fields
     * are optional. Therefore we are checking in the following if statement if the
     * current token is a checksum token. If that's the case we are directly jumping
     * to ParserState::Checksum.
     */
    if (util::isChecksumToken(token))
      state = ParserState::Checksum;

    ParserState next_state = state;

    switch(state)
    {
    case ParserState::MessadeId:                  next_state = handle_MessadeId                (token);                          break;
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
    case ParserState::Done:                       return true;                                                                   break;
    case ParserState::Error:                      return false;                                                                  break;
    };

    state = next_state;
  }

  return (state == ParserState::Done);
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

GPRMC::ParserState GPRMC::handle_MessadeId(char const * token)
{
  if(isGPRMC(token))
    return ParserState::UTCPositionFix;
  else
    return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_UTCPositionFix(char const * token, Time & time_utc)
{
  if (strlen(token))
    util::gprmc_parseTime(token, time_utc);
  else
  {
    time_utc.hour = -1;
    time_utc.minute = -1;
    time_utc.second = -1;
    time_utc.microsecond = -1;
  }

  return ParserState::Status;
}

GPRMC::ParserState GPRMC::handle_Status(char const * token, bool & is_valid)
{
  is_valid = false;

  if (strlen(token) == 0)
    return ParserState::Error;

  if(!strncmp(token, "A", 1)) {
    is_valid = true;
    return ParserState::LatitudeVal;
  }

  if(!strncmp(token, "V", 1))
    return ParserState::Done;

  return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_LatitudeVal(char const * token, float & latitude)
{
  if (strlen(token))
    latitude = util::gprmc_parseLatitude(token);
  else
    latitude = NAN;

  return ParserState::LatitudeNS;
}

GPRMC::ParserState GPRMC::handle_LatitudeNS(char const * token, float & latitude)
{
  if (strlen(token))
  {
    if(!strncmp(token, "N", 1))
      return ParserState::LongitudeVal;

    if(!strncmp(token, "S", 1)) {
      latitude *= (-1.0f);
      return ParserState::LongitudeVal;
    }
  }

  return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_LongitudeVal(char const * token, float & longitude)
{
  if (strlen(token))
    longitude = util::gprmc_parseLongitude(token);
  else
    longitude = NAN;

  return ParserState::LongitudeEW;
}

GPRMC::ParserState GPRMC::handle_LongitudeEW(char const * token, float & longitude)
{
  if (strlen(token))
  {
    if(!strncmp(token, "E", 1))
      return ParserState::SpeedOverGround;

    if(!strncmp(token, "W", 1)) {
      longitude *= (-1.0f);
      return ParserState::SpeedOverGround;
    }
  }

  return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_SpeedOverGround(char const * token, float & speed)
{
  if (strlen(token))
    speed = kts_to_m_per_s(atof(token));
  else
    speed = NAN;

  return ParserState::TrackAngle;
}

GPRMC::ParserState GPRMC::handle_TrackAngle(char const * token, float & course)
{
  if (strlen(token))
    course = atof(token);
  else
    course = NAN;

  return ParserState::Date;
}

GPRMC::ParserState GPRMC::handle_Date(char const * token, Date & date)
{
  if (strlen(token))
    util::gprmc_parseDate(token, date);
  else
  {
    date.day = -1;
    date.month = -1;
    date.year = -1;
  }

  return ParserState::MagneticVariation;
}

GPRMC::ParserState GPRMC::handle_MagneticVariation(char const * token, float & magnetic_variation)
{
  if (strlen(token) > 0)
    magnetic_variation = atof(token);
  else
    magnetic_variation = NAN;

  return ParserState::MagneticVariationEastWest;
}

GPRMC::ParserState GPRMC::handle_MagneticVariationEastWest(char const * token, float & magnetic_variation)
{
  if(!strncmp(token, "W", 1))
    magnetic_variation *= (-1.0f);

  return ParserState::Checksum;
}

GPRMC::ParserState GPRMC::handle_Checksum(char const * /* token */)
{
  return ParserState::Done;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */
