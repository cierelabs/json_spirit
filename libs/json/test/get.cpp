/**
 *  Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 *  Copyright (C) 2010 - 2011  Object Modeling Designs
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_MODULE get

#include <boost/test/unit_test.hpp>
#include "ciere/json/io.hpp"
#include "ciere/json/value.hpp"


namespace json = ciere::json;

BOOST_AUTO_TEST_CASE(basic_get)
{
   json::value value;

   {
      value = "test";
      std::string tmp = value.get<json::string_t>();
      BOOST_CHECK_EQUAL(tmp, "test");

      BOOST_CHECK_THROW(value.get<json::int_t>(), boost::bad_get /*json::get_as_error*/ );
   }


   {
      value = 42;
      int tmp = value.get<json::int_t>();
      BOOST_CHECK_EQUAL(tmp, 42);

      BOOST_CHECK_THROW(value.get<json::float_t>(), boost::bad_get /*json::get_as_error*/ );
   }


   {
      value = 42.3;
      double tmp = value.get<json::float_t>();
      BOOST_CHECK_EQUAL(tmp, 42.3);

      BOOST_CHECK_THROW(value.get<json::int_t>(), boost::bad_get /*json::get_as_error*/ );
   }



   {
      value = false;
      bool tmp = value.get<json::bool_t>();
      BOOST_CHECK_EQUAL(tmp, false);

      BOOST_CHECK_THROW(value.get<json::int_t>(), boost::bad_get /*json::get_as_error*/ );
   }
}

