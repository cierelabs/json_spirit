# Main CMake build control file for json_spirit
# Copyright (C) 2013 Jeff Trull <edaskel@att.net>
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# CMake build control file for json_spirit
# Created following settings in Jamroot and libs/json/test/Jamfile

cmake_minimum_required( VERSION 2.8 )

if( CMAKE_COMPILER_IS_GNUCXX )
  add_definitions( -ftemplate-depth=300 )
elseif( MSVC )
  add_definitions( /wd4996 )
endif()

find_package( Boost COMPONENTS unit_test_framework REQUIRED )
include_directories( . ${Boost_INCLUDE_DIRS} )

file( GLOB SRC_FILES libs/json/src/*.cpp )
add_library( json STATIC ${SRC_FILES} )

enable_testing()

add_subdirectory( libs/json/test )
