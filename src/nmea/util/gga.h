/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_NMEA_UTIL_GGA_H_
#define ARDUINO_NMEA_UTIL_GGA_H_

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "../Types.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

namespace util
{

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

bool  gga_isGPGGA(char const * nmea);
bool  gga_isGLGGA(char const * nmea);
bool  gga_isGAGGA(char const * nmea);
bool  gga_isGNGGA(char const * nmea);
bool  rmc_isBDGGA(char const * nmea);
bool  gga_isGxGGA(char const * nmea);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */

#endif /* ARDUINO_NMEA_UTIL_GGA_H_ */
