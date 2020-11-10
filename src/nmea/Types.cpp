/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "Types.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

/**************************************************************************************
 * EXTERN FUNCTION DECLARATION
 **************************************************************************************/

extern "C" time_t rk_timegm (struct tm *tm);

/**************************************************************************************
 * PUBLIC FUNCTION DEFINITION
 **************************************************************************************/

bool isValid(Date const & date)
{
  if ((date.day   == INVALID_DATE.day)   ||
      (date.month == INVALID_DATE.month) ||
      (date.year  == INVALID_DATE.year))
    return false;
  else
    return true;
}

bool isValid(Time const & time)
{
  if ((time.hour        == INVALID_TIME.hour)         ||
      (time.minute      == INVALID_TIME.minute)       ||
      (time.second      == INVALID_TIME.second)       ||
      (time.microsecond == INVALID_TIME.microsecond))
    return false;
  else
    return true;
}

bool isValid(Date const & date, Time const & time)
{
  return (isValid(date) && isValid(time));
}

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

} /* nmea */
