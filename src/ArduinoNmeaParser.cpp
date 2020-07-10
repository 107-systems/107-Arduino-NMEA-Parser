/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "ArduinoNmeaParser.h"

#include <string.h>

#include "GPRMC.h"

#include "util/Checksum.h"

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoNmeaParser::ArduinoNmeaParser(OnPositionUpdate on_position_update)
: _error{Error::None}
, _parser_state{ParserState::Synching}
, _parser_buf{{0}, 0}
, _position{20.9860468, 52.2637009, 0.0, 0.0, 0.0}
, _on_position_update{on_position_update}
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
  if (nmea::GPRMC::isGPRMC(_parser_buf.buf)) parseGPRMC();

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

void ArduinoNmeaParser::parseGPRMC()
{
  if (!nmea::GPRMC::parse(_parser_buf.buf, _position.last_fix_utc_s, _position.latitude, _position.longitude, _position.speed, _position.course))
    _error = Error::RMC;
  else {
    if (_on_position_update)
      _on_position_update(_position.last_fix_utc_s, _position.latitude, _position.longitude, _position.speed, _position.course);
  }
}
