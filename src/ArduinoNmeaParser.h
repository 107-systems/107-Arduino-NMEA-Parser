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

typedef std::function<void(nmea::RmcData const)> OnGprmcUpdateFunc;

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoNmeaParser
{

public:

  ArduinoNmeaParser(OnGprmcUpdateFunc on_rmc_update);


  void encode(char const c);


  inline const nmea::RmcData rmc() const { return _rmc; }


  enum class Error { None, Checksum, RMC };

  inline void  clearerr()       { _error = Error::None; }
  inline Error error   () const { return _error; }


private:

  static size_t constexpr NMEA_PARSE_BUFFER_SIZE = 82 + 1; /* Leave space for the '\0' terminator */

  typedef struct
  {
    char buf[NMEA_PARSE_BUFFER_SIZE];
    size_t elems_in_buf;
  } ParserBuffer;

  enum class ParserState
  {
    Synching, Synced
  };

  Error _error;
  ParserState _parser_state;
  ParserBuffer _parser_buf;
  nmea::RmcData _rmc;
  OnGprmcUpdateFunc _on_gprmc_update;

  bool isParseBufferFull();
  void addToParserBuffer(char const c);
  void flushParserBuffer();
  bool isCompleteNmeaMessageInParserBuffer();
  void terminateParserBuffer();
  void parseGPRMC();

};

#endif /* ARDUINO_MTK3333_NMEA_PARSER_H_ */
