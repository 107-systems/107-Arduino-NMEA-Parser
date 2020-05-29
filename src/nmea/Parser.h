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


  inline float longitude() const { return _position.longitude; }
  inline float latitude () const { return _position.latitude; }
  inline float speed    () const { return _position.speed; }
  inline float course   () const { return _position.course; }


private:

  static size_t constexpr NMEA_PARSE_BUFFER_SIZE = 82;

  typedef struct
  {
    char buf[NMEA_PARSE_BUFFER_SIZE];
    size_t elems_in_buf;
  } NMEAParseBuffer;

  typedef struct
  {
    float longitude;
    float latitude;
    float speed;
    float course; 
  } PositionData;
  

  NMEAParseBuffer _parse_buf;
  PositionData _position;

};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* nmea */

#endif /* ARDUINO_MTK3333_NMEA_PARSER_H_ */
