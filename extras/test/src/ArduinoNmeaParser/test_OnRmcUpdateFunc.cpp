/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <string.h>

#include <string>
#include <algorithm>

#include <catch.hpp>

#include <ArduinoNmeaParser.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

static bool on_rmc_update_called = false;

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

static void encode(ArduinoNmeaParser & parser, std::string const & nmea)
{
  std::for_each(std::begin(nmea),
                std::end(nmea),
                [&parser](char const c)
                {
                  parser.encode(c);
                });
}

void onRmcUpdate(nmea::RmcData const data)
{
  on_rmc_update_called = true;

  REQUIRE(data.source == nmea::RmcSource::GPS);

  REQUIRE(data.time_utc.hour        == 5);
  REQUIRE(data.time_utc.minute      == 28);
  REQUIRE(data.time_utc.second      == 56);
  REQUIRE(data.time_utc.microsecond == 105);

  REQUIRE(data.is_valid == true);

  REQUIRE(data.latitude  == Approx(52.514567));
  REQUIRE(data.longitude == Approx(13.350933));

  REQUIRE(data.speed == Approx(44.088f));
  REQUIRE(data.course == Approx(206.4f));

  REQUIRE(data.date.day   == 8);
  REQUIRE(data.date.month == 7);
  REQUIRE(data.date.year  == 2020);

  REQUIRE(data.magnetic_variation == Approx(0.0));
}

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("Testing execution of OnRmcUpdateFunc when a full GxRMC message has been received", "[OnRmcUpdateFunc-01]")
{
  ArduinoNmeaParser parser(onRmcUpdate, nullptr);
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");
  encode(parser, GPRMC);
  REQUIRE(on_rmc_update_called == true);
}
