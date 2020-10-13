/**
 * This software is distributed under the terms of the LGPL 3.0 License.
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
  int day;
  int month;
  int year;
} Date;

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_MTK3333_NMEA_TYPES_H_ */
