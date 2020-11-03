/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <nmea/util/convert.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("Convert date/time to POSIX timestamp with time.microseconds > 500", "[convert-01]")
{
  nmea::Date const date = {29,10,2020};
  nmea::Time const time = {13,37,25,689};

  time_t const posix_timestamp = nmea::util::toPosixTimestamp(date, time);

  REQUIRE(posix_timestamp == 1603978646);
}

TEST_CASE("Convert date/time to POSIX timestamp with time.microseconds < 500", "[convert-02]")
{
  nmea::Date const date = {29,10,2020};
  nmea::Time const time = {13,37,25,322};

  time_t const posix_timestamp = nmea::util::toPosixTimestamp(date, time);

  REQUIRE(posix_timestamp == 1603978646 - 1);
}
