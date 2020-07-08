/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "Parser.h"

#include <string.h>

#include "GPRMC.h"

#include "util/Checksum.h"
#include <iostream>
/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Parser::Parser()
: _error{Error::None}
, _parser_state{ParserState::Synching}
, _parser_buf{{0}, 0}
, _position{20.9860468, 52.2637009, 0.0, 0.0}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void Parser::encode(char const c)
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
  if (!util::isChecksumOk(_parser_buf.buf)) {
    _error = Error::Checksum;
    flushParserBuffer();
    return;
  }

  /* Parse the various NMEA messages. */
  if (GPRMC::isGPRMC(_parser_buf.buf)) parseGPRMC();

  /* The NMEA message has been fully processed and all
   * values updates so its time to flush the parser
   * buffer to make space for the next message.
   */
  flushParserBuffer();
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

bool Parser::isParseBufferFull()
{
  return (_parser_buf.elems_in_buf >= (NMEA_PARSE_BUFFER_SIZE - 1));
}

void Parser::addToParserBuffer(char const c)
{
  _parser_buf.buf[_parser_buf.elems_in_buf] = c;
  _parser_buf.elems_in_buf++;
}

void Parser::flushParserBuffer()
{
  _parser_buf.elems_in_buf = 0;
}

bool Parser::isCompleteNmeaMessageInParserBuffer()
{
  if (_parser_buf.elems_in_buf < 8) /* $GPxxx\r\n = 8 */
    return false;

  char const prev_last = _parser_buf.buf[_parser_buf.elems_in_buf - 2];
  char const      last = _parser_buf.buf[_parser_buf.elems_in_buf - 1];

  return ((prev_last == '\r') && (last == '\n'));
}

void Parser::terminateParserBuffer()
{
  addToParserBuffer('\0');
}

void Parser::parseGPRMC()
{
  uint32_t timestamp_fix_utc;

  if (!GPRMC::parse(_parser_buf.buf, timestamp_fix_utc, _position.latitude, _position.longitude, _position.speed, _position.course))
    _error = Error::RMC;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */
