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

#include <catch.hpp>

#include <nmea/GxGGA.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

static nmea::GgaData data;

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

SCENARIO("Extracting satellite system from valid GxGGA message", "[GxGGA-01]")
{
  WHEN("GPS")
  {
    std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
    nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
    REQUIRE(data.source == nmea::GgaSource::GPS);
  }
}

TEST_CASE("Extracting position time from valid GxGGA message", "[GxGGA-02]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(data.time_utc.hour        == 11);
  REQUIRE(data.time_utc.minute      == 19);
  REQUIRE(data.time_utc.second      == 8);
  REQUIRE(data.time_utc.microsecond == 952);
}

SCENARIO("Extracting latitude/longiture from valid GPGGA message", "[GxGGA-03]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(data.latitude  == Approx(48.633433));
  REQUIRE(data.longitude == Approx(13.026492));
}

TEST_CASE("Extracting fix quality angle from valid GxGGA message", "[GxGGA-04]")
{
  WHEN("GPS Fix")
  {
    std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
    nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
    REQUIRE(data.fix_quality == nmea::FixQuality::GPS_Fix);
  }
}

SCENARIO("Extracting number of satellites from valid GPGGA message", "[GxGGA-05]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(data.num_satellites == 5);
}

SCENARIO("Extracting horizontal dilution of precision from valid GPGGA message", "[GxGGA-06]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(data.hdop == Approx(2.4));
}

SCENARIO("Extracting altitude from valid GPGGA message", "[GxGGA-07]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(data.altitude == Approx(454.7));
}

SCENARIO("Extracting geoidal separation from valid GPGGA message", "[GxGGA-08]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(data.geoidal_separation == Approx(46.6));
}

SCENARIO("Extracting time since last DGPS update from valid GPGGA message", "[GxGGA-09]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(data.dgps_age == Approx(0.0));
}

SCENARIO("Extracting DGPS station id from valid GPGGA message", "[GxGGA-10]")
{
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  nmea::GxGGA::parse(const_cast<char *>(GPGGA.c_str()), data);
  REQUIRE(strncmp(data.dgps_id, "0000", 4) == 0);
}
