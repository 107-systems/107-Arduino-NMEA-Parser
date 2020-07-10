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

TEST_CASE("NMEA message with valid checksum", "[Checksum-01]")
{
  std::string const GPRMC_CHECKSUM_OK = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*45\r\n";

  REQUIRE(nmea::isChecksumOk(GPRMC_CHECKSUM_OK.c_str()) == true);
}

TEST_CASE("NMEA message with invalid checksum", "[Checksum-02]")
{
  std::string const GPRMC_CHECKSUM_ERROR = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*FF\r\n";

  REQUIRE(nmea::isChecksumOk(GPRMC_CHECKSUM_ERROR.c_str()) == false);
}
