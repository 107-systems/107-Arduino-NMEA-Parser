/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "gga.h"

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

bool rmc_isGPGGA(char const * nmea)
{
  return (strncmp(nmea, "$GPGGA", 6) == 0);
}

bool rmc_isGLGGA(char const * nmea)
{
  return (strncmp(nmea, "$GLGGA", 6) == 0);
}

bool rmc_isGAGGA(char const * nmea)
{
  return (strncmp(nmea, "$GAGGA", 6) == 0);
}

bool rmc_isGNGGA(char const * nmea)
{
  return (strncmp(nmea, "$GNGGA", 6) == 0);
}

bool rmc_isGxGGA(char const * nmea)
{
  return (rmc_isGPGGA(nmea) || rmc_isGLGGA(nmea) || rmc_isGAGGA(nmea) || rmc_isGNGGA(nmea));
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */
