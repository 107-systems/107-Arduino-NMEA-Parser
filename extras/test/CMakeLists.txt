##########################################################################

cmake_minimum_required(VERSION 2.8)

##########################################################################

project(testNmeaParser)

##########################################################################

include_directories(../../src/nmea)
include_directories(external/catch/v2.12.1/include)

##########################################################################

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################################################

set(TEST_TARGET testNmeaParser)

set(TEST_SRCS
  src/test_main.cpp
  src/test_GPRMC.cpp
  ../../src/nmea/Parser.cpp
)

##########################################################################

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   "--coverage")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--coverage")

##########################################################################

add_executable(
  ${TEST_TARGET}
  ${TEST_SRCS}
)

##########################################################################
