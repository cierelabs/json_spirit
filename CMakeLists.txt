# Main CMake build control file for json_spirit
# Copyright (C) 2013 Jeff Trull <edaskel@att.net>
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# CMake build control file for json_spirit
# Created following settings in Jamroot and libs/json/test/Jamfile

cmake_minimum_required( VERSION 2.8.11 )

project( json_spirit )

option(JSON_SPIRIT_DEBUG_PARSER "Trace spirit parser in Debug config" OFF)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions( -std=c++11 )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions( -ftemplate-depth=300 )
  add_definitions( -std=c++0x )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions( /wd4996 )
endif()

if( CMAKE_HOST_APPLE )
  set (CMAKE_OSX_ARCHITECTURES "x86_64" )
endif()

# Enable V3 of Phoenix by default. V2 has some issues with C++11
# compilers.
add_definitions( -DBOOST_SPIRIT_USE_PHOENIX_V3=1 )

find_package( Boost COMPONENTS unit_test_framework REQUIRED )
include_directories( . )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

file( GLOB SRC_FILES libs/json/src/*.cpp )
add_library( json STATIC ${SRC_FILES} )

if(JSON_SPIRIT_DEBUG_PARSER)
  target_compile_definitions(
      json PUBLIC "$<$<CONFIG:Debug>:BOOST_SPIRIT_DEBUG>"
  )
endif()

enable_testing()

add_subdirectory( libs/json/test )
