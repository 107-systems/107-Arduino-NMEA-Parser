/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_NMEA_UTIL_RMC_H_
#define ARDUINO_NMEA_UTIL_RMC_H_

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

void  gprmc_parseTime      (char const * token, Time & time_utc);
float gprmc_parseLatitude  (char const * token);
float gprmc_parseLongitude (char const * token);
void  gprmc_parseDate      (char const * token, Date & date);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */

#endif /* ARDUINO_NMEA_UTIL_RMC_H_ */
