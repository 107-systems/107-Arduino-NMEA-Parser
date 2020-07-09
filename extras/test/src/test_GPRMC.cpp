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

#include <GPRMC.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

static float latitude;
static float longitude;
static float speed;
static float course;
static float last_fix_utc_s;

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

SCENARIO("Extracting latitude/longiture from valid GPRMC message", "[GPRMC-01]")
{
  WHEN("north/east")
  {
    std::string const GPRMC = "$GPRMC,062101.714,A,5001.869,N,01912.114,E,955535.7,116.2,290520,000.0,W*45\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
    REQUIRE(latitude  == Approx(50.03114442));
    REQUIRE(longitude == Approx(19.20189679));
  }

  WHEN("north/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,4056.748,N,11212.614,W,,,290620,000.0,W*63\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
    REQUIRE(latitude  == Approx(40.9458060446613));
    REQUIRE(longitude == Approx(-112.210235595703));
  }

  WHEN("south/west")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,2727.069,S,05859.190,W,,,290620,000.0,W*76\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
    REQUIRE(latitude  == Approx(-27.4511422937699));
    REQUIRE(longitude == Approx(-58.986502289772));
  }

  WHEN("south/east")
  {
    std::string const GPRMC = "$GPRMC,122311.239,A,0610.522,S,10649.632,E,,,290620,000.0,W*6D\r\n";

    REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
    REQUIRE(latitude  == Approx(-6.17536097471491));
    REQUIRE(longitude == Approx(106.827192306519));
  }
}

TEST_CASE("Extracting speed over ground from valid GPRMC message", "[GPRMC-02]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
  /* 85.7 kts ~= 44.088 m/s */
  REQUIRE(speed == Approx(44.088f));
}

TEST_CASE("Extracting track angle from valid GPRMC message", "[GPRMC-03]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
  REQUIRE(course == Approx(206.4f));
}

TEST_CASE("Extracting position time from valid GPRMC message", "[GPRMC-04]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,A,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
  /* 052856.105 -> 05:28:56.105 -> */
  REQUIRE(last_fix_utc_s  == Approx(5*3600 + 28*60 + 56 + 0.105f));
}

TEST_CASE("Extracted status indicates void ('V') position data", "[GPRMC-05]")
{
  std::string const GPRMC = ("$GPRMC,052856.105,V,5230.874,N,01321.056,E,085.7,206.4,080720,000.0,W*78\r\n");

  latitude = 1.0f; longitude = 2.0f; speed = 3.0f; course = 4.0f; last_fix_utc_s = 0;

  REQUIRE(nmea::GPRMC::parse(GPRMC.c_str(), last_fix_utc_s, latitude, longitude, speed, course) == true);
  REQUIRE(latitude        == 1.0f);
  REQUIRE(longitude       == 2.0f);
  REQUIRE(speed           == 3.0f);
  REQUIRE(course          == 4.0f);
  REQUIRE(last_fix_utc_s  == Approx(5*3600 + 28*60 + 56 + 0.105f));
}
