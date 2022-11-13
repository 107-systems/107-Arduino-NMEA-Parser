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

bool rmc_isBDRMC(char const * nmea)
{
  return (strncmp(nmea, "$BDRMC", 6) == 0);
}

bool rmc_isGxRMC(char const * nmea)
{
  return (rmc_isGPRMC(nmea) || rmc_isGLRMC(nmea) || rmc_isGARMC(nmea) || rmc_isGNRMC(nmea) || rmc_isBDRMC(nmea));
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
