/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <Parser.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("No GPRMC message received", "[GPRMC-01]")
{
  nmea::Parser parser;

  REQUIRE(parser.longitude() == Approx(52.2637009));
  REQUIRE(parser.latitude()  == Approx(20.9860468));
  REQUIRE(parser.speed()     == Approx(0.0));
  REQUIRE(parser.course()    == Approx(0.0));
}
