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
#include <stdint.h>

#undef max
#undef min
#include <functional>

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef std::function<void(float const, float const, float const, float const, float const)> OnPositionUpdate;

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoNmeaParser
{

public:

  ArduinoNmeaParser(OnPositionUpdate on_position_update);


  void encode(char const c);


#ifdef HOST
  inline float latitude          () const { return _position.latitude; }
  inline float longitude         () const { return _position.longitude; }
  inline float speed             () const { return _position.speed; }
  inline float course            () const { return _position.course; }
  inline float last_fix_utc_s    () const { return _position.last_fix_utc_s; }
#endif
  inline float magnetic_variation() const { return _position.magnetic_variation; }
  inline int   day               () const { return _position.date.day; }
  inline int   month             () const { return _position.date.month; }
  inline int   year              () const { return _position.date.year; }

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

  typedef struct
  {
    int day;
    int month;
    int year;
  } Date;

  typedef struct
  {
    float latitude;
    float longitude;
    float speed;
    float course;
    float last_fix_utc_s;
    float magnetic_variation;
    Date  date;
  } PositionData;

  enum class ParserState
  {
    Synching, Synced
  };

  Error _error;
  ParserState _parser_state;
  ParserBuffer _parser_buf;
  PositionData _position;
  OnPositionUpdate _on_position_update;

  bool isParseBufferFull();
  void addToParserBuffer(char const c);
  void flushParserBuffer();
  bool isCompleteNmeaMessageInParserBuffer();
  void terminateParserBuffer();
  void parseGPRMC();

};

#endif /* ARDUINO_MTK3333_NMEA_PARSER_H_ */
