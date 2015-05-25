/**
 *   Copyright (C) 2012 ciere consulting, ciere.com
 *   Copyright (C) 2010, 2011 Object Modeling Designs
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CIERE_JSON_DETAIL_X3_GRAMMAR_HPP
#define CIERE_JSON_DETAIL_X3_GRAMMAR_HPP

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <ciere/json/value.hpp>
#include <boost/spirit/home/x3.hpp>
#include <string>

namespace ciere { namespace json { namespace parser
{
   namespace x3 = boost::spirit::x3;
   
   struct json_class;
   typedef x3::rule<json_class, json::value> json_type;
   typedef json_type::id json_id;
   BOOST_SPIRIT_DECLARE(json_type);

}}}

#endif  // CIERE_JSON_DETAIL_GRAMMAR_HPP
