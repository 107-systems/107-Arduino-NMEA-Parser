/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "common.h"

#include <string.h>
#include <stdlib.h>

#ifdef ARDUINO_ARCH_ESP32
#  include <stdlib_noniso.h>
#endif

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

namespace util
{

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

void parseTime(char const * token, Time & time_utc)
{
  char const hour_str       [] = {token[0], token[1], '\0'};
  char const minute_str     [] = {token[2], token[3], '\0'};
  char const second_str     [] = {token[4], token[5], '\0'};
  char const microsecond_str[] = {token[7], token[8], token[9],'\0'};

  time_utc.hour        = atoi(hour_str);
  time_utc.minute      = atoi(minute_str);
  time_utc.second      = atoi(second_str);
  time_utc.microsecond = atoi(microsecond_str);
}

float parseLatitude(char const * token)
{
  char const deg_str[] = {token[0], token[1], '\0'};
  char min_str[10] = {0};
  strncpy(min_str, token + 2, sizeof(min_str));

  float latitude  = atoi(deg_str);
        latitude += atof(min_str) / 60.0f;

  return latitude;
}

float parseLongitude(char const * token)
{
  char const deg_str[] = {token[0], token[1], token[2], '\0'};
  char min_str[10] = {0};
  strncpy(min_str, token + 3, sizeof(min_str));

  float longitude  = atoi(deg_str);
        longitude += atof(min_str) / 60.0f;

  return longitude;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */
