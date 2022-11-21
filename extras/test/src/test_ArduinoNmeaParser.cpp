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

#include <ArduinoNmeaParser.h>

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

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("No NMEA message received", "[Parser-01]")
{
  ArduinoNmeaParser parser(nullptr, nullptr);

  REQUIRE(parser.error()                              == ArduinoNmeaParser::Error::None);
  REQUIRE(parser.rmc().is_valid                       == false);
  REQUIRE(parser.rmc().source                         == nmea::RmcSource::Unknown);
  REQUIRE(std::isnan(parser.rmc().latitude)           == true);
  REQUIRE(std::isnan(parser.rmc().longitude)          == true);
  REQUIRE(parser.rmc().time_utc.hour                  == -1);
  REQUIRE(parser.rmc().time_utc.minute                == -1);
  REQUIRE(parser.rmc().time_utc.second                == -1);
  REQUIRE(parser.rmc().time_utc.microsecond           == -1);
  REQUIRE(parser.rmc().date.day                       == -1);
  REQUIRE(parser.rmc().date.month                     == -1);
  REQUIRE(parser.rmc().date.year                      == -1);
  REQUIRE(std::isnan(parser.rmc().speed)              == true);
  REQUIRE(std::isnan(parser.rmc().course)             == true);
  REQUIRE(std::isnan(parser.rmc().magnetic_variation) == true);
}

TEST_CASE("RMC message after startup, no satellites", "[Parser-02]")
{
  ArduinoNmeaParser parser(nullptr, nullptr);

  std::string const GPRMC = ("$GPRMC,,V,,,,,,,,,,N*53\r\n");

  encode(parser, GPRMC);

  REQUIRE(parser.error()                              == ArduinoNmeaParser::Error::None);
  REQUIRE(parser.rmc().is_valid                       == false);
  REQUIRE(parser.rmc().source                         == nmea::RmcSource::GPS);
  REQUIRE(std::isnan(parser.rmc().latitude)           == true);
  REQUIRE(std::isnan(parser.rmc().longitude)          == true);
  REQUIRE(parser.rmc().time_utc.hour                  == -1);
  REQUIRE(parser.rmc().time_utc.minute                == -1);
  REQUIRE(parser.rmc().time_utc.second                == -1);
  REQUIRE(parser.rmc().time_utc.microsecond           == -1);
  REQUIRE(parser.rmc().date.day                       == -1);
  REQUIRE(parser.rmc().date.month                     == -1);
  REQUIRE(parser.rmc().date.year                      == -1);
  REQUIRE(std::isnan(parser.rmc().speed)              == true);
  REQUIRE(std::isnan(parser.rmc().course)             == true);
  REQUIRE(std::isnan(parser.rmc().magnetic_variation) == true);
}

TEST_CASE("RMC message after startup, time fix available", "[Parser-03]")
{
  ArduinoNmeaParser parser(nullptr, nullptr);

  std::string const GPRMC = ("$GPRMC,141928.00,V,,,,,,,,,,N*7A\r\n");

  encode(parser, GPRMC);

  REQUIRE(parser.error()                              == ArduinoNmeaParser::Error::None);
  REQUIRE(parser.rmc().is_valid                       == false);
  REQUIRE(parser.rmc().source                         == nmea::RmcSource::GPS);
  REQUIRE(std::isnan(parser.rmc().latitude)           == true);
  REQUIRE(std::isnan(parser.rmc().longitude)          == true);
  REQUIRE(parser.rmc().time_utc.hour                  == 14);
  REQUIRE(parser.rmc().time_utc.minute                == 19);
  REQUIRE(parser.rmc().time_utc.second                == 28);
  REQUIRE(parser.rmc().time_utc.microsecond           == 0);
  REQUIRE(parser.rmc().date.day                       == -1);
  REQUIRE(parser.rmc().date.month                     == -1);
  REQUIRE(parser.rmc().date.year                      == -1);
  REQUIRE(std::isnan(parser.rmc().speed)              == true);
  REQUIRE(std::isnan(parser.rmc().course)             == true);
  REQUIRE(std::isnan(parser.rmc().magnetic_variation) == true);
}

TEST_CASE("Decoding starts mid-message", "[Parser-04]")
{
  ArduinoNmeaParser parser(nullptr, nullptr);

  std::string const GPRMC = "077.0,023.5,080720,000.0,W*79\r\n$GPRMC,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*79\r\n";

  encode(parser, GPRMC);

  REQUIRE(parser.rmc().is_valid             == true);
  REQUIRE(parser.rmc().source               == nmea::RmcSource::GPS);
  REQUIRE(parser.rmc().latitude             == Approx(52.514467));
  REQUIRE(parser.rmc().longitude            == Approx(13.349300));
  REQUIRE(parser.rmc().speed                == Approx(39.6122));
  REQUIRE(parser.rmc().course               == Approx(23.5));
  REQUIRE(parser.rmc().time_utc.hour        == 5);
  REQUIRE(parser.rmc().time_utc.minute      == 28);
  REQUIRE(parser.rmc().time_utc.second      == 52);
  REQUIRE(parser.rmc().time_utc.microsecond == 105);
}

