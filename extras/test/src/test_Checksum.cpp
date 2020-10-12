/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <string>
#include <algorithm>

#include <catch.hpp>

#include <nmea/Checksum.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("NMEA message with valid checksum - MTK3333", "[Checksum-01]")
{
  std::string const GPRMC_CHECKSUM_OK = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*45\r\n";
  REQUIRE(nmea::isChecksumOk(GPRMC_CHECKSUM_OK.c_str()) == true);
}

TEST_CASE("NMEA message with valid checksum - ublox NEO-6M", "[Checksum-02]")
{
  std::string const GPRMC_CHECKSUM_OK = "$GPRMC,193517.00,A,4837.99895,N,01301.58584,E,0.793,,111020,,,A*7D\r\n";
  REQUIRE(nmea::isChecksumOk(GPRMC_CHECKSUM_OK.c_str()) == true);
}

TEST_CASE("NMEA message with invalid checksum", "[Checksum-03]")
{
  std::string const GPRMC_CHECKSUM_ERROR = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*FF\r\n";
  REQUIRE(nmea::isChecksumOk(GPRMC_CHECKSUM_ERROR.c_str()) == false);
}

TEST_CASE("Test isChecksumToken(...) with valid checksum token", "[Checksum-04]")
{
  std::string const VALID_CHECKSUM_TOKEN = "W*FF";
  REQUIRE(nmea::isChecksumToken(VALID_CHECKSUM_TOKEN.c_str()) == true);
}

TEST_CASE("Test isChecksumToken(...) with invalid checksum token", "[Checksum-04]")
{
  std::string const INVALID_CHECKSUM_TOKEN = "01912.114";
  REQUIRE(nmea::isChecksumToken(INVALID_CHECKSUM_TOKEN.c_str()) == false);
}
