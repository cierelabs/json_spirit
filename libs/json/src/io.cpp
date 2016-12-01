/**
 *   Copyright (C) 2012 ciere consulting, ciere.com
 *
 *   Distributed under the Boost Software License, Version 1.0. (See accompanying
 *   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include "ciere/json/value.hpp"
#include "ciere/json/parser/grammar_def.hpp"

#include <boost/spirit/include/support_istream_iterator.hpp>

#include <string>
#include <vector>

using string_iter_t = std::string::const_iterator;
using vector_uint8_iter_t = std::vector<uint8_t>::const_iterator;

template struct ciere::json::parser::grammar<string_iter_t>;
template struct ciere::json::parser::grammar<boost::spirit::istream_iterator>;
template struct ciere::json::parser::grammar<vector_uint8_iter_t>;
