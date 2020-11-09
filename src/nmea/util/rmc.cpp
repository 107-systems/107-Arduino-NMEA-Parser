/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "rmc.h"

#include <string.h>
#include <stdlib.h>

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

bool rmc_isGPRMC(char const * nmea)
{
  return (strncmp(nmea, "$GPRMC", 6) == 0);
}

bool rmc_isGLRMC(char const * nmea)
{
  return (strncmp(nmea, "$GLRMC", 6) == 0);
}

bool rmc_isGARMC(char const * nmea)
{
  return (strncmp(nmea, "$GARMC", 6) == 0);
}

bool rmc_isGNRMC(char const * nmea)
{
  return (strncmp(nmea, "$GNRMC", 6) == 0);
}

bool rmc_isGxRMC(char const * nmea)
{
  return (rmc_isGPRMC(nmea) || rmc_isGLRMC(nmea) || rmc_isGARMC(nmea) || rmc_isGNRMC(nmea));
}

void rmc_parseTime(char const * token, Time & time_utc)
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

float rmc_parseLatitude(char const * token)
{
  char const deg_str[] = {token[0], token[1], '\0'};
  char min_str[10] = {0};
  strncpy(min_str, token + 2, sizeof(min_str));

  float latitude  = atoi(deg_str);
        latitude += atof(min_str) / 60.0f;

  return latitude;
}

float rmc_parseLongitude(char const * token)
{
  char const deg_str[] = {token[0], token[1], token[2], '\0'};
  char min_str[10] = {0};
  strncpy(min_str, token + 3, sizeof(min_str));

  float longitude  = atoi(deg_str);
        longitude += atof(min_str) / 60.0f;

  return longitude;
}

void rmc_parseDate(char const * token, Date & date)
{
  char const day_str  [] = {token[0], token[1], '\0'};
  char const month_str[] = {token[2], token[3], '\0'};
  char const year_str [] = {token[4], token[5], '\0'};

  date.day   = atoi(day_str);
  date.month = atoi(month_str);
  date.year  = 2000 + atoi(year_str);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */
