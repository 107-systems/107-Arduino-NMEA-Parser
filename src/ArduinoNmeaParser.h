/**
 * This software is distributed under the terms of the LGPL 3.0 License.
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

typedef std::function<void(float const, float const, float const, float const, float const)> OnRMCUpdateFunc;

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoNmeaParser
{

public:

  ArduinoNmeaParser(OnRMCUpdateFunc on_rmc_update);


  void encode(char const c);


#ifdef HOST
  inline float latitude          () const { return _rmc.latitude; }
  inline float longitude         () const { return _rmc.longitude; }
  inline float speed             () const { return _rmc.speed; }
  inline float course            () const { return _rmc.course; }
  inline float last_fix_utc_s    () const { return _rmc.last_fix_utc_s; }
#endif
  inline float magnetic_variation() const { return _rmc.magnetic_variation; }
  inline int   day               () const { return _rmc.date.day; }
  inline int   month             () const { return _rmc.date.month; }
  inline int   year              () const { return _rmc.date.year; }

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
  OnRMCUpdateFunc _on_rmc_update;

  bool isParseBufferFull();
  void addToParserBuffer(char const c);
  void flushParserBuffer();
  bool isCompleteNmeaMessageInParserBuffer();
  void terminateParserBuffer();
  void parseGPRMC();

};

#endif /* ARDUINO_MTK3333_NMEA_PARSER_H_ */
