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
, _parser_buf{0}
, _parser_buf_elems{0}
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
  /* Flash the whole parser buffer every time we encounter
   * a '$' sign. This way the parser buffer always starts
   * with a valid NMEA message.
   */
  if (c == '$')
    flushParserBuffer();

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
  if (!nmea::util::isChecksumOk(_parser_buf)) {
    _error = Error::Checksum;
    flushParserBuffer();
    return;
  }

  /* Parse the various NMEA messages. */
  if      (nmea::util::rmc_isGxRMC(_parser_buf)) parseGxRMC();
  else if (nmea::util::gga_isGxGGA(_parser_buf)) parseGxGGA();

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
  return (_parser_buf_elems >= (NMEA_PARSE_BUFFER_SIZE - 1));
}

void ArduinoNmeaParser::addToParserBuffer(char const c)
{
  _parser_buf[_parser_buf_elems] = c;
  _parser_buf_elems++;
}

void ArduinoNmeaParser::flushParserBuffer()
{
  _parser_buf_elems = 0;
}

bool ArduinoNmeaParser::isCompleteNmeaMessageInParserBuffer()
{
  /* Temporarily terminate string with a '\0' terminator in
   * order to be able to use string library functions.
   */
  _parser_buf[_parser_buf_elems] = '\0';

  /* Determine whether or not there exists a '$' marking
   * the start of a NMEA message.
   */
  char const * nmea_start = strchr(_parser_buf, '$');
  if (!nmea_start)
    return false;

  /* Now that we've got a '$' marking the start of a NMEA
   * message it is time to check if there's also an end
   * to it.
   */
  char const * nmea_stop_cr = strchr(nmea_start, '\r');
  if (!nmea_stop_cr)
    return false;

  char const * nmea_stop_lf = strchr(nmea_start, '\n');
  if (!nmea_stop_lf)
    return false;

  /* Lastly: check if the LF is really directly following
   * the CR within the NMEA message.
   */
  return ((nmea_stop_cr + 1) == nmea_stop_lf);
}

void ArduinoNmeaParser::terminateParserBuffer()
{
  addToParserBuffer('\0');
}

void ArduinoNmeaParser::parseGxRMC()
{
  nmea::GxRMC::parse(_parser_buf, _rmc);

  if (_on_rmc_update)
    _on_rmc_update(_rmc);
}

void ArduinoNmeaParser::parseGxGGA()
{
  nmea::GxGGA::parse(_parser_buf, _gga);

  if (_on_gga_update)
    _on_gga_update(_gga);
}
