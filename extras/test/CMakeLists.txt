##########################################################################

cmake_minimum_required(VERSION 2.8)

##########################################################################

project(testNmeaParser)

##########################################################################

include_directories(../../src)
include_directories(external/catch/v2.12.1/include)

##########################################################################

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################################################

set(TEST_TARGET testNmeaParser)

set(TEST_SRCS
  src/ArduinoNmeaParser/test_OnGgaUpdateFunc.cpp
  src/ArduinoNmeaParser/test_OnRmcUpdateFunc.cpp
  src/test_ArduinoNmeaParser.cpp
  src/test_checksum.cpp
  src/test_GxGGA.cpp
  src/test_GxRMC.cpp
  src/test_Types.cpp
  src/test_main.cpp
  src/test_gga.cpp
  src/test_rmc.cpp

  ../../src/nmea/util/checksum.cpp
  ../../src/nmea/util/common.cpp
  ../../src/nmea/util/gga.cpp
  ../../src/nmea/util/rmc.cpp
  ../../src/nmea/util/timegm.c
  ../../src/nmea/GxGGA.cpp
  ../../src/nmea/GxRMC.cpp
  ../../src/nmea/Types.cpp
  ../../src/ArduinoNmeaParser.cpp
)

##########################################################################

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   "--coverage")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--coverage")

add_compile_definitions(HOST)

##########################################################################

add_executable(
  ${TEST_TARGET}
  ${TEST_SRCS}
)

##########################################################################
