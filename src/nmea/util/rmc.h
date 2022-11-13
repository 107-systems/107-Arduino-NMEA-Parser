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

bool  rmc_isGPRMC       (char const * nmea);
bool  rmc_isGLRMC       (char const * nmea);
bool  rmc_isGARMC       (char const * nmea);
bool  rmc_isGNRMC       (char const * nmea);
bool  rmc_isBDRMC       (char const * nmea);
bool  rmc_isGxRMC       (char const * nmea);
void  rmc_parseDate     (char const * token, Date & date);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */

#endif /* ARDUINO_NMEA_UTIL_RMC_H_ */
