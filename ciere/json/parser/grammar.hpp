/**
 *   Copyright (C) 2012 ciere consulting, ciere.com
 *   Copyright (C) 2010, 2011 Object Modeling Designs
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CIERE_JSON_DETAIL_GRAMMAR_HPP
#define CIERE_JSON_DETAIL_GRAMMAR_HPP

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include "../value.hpp"

#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <string>

namespace ciere { namespace json { namespace parser
{
   namespace qi      = boost::spirit::qi;
   namespace ascii   = boost::spirit::ascii;

   typedef boost::uint32_t uchar; // a unicode code point

   namespace detail
   {
      struct push_utf8
      {
         template <typename Sig>
         struct result { typedef void type; };

         push_utf8(uchar* code_point): prev_code_point(code_point) {}
         void operator()(std::string& utf8, uchar code_point) const;
         mutable uchar* prev_code_point;
      };

      struct push_esc
      {
         template <typename Sig>
         struct result { typedef void type; };

         push_esc(uchar* code_point): prev_code_point(code_point) {}
         void operator()(std::string& utf8, uchar c) const;
         mutable uchar* prev_code_point;
      };

      struct push_char
      {
         template <typename Sig>
         struct result { typedef void type; };

         push_char(uchar* code_point): prev_code_point(code_point) {}
         void operator()(std::string& utf8, uchar c) const;
         mutable uchar* prev_code_point;
      };

      struct check
      {
         template <typename Sig>
         struct result { typedef void type; };

         check(uchar* code_point): prev_code_point(code_point) {}
         void operator()(void) const;
         mutable uchar* prev_code_point;
      };

      template <typename Iterator>
      struct unicode_string : qi::grammar<Iterator, std::string()>
      {
         unicode_string();
         qi::rule<Iterator, void(std::string&)> escape;
         qi::rule<Iterator, void(std::string&)> char_esc;
         qi::rule<Iterator, std::string()> double_quoted;
         uchar code_point;
         push_utf8 push_utf8_;
         push_esc push_esc_;
         push_char push_char_;
         check check_;
      };
   }


   template< typename Iterator >
   struct grammar : qi::grammar< Iterator, json::value(), ascii::space_type >
   {
      grammar();
      qi::rule< Iterator, json::value()           , ascii::space_type > value;
      qi::rule< Iterator, json::object_t()        , ascii::space_type > object;
      qi::rule< Iterator, json::object_member_t() , ascii::space_type > member_pair;
      qi::rule< Iterator, json::array_t()         , ascii::space_type > array;
      qi::rule< Iterator, json::int_t()           , ascii::space_type > int_value;
      qi::rule< Iterator, json::double_t()        , ascii::space_type > double_value;
      detail::unicode_string< Iterator                                > string_value;
      qi::rule< Iterator, json::null_t()          , ascii::space_type > null_value;
   };

}}}

#endif  // CIERE_JSON_DETAIL_GRAMMAR_HPP
