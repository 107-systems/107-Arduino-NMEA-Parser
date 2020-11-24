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

#include <nmea/GxRMC.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

static nmea::RmcData data;

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

SCENARIO("Extracting latitude/longiture from valid GPRMC message", "[GxRMC-01]")
{
  WHEN("north/east")
  {
    std::string const GPRMC = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*45\r\n";

    nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

    REQUIRE(data.latitude  == Approx(50.03114442));
    REQUIRE(data.longitude == Approx(19.20189679));
  }

  WHEN("north/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,4056.748,N,11212.614,W,,,290620,000.0,W*63\r\n";

    nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

    REQUIRE(data.latitude  == Approx(40.9458060446613));
    REQUIRE(data.longitude == Approx(-112.210235595703));
  }

  WHEN("south/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,2727.069,S,05859.190,W,,,290620,000.0,W*76\r\n";

    nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

    REQUIRE(data.latitude  == Approx(-27.4511422937699));
    REQUIRE(data.longitude == Approx(-58.986502289772));
  }

  WHEN("south/east")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,0610.522,S,10649.632,E,,,290620,000.0,W*6D\r\n";

    nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

    REQUIRE(data.latitude  == Approx(-6.17536097471491));
    REQUIRE(data.longitude == Approx(106.827192306519));
  }
}

SCENARIO("Extracting satellite system from valid GxRMC message", "[GxRMC-02]")
{
  WHEN("GPS")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,0610.522,S,10649.632,E,,,290620,000.0,W*6D\r\n";
    nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);
    REQUIRE(data.source == nmea::RmcSource::GPS);
  }
  WHEN("GNSS")
  {
    std::string const GNSS = "$GNRMC,090348.80,A,4838.00682,N,01301.61381,E,1.533,,301020,,,A*61\r\n";
    nmea::GxRMC::parse(const_cast<char *>(GNSS.c_str()), data);
    REQUIRE(data.source == nmea::RmcSource::GNSS);
  }
}

TEST_CASE("Extracting speed over ground from valid GPRMC message", "[GxRMC-03]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

  /* 85.7 kts ~= 44.088 m/s */
  REQUIRE(data.speed == Approx(44.088f));
}

TEST_CASE("Extracting track angle from valid GPRMC message", "[GxRMC-04]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

  REQUIRE(data.course == Approx(206.4f));
}

TEST_CASE("Extracting position time from valid GPRMC message", "[GxRMC-05]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

  /* 052856.105 -> 05:28:56.105 -> */
  REQUIRE(data.time_utc.hour        == 5);
  REQUIRE(data.time_utc.minute      == 28);
  REQUIRE(data.time_utc.second      == 56);
  REQUIRE(data.time_utc.microsecond == 105);
}

TEST_CASE("Extracting date from valid GPRMC message", "[GxRMC-06]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

  REQUIRE(data.date.day   == 8);
  REQUIRE(data.date.month == 7);
  REQUIRE(data.date.year  == 2020);
}

TEST_CASE("Extracting magnetic variation from valid GPRMC message", "[GxRMC-07]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

  REQUIRE(data.magnetic_variation == Approx(0.0));
}

TEST_CASE("Extracted status indicates void ('V') position data", "[GxRMC-08]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,V,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

  REQUIRE(data.is_valid == false);
}

TEST_CASE("GxRMC message contains only date and time, but no location fix yet ", "[GxRMC-09]")
{
  std::string const GPRMC = ("$GPRMC,144602.00,V,,,,,,,011120,,,N*7B\r\n");

  nmea::GxRMC::parse(const_cast<char *>(GPRMC.c_str()), data);

  /* RMC TIME = '144602.00' */
  REQUIRE(data.time_utc.hour        == 14);
  REQUIRE(data.time_utc.minute      == 46);
  REQUIRE(data.time_utc.second      == 02);
  REQUIRE(data.time_utc.microsecond == 0);

  /* RMC DATE = '011120' */
  REQUIRE(data.date.day             == 1);
  REQUIRE(data.date.month           == 11);
  REQUIRE(data.date.year            == 2020);
}
