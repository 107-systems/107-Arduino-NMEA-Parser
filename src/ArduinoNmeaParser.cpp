/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-NMEA-Parser/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "ArduinoNmeaParser.h"

#include <string.h>

#include "nmea/GxRMC.h"
#include "nmea/GxGGA.h"
#include "nmea/util/rmc.h"
#include "nmea/util/gga.h"
#include "nmea/util/checksum.h"

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoNmeaParser::ArduinoNmeaParser(OnRmcUpdateFunc on_rmc_update,
                                     OnGgaUpdateFunc on_gga_update)
: _error{Error::None}
, _parser_state{ParserState::Synching}
, _parser_buf{{0}, 0}
, _rmc{nmea::INVALID_RMC}
, _gga{nmea::INVALID_GGA}
, _on_rmc_update{on_rmc_update}
, _on_gga_update{on_gga_update}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoNmeaParser::encode(char const c)
{
  /* Wait for the first '$' to be received which
   * indicates the start of a NMEA message.
   */
  if (_parser_state == ParserState::Synching) {
    if (c == '$')
      _parser_state = ParserState::Synced;
    else
      return;
  }

  if (!isParseBufferFull())
    addToParserBuffer(c);

  if (!isCompleteNmeaMessageInParserBuffer()) {
    if (isParseBufferFull()) {
      flushParserBuffer();
    }
    return;
  }

  /* At this point a complete NMEA message is stored
   * in the parser buffer and a 0 termination is added
   * to allow the usage of string library functions on
   * the buffer.
   */
  terminateParserBuffer();

  /* Verify if the checksum of the NMEA message is correct. */
  if (!nmea::util::isChecksumOk(_parser_buf.buf)) {
    _error = Error::Checksum;
    flushParserBuffer();
    return;
  }

  /* Parse the various NMEA messages. */
  if      (nmea::util::rmc_isGxRMC(_parser_buf.buf)) parseGxRMC();
  else if (nmea::util::rmc_isGxGGA(_parser_buf.buf)) parseGxGGA();

  /* The NMEA message has been fully processed and all
   * values updates so its time to flush the parser
   * buffer to make space for the next message.
   */
  flushParserBuffer();
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

bool ArduinoNmeaParser::isParseBufferFull()
{
  return (_parser_buf.elems_in_buf >= (NMEA_PARSE_BUFFER_SIZE - 1));
}

void ArduinoNmeaParser::addToParserBuffer(char const c)
{
  _parser_buf.buf[_parser_buf.elems_in_buf] = c;
  _parser_buf.elems_in_buf++;
}

void ArduinoNmeaParser::flushParserBuffer()
{
  _parser_buf.elems_in_buf = 0;
}

bool ArduinoNmeaParser::isCompleteNmeaMessageInParserBuffer()
{
  if (_parser_buf.elems_in_buf < 8) /* $GPxxx\r\n = 8 */
    return false;

  char const prev_last = _parser_buf.buf[_parser_buf.elems_in_buf - 2];
  char const      last = _parser_buf.buf[_parser_buf.elems_in_buf - 1];

  return ((prev_last == '\r') && (last == '\n'));
}

void ArduinoNmeaParser::terminateParserBuffer()
{
  addToParserBuffer('\0');
}

void ArduinoNmeaParser::parseGxRMC()
{
  nmea::GxRMC::parse(_parser_buf.buf, _rmc);

  if (_on_rmc_update)
    _on_rmc_update(_rmc);
}

void ArduinoNmeaParser::parseGxGGA()
{
  nmea::GxGGA::parse(_parser_buf.buf, _gga);

  if (_on_gga_update)
    _on_gga_update(_gga);
}
