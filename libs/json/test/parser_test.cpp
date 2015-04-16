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

#if !defined(CIERE_JSON_USE_SPIRIT_X3)
typedef ciere::json::parser::grammar<iter_t> grammar_t;
#else
auto const grammar = ciere::json::parser::value;
#endif

BOOST_TEST_DONT_PRINT_LOG_VALUE(iter_t)

BOOST_AUTO_TEST_CASE(basic_test_1)
{
#if !defined(CIERE_JSON_USE_SPIRIT_X3)
   boost::spirit::ascii::space_type space;
#else
   using boost::spirit::x3::ascii::space;
#endif

   std::string test_input( 
         "{"
         "  \"value\"  : 12.34,"
         "  \"on\"     : false,"
         "  \"jump\"   : null,"
         "  \"result\" : [1,34.5,{},\"Sosa did fine.\\u263A\",\"Snowman: \\u2603\"]"
         "}"
         );

#if !defined(CIERE_JSON_USE_SPIRIT_X3)
   grammar_t grammar;
#endif
   ciere::json::value value;

   iter_t iter = test_input.begin();
   iter_t end =  test_input.end();

#if !defined(CIERE_JSON_USE_SPIRIT_X3)
   BOOST_CHECK( boost::spirit::qi::phrase_parse( iter, end, grammar, space, value ) );
#else
   BOOST_CHECK( boost::spirit::x3::phrase_parse( iter, end, grammar, space, value ) );
#endif
   BOOST_CHECK_EQUAL(iter, end);
}

BOOST_AUTO_TEST_CASE(basic_test_2)
{
#if !defined(CIERE_JSON_USE_SPIRIT_X3)
   boost::spirit::ascii::space_type space;
#else
   using boost::spirit::x3::ascii::space;
#endif

   std::string test_input(
         "{"
         "  \"foo\":123.456,"
         "  \"the_value\":        42,"
         "  \"bar\":[1,\"some test \", {\"on\":false,\"object\":null}],"
         "  \"gorp\":\"how about this mom\""
         "}"
         );

#if !defined(CIERE_JSON_USE_SPIRIT_X3)
   grammar_t grammar;
#endif
   ciere::json::value value;

   iter_t iter = test_input.begin();
   iter_t end =  test_input.end();

#if !defined(CIERE_JSON_USE_SPIRIT_X3)
   BOOST_CHECK( boost::spirit::qi::phrase_parse( iter, end, grammar, space, value ) );
#else
   BOOST_CHECK( boost::spirit::x3::phrase_parse( iter, end, grammar, space, value ) );
#endif
   BOOST_CHECK_EQUAL(iter, end);
}

