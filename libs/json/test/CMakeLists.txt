# CMake build control file for json_spirit unit tests
# Copyright (C) 2013 Jeff Trull <edaskel@att.net>
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# CMake build control file for json_spirit tests
include_directories( ../../.. )

foreach( tname
    construct value_basic value_array value_object value_non_container get get_as parser_test 
)
  add_executable( ${tname} ${tname}.cpp )
  target_link_libraries( ${tname} 
    json 
    debug libboost_unit_test_framework-clang-darwin-mt-d-1_55.a
    optimized libboost_unit_test_framework-clang-darwin-mt-d-1_55.a
  )
  add_test( ${tname} ${tname} )
endforeach()

