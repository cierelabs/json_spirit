/**
 *   Copyright (C) 2012, 2013 ciere consulting, ciere.com
 *   Copyright (C) 2011, 2012 Object Modeling Designs
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CIERE_JSON_IO_HPP
#define CIERE_JSON_IO_HPP

#include "value.hpp"

#include <iostream>
#include <string>

namespace ciere { namespace json
{
   std::ostream & operator<<(std::ostream & stream, null_t);
   std::ostream & operator<<(std::ostream & stream, value const & value);
   std::istream & operator>>(std::istream & stream, value & object);

   /**
    * Construct a JSON value from the input stream in a non-throwing fashion.
    */
   bool construct(std::istream & input, value & value);

   /**
    * Construct a JSON value from the input stream.
    */
   value construct(std::istream & input);


   /**
    *  Construct a JSON value from the input iterators. Returns false if the
    *  parse fails.
    **/
   template<typename Iterator>
   bool construct(Iterator & iter, Iterator const & iter_end, value & v);


   /**
    *  Construct a JSON value from the input iterators. Returns the json::value
    *  representing the first json construct. Throws json::parse_error
    *  if parsing failed.
    **/
   template<typename Iterator>
   value construct(Iterator & iter, Iterator const & iter_end);


   /**
    *  Construct a JSON value from the input string in a non-throwing fashion.
    **/
   bool construct(std::string const & input, value & v, bool parse_complete=false);

   /**
    *  Construct a JSON value from the input string.
    */
   value construct(std::string const & input, bool parse_complete=false);
}}

#include "detail/io_impl.hpp"

#endif  // CIERE_JSON_IO_HPP
