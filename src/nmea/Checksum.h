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

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

/* This function expects a zero-terminated string containing a full NMEA
 * message starting with '$' and ending after the trailing \r\n, e.g.
 * "$GPRMC,......*CA\r\n\0".
 */
bool isChecksumOk(char const * const nmea_str);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_NMEA_UTIL_CHECKSUM_H_ */
