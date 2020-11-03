/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_NMEA_UTIL_TIMEGM_H_
#define ARDUINO_NMEA_UTIL_TIMEGM_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <time.h>

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

time_t rk_timegm (struct tm *tm);

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_NMEA_UTIL_TIMEGM_H_ */
