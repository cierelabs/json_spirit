/**
 *  Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 *  Copyright (C) 2010 - 2011  Object Modeling Designs
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_MODULE parser_test

#include <boost/test/unit_test.hpp>
#include "ciere/json/io.hpp"
#include "ciere/json/value.hpp"
#include "ciere/json/parser/grammar.hpp"
#include "ciere/json/parser/grammar_def.hpp"

typedef std::string::const_iterator iter_t;
typedef ciere::json::parser::grammar<iter_t> grammar_t;

BOOST_TEST_DONT_PRINT_LOG_VALUE(iter_t)

BOOST_AUTO_TEST_CASE(basic_test_1)
{
   boost::spirit::ascii::space_type space;

   std::string test_input( 
         "{"
         "  \"value\"  : 12.34,"
         "  \"on\"     : false,"
         "  \"jump\"   : null,"
         "  \"result\" : [1,34.5,{},\"Sosa did fine.\\u263A\",\"Snowman: \\u2603\"]"
         "}"
         );

   grammar_t grammar;
   ciere::json::value value;

   iter_t iter = test_input.begin();
   iter_t end =  test_input.end();

   BOOST_CHECK( boost::spirit::qi::phrase_parse( iter, end, grammar, space, value ) );
   BOOST_CHECK_EQUAL(iter, end);
}

BOOST_AUTO_TEST_CASE(basic_test_2)
{
   boost::spirit::ascii::space_type space;

   std::string test_input(
         "{"
         "  \"foo\":123.456,"
         "  \"the_value\":        42,"
         "  \"bar\":[1,\"some test \", {\"on\":false,\"object\":null}],"
         "  \"gorp\":\"how about this mom\""
         "}"
         );

   grammar_t grammar;
   ciere::json::value value;

   iter_t iter = test_input.begin();
   iter_t end =  test_input.end();

   BOOST_CHECK( boost::spirit::qi::phrase_parse( iter, end, grammar, space, value ) );
   BOOST_CHECK_EQUAL(iter, end);
}

