/**
 *   Copyright (C) 2012, 2013 ciere consulting, ciere.com
 */

#define BOOST_TEST_MODULE value_object

#include <boost/test/unit_test.hpp>
#include "ciere/json/io.hpp"
#include "ciere/json/value.hpp"
#include <string>

namespace json = ciere::json;

BOOST_TEST_DONT_PRINT_LOG_VALUE(json::value::object_iterator)
BOOST_TEST_DONT_PRINT_LOG_VALUE(json::value::const_object_iterator)

void const_test_f( json::value const & value );

BOOST_AUTO_TEST_CASE(all)
{
   json::value value;

   BOOST_CHECK_EQUAL(value.type(), json::null_type);

   value = ciere::json::object()
      ("number" ,123)
      ("foo"    ,"bar")
      ("meaning", 42)
      ;

   BOOST_CHECK_EQUAL(value.type(), json::object_type);


   BOOST_CHECK_EQUAL(value["number"]  , 123);
   BOOST_CHECK_EQUAL(value["foo"]     , "bar");
   BOOST_CHECK_EQUAL(value["meaning"] , 42);

   value["number"] = 78.3;
   BOOST_CHECK_EQUAL(value["number"]  , 78.3);

   value["foo"] = ciere::json::object()
      ( "color",  "black" )
      ( "size" ,  93      )
      ( "tone" ,  ciere::json::null_t() )
      ;

   BOOST_CHECK_EQUAL(value["foo"]["size"]  , 93);
   BOOST_CHECK_EQUAL(value["foo"]["color"] , "black");

   value["foo"]["color"] = "blue";
   BOOST_CHECK_NE   (value["foo"]["color"] , "black");
   BOOST_CHECK_EQUAL(value["foo"]["color"] , "blue");
   BOOST_CHECK_EQUAL(value["foo"].length() , 3u);

   json::value::object_iterator iter     = value["foo"].begin_object();
   json::value::object_iterator iter_end = value["foo"].end_object();
   BOOST_CHECK_EQUAL(iter->name()  , "color");
   BOOST_CHECK_EQUAL(iter->value() , "blue");
   ++iter;
   BOOST_CHECK_EQUAL(iter->name()  , "size");
   BOOST_CHECK_EQUAL(iter->value() , 93);
   ++iter;
   BOOST_CHECK_EQUAL(iter->name()  , "tone");
   BOOST_CHECK_EQUAL(iter->value() , json::null_t());
   ++iter;
   BOOST_CHECK_EQUAL(iter , iter_end);

   value.set("bar", json::array()(42)("top")(1234.5) );
   BOOST_CHECK( value.has_key("bar") );
   BOOST_CHECK( !value.has_key("cafe") );
   BOOST_CHECK_EQUAL(value["bar"][2], 1234.5);

   value.set("cafe", "open");
   BOOST_CHECK_EQUAL(value["cafe"], "open");

   const_test_f(value);
}



void const_test_f( json::value const & value )
{
   BOOST_CHECK(value.has_key("cafe"));

   BOOST_CHECK_THROW(value["sam"], std::out_of_range);

   BOOST_CHECK_EQUAL(value["cafe"], "open");
   BOOST_CHECK_EQUAL(value["bar"][2], 1234.5);
}
