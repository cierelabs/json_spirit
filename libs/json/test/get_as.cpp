/**
 *  Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 *  Copyright (C) 2010 - 2011  Object Modeling Designs
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_MODULE get_as

#include <boost/test/unit_test.hpp>
#include "ciere/json/io.hpp"
#include "ciere/json/value.hpp"

namespace json = ciere::json;

BOOST_AUTO_TEST_CASE(basic_get_as)
{
   json::value value;

   {
      value = "test";
      std::string tmp = value.get_as<json::string_t>();
      BOOST_CHECK_EQUAL(tmp, "test");
   }


   {
      value = 42;
      int tmp = value.get_as<json::int_t>();
      BOOST_CHECK_EQUAL(tmp, 42);
      std::string tmp_str = value.get_as<json::string_t>();
      BOOST_CHECK_EQUAL(tmp_str, "42");
      double tmp_dbl = value.get_as<json::float_t>();
      BOOST_CHECK_EQUAL(tmp_dbl, 42.0);
   }


   {
      value = 42.5;
      double tmp = value.get<json::float_t>();
      BOOST_CHECK_EQUAL(tmp, 42.5);
      std::string tmp_str = value.get_as<json::string_t>();
      BOOST_CHECK_EQUAL(tmp_str, "42.5");
      int tmp_int = value.get_as<json::int_t>();
      BOOST_CHECK_EQUAL(tmp_int, 42);
   }


   {
      value = false;
      bool tmp = value.get_as<json::bool_t>();
      BOOST_CHECK_EQUAL(tmp, false);
      int tmp_int = value.get_as<int>();
      BOOST_CHECK_EQUAL(tmp_int, 0);
   }

   {
      value = "true";
      BOOST_CHECK_EQUAL(true, value.get_as<json::bool_t>());
   }

   {
      value = 42;
      BOOST_CHECK_EQUAL(true, value.get_as<json::bool_t>());
      value = 0;
      BOOST_CHECK_EQUAL(false, value.get_as<json::bool_t>());
   }

   {
      value = "42";
      BOOST_CHECK_EQUAL(42, value.get_as<int>());
   }


   {
      value = json::null_t();
   }
}

