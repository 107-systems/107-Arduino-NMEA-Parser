/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "GPRMC.h"

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

bool GPRMC::parse(char const * gprmc, uint32_t & /* timestamp_fix_utc */, float & latitude, float & longitude, float & speed, float & course)
{
  ParserState state = ParserState::MessadeId;
  
  for (char * token = strtok(const_cast<char *>(gprmc), ",");
       token != nullptr;
       token = strtok(nullptr, ","))
  {
    ParserState next_state = state;

    switch(state)
    {
    case ParserState::MessadeId:         next_state = handle_MessadeId        (token);            break;
    case ParserState::UTCPositionFix:    next_state = handle_UTCPositionFix   (token);            break;
    case ParserState::Status:            next_state = handle_Status           (token);            break;
    case ParserState::LatitudeVal:       next_state = handle_LatitudeVal      (token, latitude);  break;
    case ParserState::LatitudeNS:        next_state = handle_LatitudeNS       (token, latitude);  break;
    case ParserState::LongitudeVal:      next_state = handle_LongitudeVal     (token, longitude); break;
    case ParserState::LongitudeEW:       next_state = handle_LongitudeEW      (token, longitude); break;
    case ParserState::SpeedOverGround:   next_state = handle_SpeedOverGround  (token, speed);     break;
    case ParserState::TrackAngle:        next_state = handle_TrackAngle       (token, course);    break;
    case ParserState::Checksum:          next_state = handle_Checksum         (token);            break;
    case ParserState::Done:              return true;                                             break;
    case ParserState::Error:             return false;                                            break;
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
  if(!strncmp(token, "$GPRMC", 6))
    return ParserState::UTCPositionFix;
  else
    return ParserState::Error;
}

GPRMC::ParserState GPRMC::handle_UTCPositionFix(char const * /* token */)
{
  return ParserState::Status;
}

GPRMC::ParserState GPRMC::handle_Status(char const * token)
{
  if(!strncmp(token, "A", 1))
    return ParserState::LatitudeVal;
  else
    return ParserState::Error;  
}

GPRMC::ParserState GPRMC::handle_LatitudeVal(char const * token, float & latitude)
{
  char const     deg_str[] = {token[0], token[1], '\0'};
  char const     min_str[] = {token[2], token[3], '\0'};
  char const sub_min_str[] = {token[5], token[6], token[7], '\0'};

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
  char const     deg_str[] = {token[0], token[1], token[2], '\0'};
  char const     min_str[] = {token[3], token[4], '\0'};
  char const sub_min_str[] = {token[6], token[7], token[8], '\0'};

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
  char const     speed_kts_str[] = {token[0], token[1], token[2], '\0'};
  char const sub_speed_kts_str[] = {token[4], '\0'};

  float speed_kts  = atoi(speed_kts_str);
  speed_kts       += static_cast<float>(atoi(sub_speed_kts_str)) / 10.0f;

  speed = kts_to_m_per_s(speed_kts);

  return ParserState::TrackAngle;
}

GPRMC::ParserState GPRMC::handle_TrackAngle(char const * token, float & course)
{
  char const     course_deg_str[] = {token[0], token[1], token[2], '\0'};
  char const sub_course_deg_str[] = {token[4], '\0'};

  course  = atoi(course_deg_str);
  course += static_cast<float>(atoi(sub_course_deg_str)) / 10.0f;

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
