/**
 *   Copyright (C) 2012-2013 ciere consulting, ciere.com
 *   Copyright (c) 2011-2015 Joel de Guzman
 *   Copyright (C) 2010, 2011 Object Modeling Designs
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CIERE_JSON_DETAIL_X3_GRAMMAR_DEF_HPP
#define CIERE_JSON_DETAIL_X3_GRAMMAR_DEF_HPP

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include "grammar.hpp"

#include <boost/fusion/adapted/std_pair.hpp>
#include <string>

namespace ciere { namespace json { namespace parser
{
   namespace x3 = boost::spirit::x3;
   namespace ascii = boost::spirit::x3::ascii;
   typedef boost::uint32_t uchar; // a unicode code point

   namespace detail
   {
      using x3::_attr;
      using x3::_val;

      auto push_utf8 = [](auto& ctx)
      {
         typedef std::back_insert_iterator<std::string> insert_iter;
         insert_iter out_iter(_val(ctx));
         boost::utf8_output_iterator<insert_iter> utf8_iter(out_iter);
         *utf8_iter++ = _attr(ctx);
      };

      auto push_esc = [](auto& ctx)
      {
         auto& utf8 = _val(ctx);
         switch (_attr(ctx))
         {
            case '"': utf8 += '"';          break;
            case '\\': utf8 += '\\';        break;
            case '/': utf8 += '/';          break;
            case 'b': utf8 += '\b';         break;
            case 'f': utf8 += '\f';         break;
            case 'n': utf8 += '\n';         break;
            case 'r': utf8 += '\r';         break;
            case 't': utf8 += '\t';         break;
         }
      };

      using x3::char_;
      using x3::hex;
      using x3::uint_parser;

      uint_parser<uchar, 16, 4, 4> const hex4 = {};

      auto const escape =
             ('u' > hex4)           [push_utf8]
         |   char_("\"\\/bfnrt")    [push_esc]
         ;

      auto const char_esc =
         '\\' > escape
         ;

      // a double quoted string containes 0 or more characters
      // where a character is:
      //     any-Unicode-character-except-"-or-\-or-control-character
      //
      auto const append = [](auto& ctx) { _val(ctx) += _attr(ctx); };
      
      auto const double_quoted =
           '"'
         > *(  char_esc
             | (char_("\x20\x21\x23-\x5b\x5d-\x7e")  )    [append]
            )
         > '"'
         ;
      
      struct unicode_string_class;
      typedef x3::rule<unicode_string_class, std::string> unicode_string_type;
      unicode_string_type const unicode_string = "unicode_string";
      
      auto const unicode_string_def = double_quoted;

      BOOST_SPIRIT_DEFINE(unicode_string);

   }  // end detail

   using x3::lit;
   using x3::double_;
   using x3::lexeme;
   using x3::attr;
   using ascii::char_;
   using ascii::digit;

   x3::int_parser<int64_t> const int_ = {};
   ascii::bool_type const bool_value = {};

   struct value_class;
   struct object_class;
   struct member_pair_class;
   struct array_class;
   struct string_class;

   typedef x3::rule<value_class, json::value> value_type;
   typedef x3::rule<object_class, json::object_t> object_type;
   typedef x3::rule<member_pair_class, json::object_member_t> member_pair_type;
   typedef x3::rule<array_class, json::array_t> array_type;
   
   value_type const value = "value";
   object_type const object = "object";
   member_pair_type const member_pair = "member_pair";
   array_type const array = "array";

   auto const null_value =
         lit("null")
      >> attr(json::null_t())
      ;

   auto const value_def =
        null_value
      | bool_value
      | detail::unicode_string
      | lexeme[!('+' | (-lit('-') >> '0' >> digit)) >> int_ >> !char_(".eE")]
      | lexeme[!('+' | (-lit('-') >> '0' >> digit)) >> double_]
      | object
      | array
      ;

   auto const object_def =
         lit('{')
      >> -(member_pair % ',')
      >> lit('}')
      ;

   auto const member_pair_def =
         detail::unicode_string
      >> ':'
      >> value
      ;

   auto const array_def =
         lit('[')
      >> -(value % ',')
      >> lit(']')
      ;

   BOOST_SPIRIT_DEFINE(
       value
     , object
     , member_pair
     , array
   );

}}}

#endif  // CIERE_JSON_DETAIL_GRAMMAR_DEF_HPP
