/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <nmea/util/gga.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing 'gga_isGPGGA(...)' with valid and invalid GPGGA NMEA messages", "[gga_isGPGGA-01]")
{
  WHEN ("a valid GPGGA NMEA message")
    REQUIRE (nmea::util::gga_isGPGGA("$GPGGA") == true);
  WHEN ("a invalid GPGGA NMEA message")
    REQUIRE (nmea::util::gga_isGPGGA("$GAGGA") == false);
}

TEST_CASE ("Testing 'gga_isGLGGA(...)' with valid and invalid GLGGA NMEA messages", "[gga_isGLGGA-01]")
{
  WHEN ("a valid GLGGA NMEA message")
    REQUIRE (nmea::util::gga_isGLGGA("$GLGGA") == true);
  WHEN ("a invalid GLGGA NMEA message")
    REQUIRE (nmea::util::gga_isGLGGA("$GAGGA") == false);
}

TEST_CASE ("Testing 'gga_isGAGGA(...)' with valid and invalid GAGGA NMEA messages", "[gga_isGAGGA-01]")
{
  WHEN ("a valid GAGGA NMEA message")
    REQUIRE (nmea::util::gga_isGAGGA("$GAGGA") == true);
  WHEN ("a invalid GAGGA NMEA message")
    REQUIRE (nmea::util::gga_isGAGGA("$GLGGA") == false);
}

TEST_CASE ("Testing 'gga_isGNGGA(...)' with valid and invalid GNGGA NMEA messages", "[gga_isGNGGA-01]")
{
  WHEN ("a valid GNGGA NMEA message")
    REQUIRE (nmea::util::gga_isGNGGA("$GNGGA") == true);
  WHEN ("a invalid GNGGA NMEA message")
    REQUIRE (nmea::util::gga_isGNGGA("$GAGGA") == false);
}

TEST_CASE ("Testing 'gga_isGxGGA(...)' with valid and invalid G(P|L|A|N)GGA NMEA messages", "[gga_isGxGGA-01]")
{
  WHEN ("a valid G(P|L|A|N)GGA NMEA message")
  {
    REQUIRE (nmea::util::gga_isGxGGA("$GPGGA") == true);
    REQUIRE (nmea::util::gga_isGxGGA("$GLGGA") == true);
    REQUIRE (nmea::util::gga_isGxGGA("$GAGGA") == true);
    REQUIRE (nmea::util::gga_isGxGGA("$GNGGA") == true);
  }
  WHEN ("a invalid G(P|L|A|N)GGA NMEA message")
    REQUIRE (nmea::util::gga_isGxGGA("$GIGGA") == false);
}
