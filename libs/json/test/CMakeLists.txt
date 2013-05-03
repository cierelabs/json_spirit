# CMake build control file for json_spirit unit tests
# Copyright (C) 2013 Jeff Trull <edaskel@att.net>
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# CMake build control file for json_spirit tests
include_directories( ../../.. )

add_definitions( -DBOOST_TEST_DYN_LINK )

if( CMAKE_HOST_WIN32 )
  # Copy Boost dlls to the test binary directories so they can be found
  set( blibs ${Boost_LIBRARIES} )
  while( blibs )
    list( GET blibs 0 cfg )
    list( GET blibs 1 libpath )
    # create the corresponding dll name (libpath will be .lib)
    get_filename_component( dllname ${libpath} NAME_WE )
    get_filename_component( dllpath ${libpath} PATH )
    set( dllpath "${dllpath}/${dllname}.dll" )
    if( cfg STREQUAL optimized )
      file( COPY ${dllpath} DESTINATION Release )
    else()
      file( COPY ${dllpath} DESTINATION Debug )
    endif()
    list( REMOVE_AT blibs 0 1 )
  endwhile()
endif()

foreach( tname
    construct value_basic value_array value_object value_non_container get get_as parser_test )
  add_executable( ${tname} ${tname}.cpp )
  target_link_libraries( ${tname} json ${Boost_LIBRARIES} )
  add_test( ${tname} ${tname} )
endforeach()