TEST_CASE("NMEA message with data corruption (checksum mismatch) received", "[Parser-05]")
{
  ArduinoNmeaParser parser(nullptr, nullptr);

  std::string const GPRMC = "$GPXXX,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*79\r\n";

  REQUIRE(parser.error() == ArduinoNmeaParser::Error::None);
  encode(parser, GPRMC);
  REQUIRE(parser.error() == ArduinoNmeaParser::Error::Checksum);
}

TEST_CASE("Multiple NMEA messages received", "[Parser-06]")
{
  ArduinoNmeaParser parser(nullptr, nullptr);

  std::vector<std::string> const NMEA_RMC =
  {
    std::string("$GPRMC,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*79\r\n"),
    std::string("$BDRMC,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*68\r\n"),

    std::string("$GPRMC,052853.105,A,5230.888,N,01320.967,E,084.5,069.2,080720,000.0,W*7A\r\n"),
    std::string("$GPRMC,052854.105,A,5230.901,N,01321.000,E,085.8,099.8,080720,000.0,W*7D\r\n"),
    std::string("$GPRMC,052855.105,A,5230.894,N,01321.038,E,084.3,138.7,080720,000.0,W*75\r\n"),
    std::string("$GPRMC,142600.00,A,4837.99474,N,01301.53452,E,27.920,247.03,121020,,,A*5A\r\n")
  };

  std::vector<nmea::RmcSource> const SOURCE_EXPECTED =
  {
    nmea::RmcSource::GPS, nmea::RmcSource::BDS, nmea::RmcSource::GPS, nmea::RmcSource::GPS, nmea::RmcSource::GPS, nmea::RmcSource::GPS
  };

  std::vector<float> const LATITUDE_EXPECTED =
  {
    52.514467f, 52.514467f, 52.514800f, 52.515017f, 52.514900f, 48.633246f
  };

  std::vector<float> const LONGITUDE_EXPECTED =
  {
    13.349300, 13.349300, 13.349450, 13.350000, 13.350633f, 13.025575f
  };

  std::vector<float> const SPEED_EXPECTED =
  {
    39.6122f, 39.6122f, 43.47056f, 44.139333f, 43.36767f, 14.363289f
  };

  std::vector<float> const COURSE_EXPECTED =
  {
    23.5f, 23.5f, 69.2f, 99.8f, 138.7f, 247.03f
  };

  std::vector<float> const MAGNETIC_VARIATION_EXPECTED =
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, NAN
  };

  std::vector<int> const DAY_EXPECTED =
  {
    8, 8, 8, 8, 8, 12
  };

  std::vector<int> const MONTH_EXPECTED =
  {
    7, 7, 7, 7, 7, 10
  };

  std::vector<int> const YEAR_EXPECTED =
  {
    2020, 2020, 2020, 2020, 2020, 2020
  };


  auto source    = SOURCE_EXPECTED.begin();
  auto latitude  = LATITUDE_EXPECTED.begin();
  auto longitude = LONGITUDE_EXPECTED.begin();
  auto speed     = SPEED_EXPECTED.begin();
  auto course    = COURSE_EXPECTED.begin();
  auto mag_var   = MAGNETIC_VARIATION_EXPECTED.begin();
  auto day       = DAY_EXPECTED.begin();
  auto month     = MONTH_EXPECTED.begin();
  auto year      = YEAR_EXPECTED.begin();

  std::for_each(std::begin(NMEA_RMC),
                std::end(NMEA_RMC),
                [&](std::string const & rmc)
                {
                  encode(parser, rmc);

                  REQUIRE(parser.error()          == ArduinoNmeaParser::Error::None);
                  REQUIRE(parser.rmc().source     == *source);
                  REQUIRE(parser.rmc().latitude   == Approx(*latitude));
                  REQUIRE(parser.rmc().longitude  == Approx(*longitude));
                  REQUIRE(parser.rmc().speed      == Approx(*speed));
                  REQUIRE(parser.rmc().course     == Approx(*course));
                  REQUIRE(parser.rmc().date.day   == *day);
                  REQUIRE(parser.rmc().date.month == *month);
                  REQUIRE(parser.rmc().date.year  == *year);

                  if (std::isnan(*mag_var)) REQUIRE(std::isnan(parser.rmc().magnetic_variation) == true);
                  else                      REQUIRE(parser.rmc().magnetic_variation == Approx(*mag_var));

                  source    = std::next(source);
                  latitude  = std::next(latitude);
                  longitude = std::next(longitude);
                  speed     = std::next(speed);
                  course    = std::next(course);
                  mag_var   = std::next(mag_var);
                  day       = std::next(day);
                  month     = std::next(month);
                  year      = std::next(year);
                });

  REQUIRE(parser.error() == ArduinoNmeaParser::Error::None);
}

TEST_CASE("NMEA message with no checksum received", "[Parser-07]")
{
  ArduinoNmeaParser parser(nullptr, nullptr);
  std::string const GPRMC = "79\r\n"; /* This should not lead to a segmentation violation. */
  encode(parser, GPRMC);
}
