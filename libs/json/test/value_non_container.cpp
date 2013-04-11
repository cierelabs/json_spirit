/**
 *   Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 */

#define BOOST_TEST_MODULE value_non_container

#include "ciere/json/io.hpp"
#include "ciere/json/value.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

namespace json = ciere::json;

BOOST_AUTO_TEST_CASE(null_test)
{
   // default constructs to null
   json::value v1;
   json::value v2;
   BOOST_CHECK_EQUAL(v1, v2);
   BOOST_CHECK_EQUAL(v1.type(), json::null_type);
   BOOST_CHECK_EQUAL(v1, json::null_t());

   BOOST_CHECK_THROW(v1.length(), json::not_container_type);
}


BOOST_AUTO_TEST_CASE(bool_test)
{
   json::value v1(true);
   BOOST_CHECK_EQUAL(v1.type(), json::bool_type);

   json::value v2(false);
   BOOST_CHECK_EQUAL(v1.type(), json::bool_type);

   BOOST_CHECK_NE   (v1, v2);
   BOOST_CHECK_EQUAL(v1, true);
   BOOST_CHECK_EQUAL(v2, false);

   v1 = false;
   BOOST_CHECK_EQUAL(v1, v2);

   v1 = true;
   BOOST_CHECK_NE   (v1, false);

   BOOST_CHECK_THROW(v1.length(), json::not_container_type);
}


BOOST_AUTO_TEST_CASE(string_test)
{
   json::value v1("test");
   BOOST_CHECK_EQUAL(v1.type(), json::string_type);
   BOOST_CHECK_EQUAL(v1, "test");

   std::string s1("foo");
   json::value v2(s1);
   BOOST_CHECK_EQUAL(v2, s1);

   v1 = s1;
   BOOST_CHECK_EQUAL(v1, v2);

   v1 = "foo";
   BOOST_CHECK_EQUAL(v1, v2);

   v2 = "gorp";
   BOOST_CHECK_NE   (v1, v2);
   BOOST_CHECK_NE   (v1, std::string("bar"));

   BOOST_CHECK_THROW(v1.length(), json::not_container_type);
}


BOOST_AUTO_TEST_CASE(int_test)
{
   json::value v1(12);
   BOOST_CHECK_EQUAL(v1.type(), json::int_type);
   BOOST_CHECK_EQUAL(v1, 12);

   v1 = uint8_t(8);
   BOOST_CHECK_EQUAL(v1.type(), json::int_type);
   BOOST_CHECK_EQUAL(v1, 8);

   v1 = int8_t(-10);
   BOOST_CHECK_EQUAL(v1.type(), json::int_type);
   BOOST_CHECK_EQUAL(v1, -10);

   v1 = uint16_t(923);
   BOOST_CHECK_EQUAL(v1.type(), json::int_type);
   BOOST_CHECK_EQUAL(v1, 923);

   v1 = int16_t(-234);
   BOOST_CHECK_EQUAL(v1.type(), json::int_type);
   BOOST_CHECK_EQUAL(v1, -234);

   v1 = uint32_t(98);
   BOOST_CHECK_EQUAL(v1.type(), json::int_type);
   BOOST_CHECK_EQUAL(v1, 98);

   v1 = int32_t(-8);
   BOOST_CHECK_EQUAL(v1.type(), json::int_type);
   BOOST_CHECK_EQUAL(v1, -8);

   json::value v2(-8);
   BOOST_CHECK_EQUAL(v1, v2);
   BOOST_CHECK_EQUAL(v1, -8.0);

   BOOST_CHECK_THROW(v1.length(), json::not_container_type);
}


BOOST_AUTO_TEST_CASE(float_test)
{
   json::value v1(1.0);
   BOOST_CHECK_EQUAL(v1.type(), json::double_type);
   BOOST_CHECK_EQUAL(v1, 1.0);

   v1 = 987.5;
   BOOST_CHECK_EQUAL(v1, 987.5);
   BOOST_CHECK_NE   (v1, 987.1);

   json::value v2(987.5);
   BOOST_CHECK_EQUAL(v1, v2);

   double d1 = 4.56342124;
   json::value v3(d1);
   BOOST_CHECK_EQUAL(v3.type(), json::double_type);
   BOOST_CHECK_EQUAL(v3, d1);
   BOOST_CHECK_NE   (v3, v1);

   v1 = d1;
   BOOST_CHECK_EQUAL(v1, v3);

   float f1 = 123.4567;
   json::value v4(f1);
   BOOST_CHECK_EQUAL(v4.type(), json::double_type);
   BOOST_CHECK_EQUAL(v4, f1);
   BOOST_CHECK_NE   (v4, v1);
   v1 = f1;
   BOOST_CHECK_EQUAL(v1, v4);

   BOOST_CHECK_THROW(v1.length(), json::not_container_type);
}



BOOST_AUTO_TEST_CASE(value_test)
{
   json::value v1(1234);
   json::value v2(v1);
   BOOST_CHECK_EQUAL(v2.type(), json::int_type);
   BOOST_CHECK_EQUAL(v2, 1234);

   BOOST_CHECK_THROW(v1.length(), json::not_container_type);
}
