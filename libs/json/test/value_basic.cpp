/**
 *   Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 */

#define BOOST_TEST_MODULE value_basic


#include "ciere/json/io.hpp"
#include "ciere/json/value.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

BOOST_AUTO_TEST_CASE(value)
{
   ciere::json::value value;

   value = "test";
   BOOST_CHECK_EQUAL(value, "test");

   value = 42;
   BOOST_CHECK_EQUAL(value, 42);

   value = 42.3;
   BOOST_CHECK_EQUAL(value, 42.3);

   value = false;
   BOOST_CHECK_EQUAL(value, false);

   value = ciere::json::null_t();
   BOOST_CHECK_EQUAL(value, ciere::json::null_t());

   value = ciere::json::array()(123)("foo")("bar")(42.5);
   BOOST_CHECK_EQUAL(value[1], "foo");
   BOOST_CHECK_EQUAL(value[3], 42.5);

   value = ciere::json::object()
      ("number" ,123)
      ("foo"    ,"bar")
      ("meaning", 42);
   BOOST_CHECK_EQUAL(value["number"] , 123);
   BOOST_CHECK_EQUAL(value["foo"]    , "bar");
   BOOST_CHECK_EQUAL(value["meaning"], 42);
}
