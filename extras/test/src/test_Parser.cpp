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

TEST_CASE("No NMEA message received", "[Parser-01]")
{
  nmea::Parser parser;

  REQUIRE     (parser.latitude()  == Approx(20.9860468));
  REQUIRE     (parser.longitude() == Approx(52.2637009));
  REQUIRE_THAT(parser.speed(),  Catch::Matchers::WithinULP(0.0f, 10));
  REQUIRE_THAT(parser.course(), Catch::Matchers::WithinULP(0.0f, 10));
  REQUIRE     (parser.last_fix_utc_s()  == 0);
  REQUIRE     (parser.last_fix_utc_ms() == 0);
}

TEST_CASE("Decoding starts mid-message", "[Parser-02]")
{
  nmea::Parser parser;

  std::string const GPRMC = "077.0,023.5,080720,000.0,W*79\r\n$GPRMC,052852.105,A,5230.868,N,01320.958,E,077.0,023.5,080720,000.0,W*79\r\n";

  encode(parser, GPRMC);

  REQUIRE(parser.latitude()        == Approx(52.514467));
  REQUIRE(parser.longitude()       == Approx(13.349300));
  REQUIRE(parser.speed()           == Approx(39.6122));
  REQUIRE(parser.course()          == Approx(23.5));
  REQUIRE(parser.last_fix_utc_s()  == 5*3600 + 28*60 + 52);
  REQUIRE(parser.last_fix_utc_ms() == 105);
}

TEST_CASE("Multiple NMEA messages received", "[Parser-03]")
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
