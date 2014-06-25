/**
 *   Copyright (C) 2012-2013 ciere consulting, ciere.com
 *   Copyright (c) 2011 Joel de Guzman
 *   Copyright (C) 2010, 2011 Object Modeling Designs
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CIERE_JSON_DETAIL_GRAMMAR_DEF_HPP
#define CIERE_JSON_DETAIL_GRAMMAR_DEF_HPP

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include "grammar.hpp"

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <string>

#include <ciere/json/io.hpp> // BOOST_SPIRIT_DEBUG_NODE

namespace ciere { namespace json { namespace parser
{

   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;
   namespace phoenix = boost::phoenix;

   typedef boost::uint32_t uchar; // a unicode code point

   namespace detail
   {
      struct push_utf8
      {
         template <typename Sig>
         struct result { typedef void type; };

         void operator()(std::string& utf8, uchar code_point) const
         {
            typedef std::back_insert_iterator<std::string> insert_iter;
            insert_iter out_iter(utf8);
            boost::utf8_output_iterator<insert_iter> utf8_iter(out_iter);
            *utf8_iter++ = code_point;
         }
      };

      struct push_esc
      {
         template <typename Sig>
         struct result { typedef void type; };

         void operator()(std::string& utf8, uchar c) const
         {
            switch (c)
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
         }
      };


      template< typename Iterator >
      unicode_string<Iterator>::unicode_string()
         : unicode_string::base_type(double_quoted)
      {
         qi::char_type char_;
         qi::_val_type _val;
         qi::_r1_type _r1;
         qi::_1_type _1;
         qi::lit_type lit;
         qi::repeat_type repeat;
         qi::hex_type hex;
         qi::standard::cntrl_type cntrl;

         using boost::spirit::qi::uint_parser;
         using boost::phoenix::function;

         uint_parser<uchar, 16, 4, 4> hex4;
         function<detail::push_utf8> push_utf8;
         function<detail::push_esc> push_esc;

         escape =
                ('u' > hex4)           [push_utf8(_r1, _1)]
            |   char_("\"\\/bfnrt")    [push_esc(_r1, _1)]
            ;

         char_esc =
            '\\' > escape(_r1)
            ;

         // a double quoted string containes 0 or more characters
         // where a character is:
         //     any-Unicode-character-except-"-or-\-or-control-character
         //
         double_quoted =
              '"'
            > *(  char_esc(_val)
                | (char_ - '"' - '\\' - cntrl)    [_val += _1]
               )
            > '"'
            ;

         BOOST_SPIRIT_DEBUG_NODE(escape);
         BOOST_SPIRIT_DEBUG_NODE(char_esc);
         BOOST_SPIRIT_DEBUG_NODE(double_quoted);
      }

   }  // end detail


   template< typename Iterator >
   grammar<Iterator>::grammar()
      : grammar::base_type( value )
   {
      qi::lit_type             lit;
      ascii::char_type         char_;
      ascii::digit_type        digit;
      qi::double_type          double_;
      qi::int_parser<int64_t>  int_;
      qi::bool_type            bool_value;
      qi::attr_type            attr;
      qi::lexeme_type          lexeme;

      value =
           null_value
         | bool_value
         | string_value
         | lexeme[!('+' | (-lit('-') >> '0' >> digit)) >> int_ >> !char_(".eE")]
         | lexeme[!('+' | (-lit('-') >> '0' >> digit)) >> double_]
         | object
         | array
         ;

      object =
            lit('{')
         >> -(member_pair % ',')
         >> lit('}')
         ;

      member_pair =
            string_value
         >> ':'
         >> value
         ;

      array =
            lit('[')
         >> -(value % ',')
         >> lit(']')
         ;

      null_value =
            lit("null")
         >> attr(json::null_t())
         ;

      BOOST_SPIRIT_DEBUG_NODE(value);
      BOOST_SPIRIT_DEBUG_NODE(object);
      BOOST_SPIRIT_DEBUG_NODE(member_pair);
      BOOST_SPIRIT_DEBUG_NODE(array);
      BOOST_SPIRIT_DEBUG_NODE(null_value);
   }

}}}

#endif  // CIERE_JSON_DETAIL_GRAMMAR_DEF_HPP
