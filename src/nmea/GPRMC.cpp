/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "GPRMC.h"

#include "Checksum.h"

#include <stdlib.h>
#include <string.h>

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

bool GPRMC::parse(char const * gprmc, float & last_fix_utc_s, float & latitude, float & longitude, float & speed, float & course)
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
    if (isChecksumToken(token))
      state = ParserState::Checksum;

    ParserState next_state = state;

    switch(state)
    {
    case ParserState::MessadeId:                  next_state = handle_MessadeId                (token);                 break;
    case ParserState::UTCPositionFix:             next_state = handle_UTCPositionFix           (token, last_fix_utc_s); break;
    case ParserState::Status:                     next_state = handle_Status                   (token);                 break;
    case ParserState::LatitudeVal:                next_state = handle_LatitudeVal              (token, latitude);       break;
    case ParserState::LatitudeNS:                 next_state = handle_LatitudeNS               (token, latitude);       break;
    case ParserState::LongitudeVal:               next_state = handle_LongitudeVal             (token, longitude);      break;
    case ParserState::LongitudeEW:                next_state = handle_LongitudeEW              (token, longitude);      break;
    case ParserState::SpeedOverGround:            next_state = handle_SpeedOverGround          (token, speed);          break;
    case ParserState::TrackAngle:                 next_state = handle_TrackAngle               (token, course);         break;
    case ParserState::Date:                       next_state = handle_Date                     (token);                 break;
    case ParserState::MagneticVariation:          next_state = handle_MagneticVariation        (token);                 break;
    case ParserState::MagneticVariationEastWest:  next_state = handle_MagneticVariationEastWest(token);                 break;
    case ParserState::Checksum:                   next_state = handle_Checksum                 (token);                 break;
    case ParserState::Done:                       return true;                                                          break;
    case ParserState::Error:                      return false;                                                         break;
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

GPRMC::ParserState GPRMC::handle_UTCPositionFix(char const * token, float & last_fix_utc_s)
{
  char const hour_str       [] = {token[0], token[1], '\0'};
  char const minute_str     [] = {token[2], token[3], '\0'};
  char const second_str     [] = {token[4], token[5], '\0'};
  char const millisecond_str[] = {token[7], token[8], token[9], '\0'};

  last_fix_utc_s  = atoi(second_str);
  last_fix_utc_s += atoi(minute_str) * 60;
  last_fix_utc_s += atoi(hour_str) * 3600;
  last_fix_utc_s += static_cast<float>(atoi(millisecond_str)) / 1000.f;

  return ParserState::Status;
}

GPRMC::ParserState GPRMC::handle_Status(char const * token)
{
  if(!strncmp(token, "A", 1))
    return ParserState::LatitudeVal;

  if(!strncmp(token, "V", 1))
    return ParserState::Done;

  return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_LatitudeVal(char const * token, float & latitude)
{
  char const deg_str[] = {token[0], token[1], '\0'};
  char const min_str[] = {token[2], token[3], '\0'};
  char sub_min_str[10] = {0};
  strncpy(sub_min_str, strrchr(token, '.') + 1, sizeof(sub_min_str));

  latitude  = atoi(deg_str);
  latitude += static_cast<float>(atoi(min_str)) / 60;
  latitude += static_cast<float>(atoi(sub_min_str)) / (60 * 1000);

  return ParserState::LatitudeNS;
}

GPRMC::ParserState GPRMC::handle_LatitudeNS(char const * token, float & latitude)
{
  if(!strncmp(token, "N", 1))
    return ParserState::LongitudeVal;

  if(!strncmp(token, "S", 1)) {
    latitude *= (-1.0f);
    return ParserState::LongitudeVal;
  }

  return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_LongitudeVal(char const * token, float & longitude)
{
  char const deg_str[] = {token[0], token[1], token[2], '\0'};
  char const min_str[] = {token[3], token[4], '\0'};
  char sub_min_str[10] = {0};
  strncpy(sub_min_str, strrchr(token, '.') + 1, sizeof(sub_min_str));

  longitude  = atoi(deg_str);
  longitude += static_cast<float>(atoi(min_str)) / 60;
  longitude += static_cast<float>(atoi(sub_min_str)) / (60 * 1000);

  return ParserState::LongitudeEW;
}

GPRMC::ParserState GPRMC::handle_LongitudeEW(char const * token, float & longitude)
{
  if(!strncmp(token, "E", 1))
    return ParserState::SpeedOverGround;

  if(!strncmp(token, "W", 1)) {
    longitude *= (-1.0f);
    return ParserState::SpeedOverGround;
  }

  return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_SpeedOverGround(char const * token, float & speed)
{
  float const speed_kts = atof(token);
  speed = kts_to_m_per_s(speed_kts);

  return ParserState::TrackAngle;
}

GPRMC::ParserState GPRMC::handle_TrackAngle(char const * token, float & course)
{
  course = atof(token);
  return ParserState::Date;
}

GPRMC::ParserState GPRMC::handle_Date(char const * /* token */)
{
  /* TODO */
  return ParserState::MagneticVariation;
}

GPRMC::ParserState GPRMC::handle_MagneticVariation(char const * /* token */)
{
  /* TODO */
  return ParserState::MagneticVariationEastWest;
}

GPRMC::ParserState GPRMC::handle_MagneticVariationEastWest(char const * /* token */)
{
  /* TODO */
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
