# CMake build control file for json_spirit unit tests
# Copyright (C) 2013, 2015 Jeff Trull <edaskel@att.net>
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
  target_link_libraries( ${tname} json ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
  add_test( ${tname} ${tname} )
endforeach()

# if fuzzing is enabled, produce the extra fuzzer target
if( JSON_SPIRIT_FUZZING )
  if( AFL )
    add_executable( fuzzer fuzz_afl.cpp )
  elseif( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ) AND LLVM_ROOT )
    # Build the libFuzzer-based fuzzer
    add_executable( fuzzer fuzz_lib.cpp )

    # and the library it depends on
    file(GLOB LIBFUZZ_SOURCES ${LLVM_ROOT}/lib/Fuzzer/*.cpp)
    include_directories( ${LLVM_ROOT}/lib/Fuzzer )
    add_library( fuzz ${LIBFUZZ_SOURCES} )
    target_link_libraries( fuzzer fuzz )

  else()
    message( FATAL "Don't know which fuzzer to build - afl compilers not in use and Clang+LLVM_ROOT not specified" )
  endif()

  target_link_libraries( fuzzer json_fuzz ${Boost_LOCALE_LIBRARY} )
  set_target_properties( fuzzer PROPERTIES COMPILE_FLAGS ${FUZZ_TARGET_COMPILE_FLAGS}
                                           LINK_FLAGS    ${FUZZ_TARGET_LINK_FLAGS} )

endif()
