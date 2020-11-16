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

static bool on_gga_update_called = false;

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

void onGgaUpdate(nmea::GgaData const data)
{
  on_gga_update_called = true;

  REQUIRE(data.source == nmea::GgaSource::GPS);

  REQUIRE(data.time_utc.hour        == 11);
  REQUIRE(data.time_utc.minute      == 19);
  REQUIRE(data.time_utc.second      == 8);
  REQUIRE(data.time_utc.microsecond == 952);

  REQUIRE(data.latitude  == Approx(48.633433));
  REQUIRE(data.longitude == Approx(13.026492));

  REQUIRE(data.fix_quality == nmea::FixQuality::GPS_Fix);

  REQUIRE(data.num_satellites == 5);
  REQUIRE(data.hdop == Approx(2.4));
  REQUIRE(data.altitude == Approx(454.7));
  REQUIRE(data.geoidal_separation == Approx(46.6));

  REQUIRE(data.dgps_age == Approx(0.0));
  REQUIRE(strncmp(data.dgps_id, "0000", 4) == 0);
}

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("Testing execution of OnGgaUpdateFunc when a full GxGGA message has been received", "[OnGgaUpdateFunc-01]")
{
  ArduinoNmeaParser parser(nullptr, onGgaUpdate);
  std::string const GPGGA = "$GPGGA,111908.952,4838.0060,N,01301.5895,E,1,05,2.4,454.7,M,46.6,M,0.0,0000*7A\r\n";
  encode(parser, GPGGA);
  REQUIRE(on_gga_update_called == true);
}
