# Main CMake build control file for json_spirit
# Copyright (C) 2013,2015 Jeff Trull <edaskel@att.net>
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# CMake build control file for json_spirit
# Created following settings in Jamroot and libs/json/test/Jamfile

cmake_minimum_required( VERSION 2.8.11 )

project( json_spirit )

option(JSON_SPIRIT_DEBUG_PARSER "Trace spirit parser in Debug config" OFF)

option(JSON_SPIRIT_FUZZING "Enable build of fuzzing targets" OFF)
set( SANITIZER address )    # "memory" is the other supported (but probably buggy) option

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

if( JSON_SPIRIT_FUZZING )
  # the fuzzer needs the Boost Locale library to filter valid UTF-8 strings
  find_package( Boost COMPONENTS unit_test_framework locale REQUIRED )
else()
  find_package( Boost COMPONENTS unit_test_framework REQUIRED )
endif()

include_directories( . ${Boost_INCLUDE_DIRS} )

file( GLOB SRC_FILES libs/json/src/*.cpp )
add_library( json STATIC ${SRC_FILES} )
if( JSON_SPIRIT_FUZZING )
  # FUZZING USAGE:

  # libFuzzer (requires Clang and its tooling library):
  # cmake -DLLVM_ROOT=/path/to/llvm -DJSON_SPIRIT_FUZZING=ON ..;make
  # libs/json/test/fuzzer -max_len=256 -jobs=2 /path/to/corpus

  # AFL via Clang "fast" instrumentation
  # cmake -DCMAKE_CXX_COMPILER=/path/to/afl/afl-clang-fast++ -DJSON_SPIRIT_FUZZING=ON ..;make
  # /path/to/afl/afl-fuzz -i /path/to/corpus -o FINDINGS -m none -- libs/json/test/fuzzer

  # AFL via g++
  # cmake -DCMAKE_CXX_COMPILER=/path/to/afl/afl-g++ -DJSON_SPIRIT_FUZZING=ON ..;make
  # /path/to/afl/afl-fuzz -i /path/to/corpus -o FINDINGS -m none -- libs/json/test/fuzzer

  # create a version of the json library with the sanitizer enabled
  # for use by the fuzzer executable in the test directory
  add_library( json_fuzz STATIC ${SRC_FILES} )
  set( FUZZ_TARGET_COMPILE_FLAGS "-fsanitize=${SANITIZER}" )
  set( FUZZ_TARGET_LINK_FLAGS    "-fsanitize=${SANITIZER}" )
  # depending on the compiler and sanitizer, we may need to
  # add extra flags
  if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    # are we using the special AFL instrumented compiler?
    string(REGEX MATCH "afl-clang-fast\\+\\+$" AFL ${CMAKE_CXX_COMPILER})
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # or the gcc equivalent?
    string(REGEX MATCH "afl-g\\+\\+$" AFL ${CMAKE_CXX_COMPILER})
  else()
    message( FATAL "Fuzzing only works with gcc or Clang" )
  endif()

  if( AFL )
    if( SANITIZER EQUAL "address" )
      # add "hardening options"
      set( FUZZ_TARGET_COMPILE_FLAGS "${FUZZ_TARGET_COMPILE_FLAGS} -D_FORTIFY_SOURCE=2 -fstack-protector-all" )
    elseif( SANITIZER EQUAL "memory" )
      set( FUZZ_TARGET_COMPILE_FLAGS "${FUZZ_TARGET_COMPILE_FLAGS} -sanitize-memory-track-origins=2" )
    endif()
  elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    # assuming libFuzzer
    set( FUZZ_TARGET_COMPILE_FLAGS "${FUZZ_TARGET_COMPILE_FLAGS} -fsanitize-coverage=edge" )
    set( FUZZ_TARGET_LINK_FLAGS    "${FUZZ_TARGET_LINK_FLAGS}    -fsanitize-coverage=edge" )
  endif()

  if( WIN32 AND NOT AFL )
    message( FATAL "libFuzzer does not work under Windows, for now" )
  endif()

  set_target_properties( json_fuzz PROPERTIES COMPILE_FLAGS ${FUZZ_TARGET_COMPILE_FLAGS}
                                              LINK_FLAGS    ${FUZZ_TARGET_LINK_FLAGS} )

endif()

if(JSON_SPIRIT_DEBUG_PARSER)
  target_compile_definitions(
      json PUBLIC "$<$<CONFIG:Debug>:BOOST_SPIRIT_DEBUG>"
  )
endif()

enable_testing()

add_subdirectory( libs/json/test )
