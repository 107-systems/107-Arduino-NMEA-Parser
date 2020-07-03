/**
 * @brief Arduino library for interfacing with the PA1010D GPS module (MTK3333 chipset).
 * @license LGPL 3.0
 */

#ifndef ARDUINO_MTK3333_NMEA_PARSER_H_
#define ARDUINO_MTK3333_NMEA_PARSER_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <stdlib.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace nmea
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Parser
{

public:

  Parser();


  void encode(char const c);


  inline float latitude () const { return _position.latitude; }
  inline float longitude() const { return _position.longitude; }
  inline float speed    () const { return _position.speed; }
  inline float course   () const { return _position.course; }


  enum class Error { None, RMC };

  inline void  clearerr()       { _error = Error::None; }
  inline Error error   () const { return _error; }


private:

  static size_t constexpr NMEA_PARSE_BUFFER_SIZE = 82 + 1; /* Leave space for the '\0' terminator */

  typedef struct
  {
    char buf[NMEA_PARSE_BUFFER_SIZE];
    size_t elems_in_buf;
  } ParserBuffer;

  typedef struct
  {
    float latitude;
    float longitude;
    float speed;
    float course; 
  } PositionData;

  enum class ParserState
  {
    Synching, Synced
  };
  
  Error _error;
  ParserState _parser_state;
  ParserBuffer _parser_buf;
  PositionData _position;

  bool isParseBufferFull();
  void addToParserBuffer(char const c);
  void flushParserBuffer();
  bool isCompleteNmeaMessageInParserBuffer();
  void terminateParserBuffer();
  bool isGPRMC();
  void parseGPRMC();

};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_MTK3333_NMEA_PARSER_H_ */
