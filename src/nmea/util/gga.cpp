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

bool gga_isGPGGA(char const * nmea)
{
  return (strncmp(nmea, "$GPGGA", 6) == 0);
}

bool gga_isGLGGA(char const * nmea)
{
  return (strncmp(nmea, "$GLGGA", 6) == 0);
}

bool gga_isGAGGA(char const * nmea)
{
  return (strncmp(nmea, "$GAGGA", 6) == 0);
}

bool gga_isGNGGA(char const * nmea)
{
  return (strncmp(nmea, "$GNGGA", 6) == 0);
}

bool rmc_isBDGGA(char const * nmea)
{
  return (strncmp(nmea, "$BDGGA", 6) == 0);
}


bool gga_isGxGGA(char const * nmea)
{
  return (gga_isGPGGA(nmea) || gga_isGLGGA(nmea) || gga_isGAGGA(nmea) || gga_isGNGGA(nmea) || rmc_isBDGGA(nmea));
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */
