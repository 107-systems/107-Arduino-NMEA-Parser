/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_MTK3333_NMEA_TYPES_H_
#define ARDUINO_MTK3333_NMEA_TYPES_H_

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

typedef struct
{
  bool  is_valid;
  float latitude;
  float longitude;
  float speed;
  float course;
  float magnetic_variation;
  Time time_utc;
  Date date;
} RmcData;

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_MTK3333_NMEA_TYPES_H_ */
