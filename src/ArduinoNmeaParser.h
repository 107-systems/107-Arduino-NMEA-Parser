/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

#ifndef ARDUINO_MTK3333_NMEA_PARSER_H_
#define ARDUINO_MTK3333_NMEA_PARSER_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <stdlib.h>
#include <stdint.h>

#undef max
#undef min
#include <functional>

#include "nmea/Types.h"

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef std::function<void(nmea::RmcData const)> OnRmcUpdateFunc;
typedef std::function<void(nmea::GgaData const)> OnGgaUpdateFunc;

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoNmeaParser
{

public:

  ArduinoNmeaParser(OnRmcUpdateFunc on_rmc_update,
                    OnGgaUpdateFunc on_gga_update);


  void encode(char const c);


  inline const nmea::RmcData rmc() const { return _rmc; }
  inline const nmea::GgaData gga() const { return _gga; }


  enum class Error { None, Checksum };

  inline void  clearerr()       { _error = Error::None; }
  inline Error error   () const { return _error; }


private:

  static size_t constexpr NMEA_PARSE_BUFFER_SIZE = 82 + 1; /* Leave space for the '\0' terminator */

  Error _error;
  char _parser_buf[NMEA_PARSE_BUFFER_SIZE];
  size_t _parser_buf_elems;
  nmea::RmcData _rmc;
  nmea::GgaData _gga;
  OnRmcUpdateFunc _on_rmc_update;
  OnGgaUpdateFunc _on_gga_update;

  bool isParseBufferFull();
  void addToParserBuffer(char const c);
  void flushParserBuffer();
  bool isCompleteNmeaMessageInParserBuffer();
  void terminateParserBuffer();
  void parseGxRMC();
  void parseGxGGA();
};

#endif /* ARDUINO_MTK3333_NMEA_PARSER_H_ */
