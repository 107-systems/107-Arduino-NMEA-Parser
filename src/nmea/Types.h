/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_MTK3333_NMEA_TYPES_H_
#define ARDUINO_MTK3333_NMEA_TYPES_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <time.h>
#include <math.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef struct
{
  int hour;
  int minute;
  int second;
  int microsecond;
} Time;

typedef struct
{
  int day;
  int month;
  int year;
} Date;

enum class RmcSource
{
  Unknown, GPS, Galileo, GLONASS, GNSS
};

typedef struct
{
  bool is_valid;
  RmcSource source;
  float latitude;
  float longitude;
  float speed;
  float course;
  float magnetic_variation;
  Time time_utc;
  Date date;
} RmcData;

/**************************************************************************************
 * CONST
 **************************************************************************************/

Time    const INVALID_TIME = {-1, -1, -1, -1};
Date    const INVALID_DATE = {-1, -1, -1};
RmcData const INVALID_RMC  = {false, RmcSource::Unknown, NAN, NAN, NAN, NAN, NAN, INVALID_TIME, INVALID_DATE};

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

bool   isValid         (Date const & date);
bool   isValid         (Time const & time);
bool   isValid         (Date const & date, Time const & time);
time_t toPosixTimestamp(Date const & date, Time const & time);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_MTK3333_NMEA_TYPES_H_ */
