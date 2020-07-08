/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "Checksum.h"

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
