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

#include <Parser.h>

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

void encode(nmea::Parser & parser, std::string const & nmea)
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

TEST_CASE("No GPRMC message received", "[GPRMC-01]")
{
  nmea::Parser parser;

  REQUIRE     (parser.latitude()  == Approx(20.9860468));
  REQUIRE     (parser.longitude() == Approx(52.2637009));
  REQUIRE_THAT(parser.speed(),  Catch::Matchers::WithinULP(0.0f, 10));
  REQUIRE_THAT(parser.course(), Catch::Matchers::WithinULP(0.0f, 10));
}

SCENARIO("Valid GPRMC message received", "[GPRMC-02]")
{
  nmea::Parser parser;

  WHEN("north/east")
  {
    std::string const GPRMC = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*45\r\n";

    encode(parser, GPRMC);

    REQUIRE(parser.latitude()  == Approx(50.03114442));
    REQUIRE(parser.longitude() == Approx(19.20189679));
  }

  WHEN("north/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,4056.748,N,11212.614,W,,,290620,000.0,W*63\r\n";

    encode(parser, GPRMC);

    REQUIRE(parser.latitude()  == Approx(40.9458060446613));
    REQUIRE(parser.longitude() == Approx(-112.210235595703));
  }

  WHEN("south/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,2727.069,S,05859.190,W,,,290620,000.0,W*76\r\n";

    encode(parser, GPRMC);

    REQUIRE(parser.latitude()  == Approx(-27.4511422937699));
    REQUIRE(parser.longitude() == Approx(-58.986502289772));
  }

  WHEN("south/east")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,0610.522,S,10649.632,E,,,290620,000.0,W*6D\r\n";

    encode(parser, GPRMC);

    REQUIRE(parser.latitude()  == Approx(-6.17536097471491));
    REQUIRE(parser.longitude() == Approx(106.827192306519));
  }
}

TEST_CASE("Valid GPRMC with speed over ground > 0 received", "[GPRMC-03]")
{
  nmea::Parser parser;

  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  encode(parser, GPRMC);

  REQUIRE(parser.speed() == Approx(44.088f)); /* 85.7 kts ~= 44.088 m/s */
}
