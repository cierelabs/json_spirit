/**
 *   Copyright (C) 2012 - 2013 ciere consulting, ciere.com
 *   Copyright (C) 2011, 2012  Object Modeling Designs
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CIERE_JSON_DETAIL_IO_IMPL_HPP
#define CIERE_JSON_DETAIL_IO_IMPL_HPP

#include "../exception.hpp"
#include "../parser/grammar.hpp"
#include "../parser/grammar_def.hpp"

#include <boost/foreach.hpp>
#include <boost/spirit/include/qi_expect.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_parse_attr.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/variant/static_visitor.hpp>

#include <ios>
#include <istream>
#include <string>
#include <vector>

using string_iter_t = std::string::const_iterator;
using vector_uint8_iter_t = std::vector<uint8_t>::const_iterator;
extern template struct ciere::json::parser::grammar<string_iter_t>;
extern template struct ciere::json::parser::grammar<boost::spirit::istream_iterator>;
extern template struct ciere::json::parser::grammar<vector_uint8_iter_t>;


namespace ciere { namespace json
{
   namespace spirit = boost::spirit;

   namespace detail
   {
      struct printer : public boost::static_visitor<>
      {
         printer(std::ostream& s) : stream(s) {}

         void operator()(string_t const & utf) const
         {
            stream << '"';

            typedef ::boost::uint32_t ucs4_char;
            typedef boost::u8_to_u32_iterator<std::string::const_iterator> iter_t;
            iter_t f = utf.begin();
            iter_t l = utf.end();

            for(iter_t i = f; i != l; ++i)
            {
               ucs4_char c = *i;
               switch (c)
               {
                  case '"':   stream << "\\\""; break;
                  case '\\':  stream << "\\\\"; break;
                  case '/':   stream << "\\/";  break;
                  case '\b':  stream << "\\b";  break;
                  case '\f':  stream << "\\f";  break;
                  case '\n':  stream << "\\n";  break;
                  case '\r':  stream << "\\r";  break;
                  case '\t':  stream << "\\t";  break;

                  default:    stream << boost::spirit::to_utf8(c);
               }
            }

            stream << '"';
         }

         template< typename T >
         void operator()(T const & v) const
         {
            stream << v;
         }

         void operator()(double d) const
         {
            // javascript's handling of NaN and +/-Infinity
            // isn't so great. JSON simply follows the javascript
            // standard. We can output nan and infinity; however,
            // we cannot actually parse it back in afaict because
            // the javascript side is generating a null?
            //
            // TODO: clear this up with something definitive
            if(boost::math::isnan(d))
            {
               stream << "NaN";
               return;
            }
            if(boost::math::isinf(d))
            {
               if(d < 0.0) { stream << '-'; }
               stream << "Infinity";
               return;
            }
            stream << d;
         }

         void operator()(bool_t v) const
         {
            stream << (v?"true":"false");
         }

         void operator()(null_t) const
         {
            stream << "null";
         }

         void operator()(object_t const & obj) const
         {
            stream << "{";
            bool first = true;

            BOOST_FOREACH( object_t::value_type const & v, obj )
            {
               if( first ) { first = false;  }
               else        { stream << ", "; }

               stream << '"' << v.first << "\":";
               boost::apply_visitor( *this,v.second);
            }

            stream << "}";
         }

         void operator()(array_t const & arr) const
         {
            stream << "[";
            bool first = true;

            BOOST_FOREACH( value const & v, arr )
            {
               if( first ) { first = false;  }
               else        { stream << ", "; }

               boost::apply_visitor(*this,v);
            }
            stream << "]";
         }

         std::ostream& stream;
      };
   }

   inline std::ostream & operator<<(std::ostream & stream, null_t)
   {
      stream << "null";
      return stream;
   }

   inline std::ostream & operator<<(std::ostream & stream, value const & v)
   {
      boost::apply_visitor(detail::printer(stream),v);
      return stream;
   }

   inline std::istream & operator>>(std::istream & stream, value & object)
   {
      if( !json::construct( stream, object ) )
      {
         stream.setstate( std::ios_base::failbit );
      }

      return stream;
   }

   inline bool construct(std::istream & input, value & v)
   {
      typedef spirit::istream_iterator iterator_t;
      typedef parser::grammar<iterator_t> grammar_t;

      bool skipws_was_set = (input.flags() & std::ios::skipws) != 0;
      input.unsetf(std::ios::skipws);

      iterator_t iterator(input), end;

      grammar_t grammar;
      bool parse_success = false;
      try
      {
         parse_success = spirit::qi::phrase_parse( iterator, end
                                                 , grammar
                                                 , spirit::ascii::space_type()
                                                 , v );
      }
      catch(spirit::qi::expectation_failure<iterator_t> const &){}

      if(skipws_was_set)
      {
         input.unsetf(std::ios::skipws);
      }

      return parse_success;
   }

   inline value construct(std::istream & input)
   {
      value v;

      if(construct(input, v) == false)
      {
         throw parse_error();
      }

      return v;
   }

   template<typename Iterator>
   bool construct(Iterator & iter, Iterator const & iter_end, value & v)
   {
      typedef parser::grammar<Iterator> grammar_t;
      grammar_t grammar;

      bool parse_success = false;
      try
      {
         parse_success = spirit::qi::phrase_parse( iter, iter_end
                                                 , grammar
                                                 , spirit::ascii::space_type()
                                                 , v );
      }
      catch(spirit::qi::expectation_failure<Iterator> const &){}

      return parse_success;
   }


   template<typename Iterator>
   value construct(Iterator & iter, Iterator const & iter_end)
   {
      value v;

      if(construct(iter, iter_end, v) == false)
      {
         throw parse_error();
      }

      return v;
   }

   inline bool construct(std::string const & input, value & v, bool parse_complete)
   {
      typedef std::string::const_iterator iterator_t;
      iterator_t iter     = input.begin();
      iterator_t iter_end = input.end();

      bool parse_success = construct(iter, iter_end, v);

      return ( parse_success
               &&
               ( !parse_complete || iter == iter_end ) );
   }

   inline value construct(std::string const & input, bool parse_complete)
   {
      value v;

      if(construct(input, v, parse_complete) == false)
      {
         throw parse_error();
      }

      return v;
   }
}}

#endif // CIERE_JSON_DETAIL_IO_IMPL_HPP
