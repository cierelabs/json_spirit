/**
 *  Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 *  Copyright (C) 2010 - 2011  Object Modeling Designs
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_MODULE construct

#include <boost/test/unit_test.hpp>
#include <ciere/json/io.hpp>
#include <ciere/json/value.hpp>

#include <string>

BOOST_AUTO_TEST_CASE(null)
{
   using namespace ciere;

   BOOST_CHECK_EQUAL(json::construct("null"), json::null_t());
}

BOOST_AUTO_TEST_CASE(bool_)
{
   using namespace ciere;

    BOOST_CHECK_EQUAL(json::construct("true"), json::value{ true });
    BOOST_CHECK_EQUAL(json::construct("false"), json::value{ false });
}

BOOST_AUTO_TEST_CASE(number)
{
   using namespace ciere;

   BOOST_CHECK_THROW(json::construct("+"), json::parse_error);
   BOOST_CHECK_THROW(json::construct("-"), json::parse_error);

   BOOST_CHECK_EQUAL(json::construct("0"), json::value{ 0 });
   BOOST_CHECK_THROW(json::construct("+0"), json::parse_error);
   BOOST_CHECK_EQUAL(json::construct("-0"), json::value{ 0 });

   BOOST_CHECK_EQUAL(json::construct("23"), json::value{ 23 });
   BOOST_CHECK_THROW(json::construct("+23"), json::parse_error);
   BOOST_CHECK_EQUAL(json::construct("-23"), json::value{ -23 });

   BOOST_CHECK_THROW(json::construct("+ 23"), json::parse_error);
   BOOST_CHECK_THROW(json::construct("- 23"), json::parse_error);

   BOOST_CHECK_THROW(json::construct("2 3",true), json::parse_error);

   BOOST_CHECK_THROW(json::construct("023"), json::parse_error);
   BOOST_CHECK_THROW(json::construct("+023"), json::parse_error);
   BOOST_CHECK_THROW(json::construct("-023"), json::parse_error);
}

BOOST_AUTO_TEST_CASE(string)
{
   using namespace ciere;

   BOOST_CHECK_EQUAL(json::construct("\"\""), json::value{ "" });
   BOOST_CHECK_EQUAL(json::construct("\" \""), json::value{ " " });
   BOOST_CHECK_EQUAL(json::construct("\"X\""), json::value{ "X" });
   BOOST_CHECK_EQUAL(json::construct("\"X \""), json::value{ "X " });
   BOOST_CHECK_EQUAL(json::construct("\" X \""), json::value{ " X " });

   BOOST_CHECK_THROW(json::construct("\"\"\"",true), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\\\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\x00\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\f\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\n\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\r\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\t\""), json::parse_error);

   BOOST_CHECK_EQUAL(json::construct("\"\\\"\""), json::value{ "\"" });
   BOOST_CHECK_EQUAL(json::construct("\"\\\\\""), json::value{ "\\" });
   BOOST_CHECK_EQUAL(json::construct("\"\\/\""), json::value{ "/" });
   BOOST_CHECK_EQUAL(json::construct("\"\\b\""), json::value{ "\b" });
   BOOST_CHECK_EQUAL(json::construct("\"\\f\""), json::value{ "\f" });
   BOOST_CHECK_EQUAL(json::construct("\"\\n\""), json::value{ "\n" });
   BOOST_CHECK_EQUAL(json::construct("\"\\r\""), json::value{ "\r" });
   BOOST_CHECK_EQUAL(json::construct("\"\\t\""), json::value{ "\t" });

   BOOST_CHECK_THROW(json::construct("\"\\u\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\\u0\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\\u00\""), json::parse_error);
   BOOST_CHECK_THROW(json::construct("\"\\u000\""), json::parse_error);
   BOOST_CHECK_EQUAL(json::construct("\"\\u0000\""), json::value{ std::string("\x00", 1) });
   BOOST_CHECK_EQUAL(
                     json::construct("\"\\u2603\""), json::value{ std::string("\xe2\x98\x83") });
   BOOST_CHECK_EQUAL(
                     json::construct("\"\\u26030\""), json::value{ std::string("\xe2\x98\x83\x30") });

   json::value v;
   BOOST_CHECK_EQUAL(json::construct("[8,42.5,true] \"foo\"", v       ), true);
   BOOST_CHECK_EQUAL(json::construct("[8,42.5,true] \"foo\"", v, false), true);
   BOOST_CHECK_EQUAL(json::construct("[8,42.5,true] \"foo\"", v, true ), false);

   BOOST_CHECK_EQUAL(json::construct("{ \"foo\" : \"bar\"}", v , true ), true);
   BOOST_CHECK_EQUAL(json::construct("[8,42.5,true] \"foo\"", v, false), true);
   BOOST_CHECK_EQUAL(json::construct("[8,42.5,true] \"foo\"", v, true ), false);


   json::construct(
      "{"
      "   \"foo\"  : 42    ,"
      "   \"bar\"  : 498.5 ,"
      "   \"gorp\" : \"ciere\","
      "   \"bork\" : [\"ciere\",23,false]"
      "}"
      , v
      );

   BOOST_CHECK_EQUAL( v["foo"], json::value{ 42 });
   BOOST_CHECK_EQUAL( v["bar"], json::value{ 498.5 });
   BOOST_CHECK_EQUAL( v["gorp"], json::value{ "ciere" });
   BOOST_CHECK_EQUAL( v["bork"][2], json::value{ false });

}
