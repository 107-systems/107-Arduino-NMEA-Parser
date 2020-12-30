`107-Arduino-NMEA-Parser`
=========================
[![Unit Tests Status](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Unit%20Tests/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Unit+Tests)
[![codecov](https://codecov.io/gh/107-systems/107-Arduino-NMEA-Parser/branch/master/graph/badge.svg)](https://codecov.io/gh/107-systems/107-Arduino-NMEA-Parser)
[![Compile Examples](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Compile%20Examples/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Compile+Examples)
[![Extra Library Checks](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Extra%20Library%20Checks/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Extra+Library+Checks)
[![General Formatting Checks](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/General%20Formatting%20Checks/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=General+Formatting+Checks)
[![Spell Check](https://github.com/107-systems/107-Arduino-NMEA-Parser/workflows/Spell%20Check/badge.svg)](https://github.com/107-systems/107-Arduino-NMEA-Parser/actions?workflow=Spell+Check)

Arduino library for interfacing with any GPS, GLONASS, Galileo or GNSS module and interpreting its NMEA messages.

<p align="center">
  <a href="https://github.com/107-systems/Viper-Firmware"><img src="https://github.com/107-systems/.github/raw/main/logo/viper.jpg" width="40%"></a>
</p>

## Example
```C++
#include <ArduinoNmeaParser.h>
/* ... */
void onRmcUpdate(nmea::RmcData const rmc)
{
  Serial.print(rmc.time_utc.hour);
  Serial.print(":");
  Serial.print(rmc.time_utc.minute);
  Serial.print(":");
  Serial.print(rmc.time_utc.second);
  Serial.print(".");
  Serial.print(rmc.time_utc.microsecond);

  if (rmc.is_valid)
  {
    Serial.print(" : LON ");
    Serial.print(rmc.longitude);
    Serial.print(" ° | LAT ");
    Serial.print(rmc.latitude);
    Serial.print(" ° | VEL ");
    Serial.print(rmc.speed);
    Serial.print(" m/s | HEADING ");
    Serial.print(rmc.course);
    Serial.print(" °");
  }

  Serial.println();
}
/* ... */
ArduinoNmeaParser parser(onRmcUpdate, nullptr);
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
