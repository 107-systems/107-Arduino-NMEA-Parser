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

#include <nmea/Types.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("Convert date/time to POSIX timestamp with time.microseconds > 500", "[toPosixTimestamp-01]")
{
  nmea::Date const date = {29,10,2020};
  nmea::Time const time = {13,37,25,689};

  time_t const posix_timestamp = nmea::toPosixTimestamp(date, time);

  REQUIRE(posix_timestamp == 1603978646);
}

TEST_CASE("Convert date/time to POSIX timestamp with time.microseconds < 500", "[toPosixTimestamp-02]")
{
  nmea::Date const date = {29,10,2020};
  nmea::Time const time = {13,37,25,322};

  time_t const posix_timestamp = nmea::toPosixTimestamp(date, time);

  REQUIRE(posix_timestamp == 1603978646 - 1);
}

TEST_CASE("Testing isValid(Date const &)", "[isValid(Date)-01]")
{
  WHEN ("Date is valid")
  {
    nmea::Date const date = {29,10,2020};
    REQUIRE(nmea::isValid(date) == true);
  }
  WHEN ("Date.day is invalid")
  {
    nmea::Date const date = {-1,10,2020};
    REQUIRE(nmea::isValid(date) == false);
  }
  WHEN ("Date.month is invalid")
  {
    nmea::Date const date = {29,-1,2020};
    REQUIRE(nmea::isValid(date) == false);
  }
  WHEN ("Date.year is invalid")
  {
    nmea::Date const date = {29,10,-1};
    REQUIRE(nmea::isValid(date) == false);
  }
}

TEST_CASE("Testing isValid(Time const &)", "[isValid(Time)-01]")
{
  WHEN ("Time is valid")
  {
    nmea::Time const time = {13,37,25,322};
    REQUIRE(nmea::isValid(time) == true);
  }
  WHEN ("Time.hour is invalid")
  {
    nmea::Time const time = {-1,37,25,322};
    REQUIRE(nmea::isValid(time) == false);
  }
  WHEN ("Time.minute is invalid")
  {
    nmea::Time const time = {13,-1,25,322};
    REQUIRE(nmea::isValid(time) == false);
  }
  WHEN ("Time.second is invalid")
  {
    nmea::Time const time = {13,37,-1,322};
    REQUIRE(nmea::isValid(time) == false);
  }
  WHEN ("Time.microsecond is invalid")
  {
    nmea::Time const time = {13,37,25,-1};
    REQUIRE(nmea::isValid(time) == false);
  }
}

TEST_CASE("Testing isValid(Date const &, Time const &)", "[isValid(Date,Time)-01]")
{
  WHEN ("Both date and time are valid")
  {
    nmea::Date const date = {29,10,2020};
    nmea::Time const time = {13,37,25,322};
    REQUIRE(nmea::isValid(date, time) == true);
  }
  WHEN ("Date is invalid")
  {
    nmea::Date const date = {-1,10,2020};
    nmea::Time const time = {13,37,25,322};
    REQUIRE(nmea::isValid(date, time) == false);
  }
  WHEN ("Time is invalid")
  {
    nmea::Date const date = {29,10,2020};
    nmea::Time const time = {-1,37,25,322};
    REQUIRE(nmea::isValid(date, time) == false);
  }
  WHEN ("Both date and time are invalid")
  {
    nmea::Date const date = {-1,10,2020};
    nmea::Time const time = {-1,37,25,322};
    REQUIRE(nmea::isValid(date, time) == false);
  }
}
