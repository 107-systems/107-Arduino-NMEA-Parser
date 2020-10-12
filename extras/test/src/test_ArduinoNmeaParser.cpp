/**
 * This software is distributed under the terms of the LGPL 3.0 License.
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

void encode(ArduinoNmeaParser & parser, std::string const & nmea)
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
  ArduinoNmeaParser parser(nullptr);

  REQUIRE(parser.latitude()  == Approx(20.9860468));
  REQUIRE(parser.longitude() == Approx(52.2637009));
  REQUIRE(std::isnan(parser.speed())              == true);
  REQUIRE(std::isnan(parser.course())             == true);
  REQUIRE(std::isnan(parser.last_fix_utc_s())     == true);
  REQUIRE(std::isnan(parser.magnetic_variation()) == true);
  REQUIRE(parser.day  () == -1);
  REQUIRE(parser.month() == -1);
  REQUIRE(parser.year () == -1);
}

TEST_CASE("RMC message after startup, no satellites", "[Parser-02]")
{
  ArduinoNmeaParser parser(nullptr);

  std::string const GPRMC = ("$GPRMC,,V,,,,,,,,,,N*53\r\n");

  encode(parser, GPRMC);

  REQUIRE(parser.error()                          == ArduinoNmeaParser::Error::None);
  REQUIRE(parser.latitude()                       == Approx(20.9860468));
  REQUIRE(parser.longitude()                      == Approx(52.2637009));
  REQUIRE(std::isnan(parser.speed())              == true);
  REQUIRE(std::isnan(parser.course())             == true);
  REQUIRE(std::isnan(parser.last_fix_utc_s())     == true);
  REQUIRE(std::isnan(parser.magnetic_variation()) == true);
  REQUIRE(parser.day()                            == -1);
  REQUIRE(parser.month()                          == -1);
  REQUIRE(parser.year()                           == -1);
}

TEST_CASE("RMC message after startup, time fix available", "[Parser-03]")
{
  ArduinoNmeaParser parser(nullptr);

  std::string const GPRMC = ("$GPRMC,141928.00,V,,,,,,,,,,N*7A\r\n");

  encode(parser, GPRMC);

  REQUIRE(parser.error()                          == ArduinoNmeaParser::Error::None);
  REQUIRE(parser.latitude()                       == Approx(20.9860468));
  REQUIRE(parser.longitude()                      == Approx(52.2637009));
  REQUIRE(std::isnan(parser.speed())              == true);
  REQUIRE(std::isnan(parser.course())             == true);
  REQUIRE(parser.last_fix_utc_s()                 == Approx(14*3600 + 19*60 + 28));
  REQUIRE(std::isnan(parser.magnetic_variation()) == true);
  REQUIRE(parser.day()                            == -1);
  REQUIRE(parser.month()                          == -1);
  REQUIRE(parser.year()                           == -1);
}

TEST_CASE("Decoding starts mid-message", "[Parser-04]")
{
  ArduinoNmeaParser parser(nullptr);

  std::string const GPRMC = "077.0,023.5,080720,000.0,W*79\r\n$GPRMC,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*79\r\n";

  encode(parser, GPRMC);

  REQUIRE(parser.latitude()        == Approx(52.514467));
  REQUIRE(parser.longitude()       == Approx(13.349300));
  REQUIRE(parser.speed()           == Approx(39.6122));
  REQUIRE(parser.course()          == Approx(23.5));
  REQUIRE(parser.last_fix_utc_s()  == Approx(5*3600 + 28*60 + 52 + 0.105f));
}

TEST_CASE("NMEA message with data corruption (checksum mismatch) received", "[Parser-05]")
{
  ArduinoNmeaParser parser(nullptr);

  std::string const GPRMC = "$GPXXX,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*79\r\n";

  REQUIRE(parser.error() == ArduinoNmeaParser::Error::None);
  encode(parser, GPRMC);
  REQUIRE(parser.error() == ArduinoNmeaParser::Error::Checksum);
}

TEST_CASE("Invalid GPRMC message received", "[Parser-06]")
{
  ArduinoNmeaParser parser(nullptr);

  std::string const GPRMC = "$GPRMC,052852.105,A,5230.868,Y,01320.958,E,077.0,023.5,080720,000.0,W*6E\r\n";

  REQUIRE(parser.error() == ArduinoNmeaParser::Error::None);
  encode(parser, GPRMC);
  REQUIRE(parser.error() == ArduinoNmeaParser::Error::RMC);
}

TEST_CASE("Multiple NMEA messages received", "[Parser-07]")
{
  ArduinoNmeaParser parser(nullptr);

  std::vector<std::string> const GPRMC =
  {
    std::string("$GPRMC,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*79\r\n"),
    std::string("$GPRMC,052853.105,A,5230.888,N,01320.967,E,084.5,069.2,080720,000.0,W*7A\r\n"),
    std::string("$GPRMC,052854.105,A,5230.901,N,01321.000,E,085.8,099.8,080720,000.0,W*7D\r\n"),
    std::string("$GPRMC,052855.105,A,5230.894,N,01321.038,E,084.3,138.7,080720,000.0,W*75\r\n"),
  };

  std::vector<float> const LATITUDE_EXPECTED =
  {
    52.514467f, 52.514800f, 52.515017f, 52.514900f
  };

  std::vector<float> const LONGITUDE_EXPECTED =
  {
    13.349300, 13.349450, 13.350000, 13.350633f
  };

  std::vector<float> const SPEED_EXPECTED =
  {
    39.6122f, 43.47056f, 44.139333f, 43.36767f
  };

  std::vector<float> const COURSE_EXPECTED =
  {
    23.5f, 69.2f, 99.8f, 138.7f
  };

  std::vector<float> const MAGNETIC_VARIATION_EXPECTED =
  {
    0.0f, 0.0f, 0.0f, 0.0f
  };

  std::vector<int> const DAY_EXPECTED =
  {
    8, 8, 8, 8
  };

  std::vector<int> const MONTH_EXPECTED =
  {
    7, 7, 7, 7
  };

  std::vector<int> const YEAR_EXPECTED =
  {
    2020, 2020, 2020, 2020
  };


  auto latitude  = LATITUDE_EXPECTED.begin();
  auto longitude = LONGITUDE_EXPECTED.begin();
  auto speed     = SPEED_EXPECTED.begin();
  auto course    = COURSE_EXPECTED.begin();
  auto mag_var   = MAGNETIC_VARIATION_EXPECTED.begin();
  auto day       = DAY_EXPECTED.begin();
  auto month     = MONTH_EXPECTED.begin();
  auto year      = YEAR_EXPECTED.begin();

  std::for_each(std::begin(GPRMC),
                std::end(GPRMC),
                [&](std::string const & gprmc)
                {
                  encode(parser, gprmc);

                  REQUIRE(parser.latitude          () == Approx(*latitude));
                  REQUIRE(parser.longitude         () == Approx(*longitude));
                  REQUIRE(parser.speed             () == Approx(*speed));
                  REQUIRE(parser.course            () == Approx(*course));
                  REQUIRE(parser.magnetic_variation() == Approx(*mag_var));
                  REQUIRE(parser.day               () == *day);
                  REQUIRE(parser.month             () == *month);
                  REQUIRE(parser.year              () == *year);

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
