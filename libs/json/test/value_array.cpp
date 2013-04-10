/**
 *   Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 */

#define BOOST_TEST_MODULE value_array

#include "ciere/json/io.hpp"
#include "ciere/json/value.hpp"
#include <boost/test/unit_test.hpp>
#include <string>


namespace json = ciere::json;

void const_test_f( json::value const & value );

BOOST_AUTO_TEST_CASE(basic_test)
{
   json::value v = json::array()(34)("foo")("bar")(1234.5);
   BOOST_CHECK_EQUAL(v.type(), json::array_type);
   BOOST_CHECK_EQUAL(v.length(), 4u);
   BOOST_CHECK_EQUAL(v[1], "foo");
   v[1] = "gorp";
   BOOST_CHECK_EQUAL(v[1], "gorp");
   BOOST_CHECK_EQUAL(v.at(1), "gorp");
   v.at(1) = "bar";
   BOOST_CHECK_EQUAL(v.at(1), "bar");

   v[8] = json::array()(0)(1)(2);
   BOOST_CHECK_EQUAL(v.length(), 9u);
   BOOST_CHECK_EQUAL(v[8].length(), 3u);
   BOOST_CHECK_EQUAL(v[8].type(), json::array_type);
   BOOST_CHECK_EQUAL(v[8][1], 1);

   v[8][1] = true;
   BOOST_CHECK_EQUAL(v[8][1], true);
   BOOST_CHECK_NE   (v[8][1], false);

   BOOST_CHECK_THROW(v.at(10),std::out_of_range);
}


BOOST_AUTO_TEST_CASE(reference_test)
{
   json::value v = json::array()
      (123)("foo")(false)(json::array()
                          (0)(1)(2)(3)(1234.5))
      ("bar")("gorp");

   v[4] = v[3];

   BOOST_CHECK_EQUAL(v[4].type(), json::array_type);
   BOOST_CHECK_EQUAL(v[4][2], 2);

   v[3][2] = "gorp";
   BOOST_CHECK_EQUAL(v[3][2], "gorp");
   BOOST_CHECK_NE(v[4][2], "gorp");
   BOOST_CHECK_EQUAL(v[4][2], 2);

   v[3] = 8;
   BOOST_CHECK_EQUAL(v[4], json::array()(0)(1)(2)(3)(1234.5) );
}


BOOST_AUTO_TEST_CASE(iterator_test)
{
   json::value v = json::array()(0)(1)(2)(3)(4)(5);
   json::value::array_iterator iter     = v.begin_array();
   json::value::array_iterator iter_end = v.end_array();
   int count = 0;
   while(iter != iter_end)
   {
      BOOST_CHECK_EQUAL(*iter, count++);
      *iter = count;
      ++iter;
   }
   BOOST_CHECK_EQUAL(count, 6);

   iter     = v.begin_array();
   iter_end = v.end_array();
   count = 1;
   while(iter != iter_end)
   {
      BOOST_CHECK_EQUAL(*iter, count++);
      ++iter;
   }
}

void const_test_f( json::value const & v )
{
   json::value::const_array_iterator iter     = v.begin_array();
   json::value::const_array_iterator iter_end = v.end_array();
   int count = 0;
   while(iter != iter_end)
   {
      BOOST_CHECK_EQUAL(*iter, count++);
#ifdef COMPILE_ERROR
      *iter = count;  // should create a compile error
#endif
      ++iter;
   }
   BOOST_CHECK_EQUAL(count, 5);
}

BOOST_AUTO_TEST_CASE(const_test)
{
   json::value v = json::array()(0)(1)(2)(3)(4);
   const_test_f(v);
}

