/**
 *   Copyright (C) 2013 ciere consulting, ciere.com
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CIERE_JSON_EXCEPTION_HPP
#define CIERE_JSON_EXCEPTION_HPP

#include <boost/exception/exception.hpp>
#include <exception>

namespace ciere { namespace json
{
   struct exception          : virtual boost::exception, virtual std::exception {};

   struct not_container_type : virtual json::exception    {};
   struct not_object_type    : virtual json::exception    {};
   struct not_array_type     : virtual json::exception    {};
   struct not_number_type    : virtual json::exception    {};
   struct get_error          : virtual json::exception    {};
   struct get_as_error       : virtual json::exception    {};
   struct parse_error        : virtual json::exception    {};
}}

#endif  // CIERE_JSON_EXCEPTION_HPP
