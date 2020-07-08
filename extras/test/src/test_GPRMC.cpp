/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <string>
#include <vector>
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
  REQUIRE     (parser.last_fix_utc_s()  == 0);
  REQUIRE     (parser.last_fix_utc_ms() == 0);
}

SCENARIO("Extracting latitude/longiture from valid GPRMC message", "[GPRMC-02]")
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

TEST_CASE("Extracting speed over ground from valid GPRMC message", "[GPRMC-03]")
{
  nmea::Parser parser;

  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  encode(parser, GPRMC);

  REQUIRE(parser.speed() == Approx(44.088f)); /* 85.7 kts ~= 44.088 m/s */
}

TEST_CASE("Extracting track angle from valid GPRMC message", "[GPRMC-04]")
{
  nmea::Parser parser;

  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  encode(parser, GPRMC);

  REQUIRE(parser.course() == Approx(206.4f));
}

TEST_CASE("Extracting position time from valid GPRMC message", "[GPRMC-05]")
{
  nmea::Parser parser;

  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  encode(parser, GPRMC);

  /* 052856.105 -> 05:28:56.105 -> */

  REQUIRE(parser.last_fix_utc_s () == 5*3600 + 28*60 + 56);
  REQUIRE(parser.last_fix_utc_ms() == 105);
}

TEST_CASE("Multiple valid GPRMC messages received", "[GPRMC-06]")
{
  nmea::Parser parser;

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

  auto latitude  = LATITUDE_EXPECTED.begin();
  auto longitude = LONGITUDE_EXPECTED.begin();
  auto speed     = SPEED_EXPECTED.begin();
  auto course    = COURSE_EXPECTED.begin();

  std::for_each(std::begin(GPRMC),
                std::end(GPRMC),
                [&](std::string const & gprmc)
                {
                  encode(parser, gprmc);

                  REQUIRE(parser.latitude () == Approx(*latitude));
                  REQUIRE(parser.longitude() == Approx(*longitude));
                  REQUIRE(parser.speed    () == Approx(*speed));
                  REQUIRE(parser.course   () == Approx(*course));

                  latitude  = std::next(latitude);
                  longitude = std::next(longitude);
                  speed     = std::next(speed);
                  course    = std::next(course);
                });

  REQUIRE(parser.error() == nmea::Parser::Error::None);
}
