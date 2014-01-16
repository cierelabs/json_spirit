# Main CMake build control file for json_spirit
# Copyright (C) 2013 Jeff Trull <edaskel@att.net>
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

cmake_minimum_required( VERSION 2.8 )

project( json_spirit )

message(STATUS "compiler is ${CMAKE_CXX_COMPILER_ID}" )

if( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
  add_definitions( -ftemplate-depth=300 )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
  add_definitions( /wd4996 )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
  set (CMAKE_CXX_FLAGS "-std=c++11 -DBOOST_SPIRIT_USE_PHOENIX_V3=1" )
  set (CMAKE_CXX_FLAGS_DEBUG "-g -O0" )
  set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O3" )
endif()

if( CMAKE_HOST_APPLE )
  set (CMAKE_OSX_ARCHITECTURES "x86_64" )
endif()

find_package(Boost)

# this didn't work for me
# find_package( Boost COMPONENTS unit_test_framework REQUIRED )

link_directories(
    "${BOOST_ROOT}/stage64/lib"
)

include_directories( . ${Boost_INCLUDE_DIRS} )

set(BUILD_SHARED_LIBS false)
file( GLOB SRC_FILES libs/json/src/*.cpp )
add_library( json STATIC ${SRC_FILES} )

enable_testing()

add_subdirectory( libs/json/test )
