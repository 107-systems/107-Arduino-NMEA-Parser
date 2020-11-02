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

#include <nmea/util/rmc.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing 'rmc_isGPRMC(...)' with valid and invalid GPRMC NMEA messages", "[rmc_isGPRMC-01]")
{
  WHEN ("a valid GPRMC NMEA message")
    REQUIRE (nmea::util::rmc_isGPRMC("$GPRMC") == true);
  WHEN ("a invalid GPRMC NMEA message")
    REQUIRE (nmea::util::rmc_isGPRMC("$GARMC") == false);
}

TEST_CASE ("Testing 'rmc_isGLRMC(...)' with valid and invalid GLRMC NMEA messages", "[rmc_isGLRMC-01]")
{
  WHEN ("a valid GLRMC NMEA message")
    REQUIRE (nmea::util::rmc_isGLRMC("$GLRMC") == true);
  WHEN ("a invalid GLRMC NMEA message")
    REQUIRE (nmea::util::rmc_isGLRMC("$GARMC") == false);
}

TEST_CASE ("Testing 'rmc_isGARMC(...)' with valid and invalid GARMC NMEA messages", "[rmc_isGARMC-01]")
{
  WHEN ("a valid GARMC NMEA message")
    REQUIRE (nmea::util::rmc_isGARMC("$GARMC") == true);
  WHEN ("a invalid GARMC NMEA message")
    REQUIRE (nmea::util::rmc_isGARMC("$GLRMC") == false);
}

TEST_CASE ("Testing 'rmc_isGNRMC(...)' with valid and invalid GNRMC NMEA messages", "[rmc_isGNRMC-01]")
{
  WHEN ("a valid GNRMC NMEA message")
    REQUIRE (nmea::util::rmc_isGNRMC("$GNRMC") == true);
  WHEN ("a invalid GNRMC NMEA message")
    REQUIRE (nmea::util::rmc_isGNRMC("$GARMC") == false);
}

TEST_CASE ("Testing 'rmc_isGxRMC(...)' with valid and invalid G(P|L|A|N)RMC NMEA messages", "[rmc_isGxRMC-01]")
{
  WHEN ("a valid G(P|L|A|N)RMC NMEA message")
  {
    REQUIRE (nmea::util::rmc_isGxRMC("$GPRMC") == true);
    REQUIRE (nmea::util::rmc_isGxRMC("$GLRMC") == true);
    REQUIRE (nmea::util::rmc_isGxRMC("$GARMC") == true);
    REQUIRE (nmea::util::rmc_isGxRMC("$GNRMC") == true);
  }
  WHEN ("a invalid G(P|L|A|N)RMC NMEA message")
    REQUIRE (nmea::util::rmc_isGxRMC("$GIRMC") == false);
}
