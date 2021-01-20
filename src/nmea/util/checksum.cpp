/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "checksum.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#undef max
#undef min
#include <algorithm>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

namespace util
{

/**************************************************************************************
 * INTERNAL FUNCTION DECLARATION
 **************************************************************************************/

uint8_t calcChecksum   (char const * const nmea_str);
uint8_t extractChecksum(char const * const nmea_str);

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

/* This function expects a zero-terminated string containing a full NMEA
 * message starting with '$' and ending after the trailing \r\n, e.g.
 * "$GPRMC,......*CA\r\n\0".
 */
bool isChecksumOk(char const * const nmea_str)
{
  return (calcChecksum(nmea_str) == extractChecksum(nmea_str));
}

/**************************************************************************************
 * INTERNAL FUNCTION DEFINITION
 **************************************************************************************/

uint8_t calcChecksum(char const * const nmea_str)
{
  uint8_t checksum = 0;

  std::for_each(nmea_str,
                nmea_str + strlen(nmea_str),
                [&checksum](char const c)
                {
                  static bool use_char_for_checksum = false;

                  if (c == '$') {
                    use_char_for_checksum = true;
                    return;
                  }

                  if (c == '*') {
                    use_char_for_checksum = false;
                    return;
                  }

                  if (use_char_for_checksum)
                    checksum ^= static_cast<uint8_t>(c);
                });

  return checksum;
}

uint8_t extractChecksum(char const * const nmea_str)
{
  char checksum_str[3] = {0};
  /* Find the start of the checksum message */
  char const * start_checksum = strchr(nmea_str, '*');
  /* Extract checksum. */
  strncpy(checksum_str, start_checksum + 1, 2);
  /* Convert to integer. */
  return strtol(checksum_str, nullptr, 16);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */
