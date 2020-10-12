/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

#ifndef ARDUINO_NMEA_UTIL_CHECKSUM_H_
#define ARDUINO_NMEA_UTIL_CHECKSUM_H_

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

namespace util
{

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

bool isChecksumOk   (char const * const nmea_str);
bool isChecksumToken(char const * token);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */

} /* nmea */

#endif /* ARDUINO_NMEA_UTIL_CHECKSUM_H_ */
