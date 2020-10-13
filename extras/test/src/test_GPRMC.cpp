/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <string>
#include <vector>
#include <algorithm>

#include <catch.hpp>

#include <nmea/GPRMC.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

static nmea::RmcData data;

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

SCENARIO("Extracting latitude/longiture from valid GPRMC message", "[GPRMC-01]")
{
  WHEN("north/east")
  {
    std::string const GPRMC = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*45\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
    REQUIRE(data.latitude  == Approx(50.03114442));
    REQUIRE(data.longitude == Approx(19.20189679));
  }

  WHEN("north/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,4056.748,N,11212.614,W,,,290620,000.0,W*63\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
    REQUIRE(data.latitude  == Approx(40.9458060446613));
    REQUIRE(data.longitude == Approx(-112.210235595703));
  }

  WHEN("south/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,2727.069,S,05859.190,W,,,290620,000.0,W*76\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
    REQUIRE(data.latitude  == Approx(-27.4511422937699));
    REQUIRE(data.longitude == Approx(-58.986502289772));
  }

  WHEN("south/east")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,0610.522,S,10649.632,E,,,290620,000.0,W*6D\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
    REQUIRE(data.latitude  == Approx(-6.17536097471491));
    REQUIRE(data.longitude == Approx(106.827192306519));
  }
}

TEST_CASE("Extracting speed over ground from valid GPRMC message", "[GPRMC-02]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
  /* 85.7 kts ~= 44.088 m/s */
  REQUIRE(data.speed == Approx(44.088f));
}

TEST_CASE("Extracting track angle from valid GPRMC message", "[GPRMC-03]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
  REQUIRE(data.course == Approx(206.4f));
}

TEST_CASE("Extracting position time from valid GPRMC message", "[GPRMC-04]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
  /* 052856.105 -> 05:28:56.105 -> */
  REQUIRE(data.time_utc.hour        == 5);
  REQUIRE(data.time_utc.minute      == 28);
  REQUIRE(data.time_utc.second      == 56);
  REQUIRE(data.time_utc.microsecond == 105);
}

TEST_CASE("Extracting date from valid GPRMC message", "[GPRMC-05]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
  REQUIRE(data.date.day   == 8);
  REQUIRE(data.date.month == 7);
  REQUIRE(data.date.year  == 2020);
}

TEST_CASE("Extracting magnetic variation from valid GPRMC message", "[GPRMC-06]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
  REQUIRE(data.magnetic_variation == Approx(0.0));
}

TEST_CASE("Extracted status indicates void ('V') position data", "[GPRMC-07]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,V,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  data.latitude = 1.0f; data.longitude = 2.0f; data.speed = 3.0f; data.course = 4.0f;
  data.time_utc.hour = -1; data.time_utc.minute = -1; data.time_utc.second = -1; data.time_utc.microsecond = -1;

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), data) == true);
  REQUIRE(data.latitude             == 1.0f);
  REQUIRE(data.longitude            == 2.0f);
  REQUIRE(data.speed                == 3.0f);
  REQUIRE(data.course               == 4.0f);
  REQUIRE(data.time_utc.hour        == 5);
  REQUIRE(data.time_utc.minute      == 28);
  REQUIRE(data.time_utc.second      == 56);
  REQUIRE(data.time_utc.microsecond == 105);
}
