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
#include <algorithm>

#include <catch.hpp>

#include <nmea/util/checksum.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("NMEA message with valid checksum - MTK3333", "[checksum-01]")
{
  std::string const GPRMC_CHECKSUM_OK = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*45\r\n";
  REQUIRE(nmea::util::isChecksumOk(GPRMC_CHECKSUM_OK.c_str()) == true);
}

TEST_CASE("NMEA message with valid checksum - ublox NEO-6M", "[checksum-02]")
{
  std::string const GPRMC_CHECKSUM_OK = "$GPRMC,193517.00,A,4837.99895,N,01301.58584,E,0.793,,111020,,,A*7D\r\n";
  REQUIRE(nmea::util::isChecksumOk(GPRMC_CHECKSUM_OK.c_str()) == true);
}

TEST_CASE("NMEA message with invalid checksum", "[checksum-03]")
{
  std::string const GPRMC_CHECKSUM_ERROR = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*FF\r\n";
  REQUIRE(nmea::util::isChecksumOk(GPRMC_CHECKSUM_ERROR.c_str()) == false);
}
