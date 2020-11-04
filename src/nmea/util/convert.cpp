/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "convert.h"

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

extern "C" time_t rk_timegm (struct tm *tm);

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

time_t toPosixTimestamp(Date const & date, Time const & time)
{
  struct tm tm =
  {
    /* tm_sec    */ time.second + ((time.microsecond > 500) ? 1 : 0),
    /* tm_min    */ time.minute,
    /* tm_hour   */ time.hour,
    /* tm_mday   */ date.day,
    /* tm_mon    */ date.month - 1,
    /* tm_year   */ date.year - 1900,
    /* tm_wday   */ 0,
    /* tm_yday   */ 0,
    /* tm_isdst  */ 0,
#ifdef HOST
    /* tm_gmtoff */ 0,
    /* tm_zone   */ 0,
#endif
  };

  return rk_timegm(&tm);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */
