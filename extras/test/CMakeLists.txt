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
  src/test_ArduinoNmeaParser.cpp
  src/test_checksum.cpp
  src/test_GPRMC.cpp
  src/test_main.cpp

  ../../src/nmea/util/checksum.cpp
  ../../src/nmea/util/gprmc.cpp
  ../../src/nmea/GPRMC.cpp
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
