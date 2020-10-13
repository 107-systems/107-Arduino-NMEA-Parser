`107-Arduino-NMEA-Parser`
=========================
[![Unit Tests Status](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Unit%20Tests/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Unit+Tests)
[![codecov](https://codecov.io/gh/107-systems/107-Arduino-NMEA-Parser/branch/master/graph/badge.svg)](https://codecov.io/gh/107-systems/107-Arduino-NMEA-Parser)
[![Compile Examples](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Compile%20Examples/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Compile+Examples)
[![Extra Library Checks](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Extra%20Library%20Checks/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Extra+Library+Checks)
[![General Formatting Checks](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/General%20Formatting%20Checks/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=General+Formatting+Checks)
[![Spell Check](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Spell%20Check/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Spell+Check)

Arduino library for interfacing with any GPS module and interpreting its NMEA messages.

<p align="center">
  <a href="https://github.com/107-systems/107-Arduino-Viper"><img src="extras/logo/viper-logo.jpg" width="40%"></a>
</p>

## Example
```C++
#include <ArduinoNmeaParser.h>
/* ... */
void onPositionUpdate(float const last_fix_utc_s, float const latitude, float const longitude, float const speed, float const course)
{
  char msg[64] = {0};
  snprintf(msg, 64, "[%f] %f LON | %f LAT | %d m/s | %d °", last_fix_utc_s, latitude, longitude, speed, course);
  Serial.println(msg);
}
/* ... */
ArduinoNmeaParser parser(onPositionUpdate);
/* ... */
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}
/* ... */
void loop() {
  while (Serial1.available()) {
    parser.encode((char)Serial1.read());
  }
}
```
