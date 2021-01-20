/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_NMEA_UTIL_CHECKSUM_H_
#define ARDUINO_NMEA_UTIL_CHECKSUM_H_

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

bool isChecksumOk(char const * const nmea_str);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */

#endif /* ARDUINO_NMEA_UTIL_CHECKSUM_H_ */
