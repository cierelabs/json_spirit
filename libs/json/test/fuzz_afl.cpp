/**
 *  Copyright (C) 2015 Jeff Trull <edaskel@att.net>
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// "handle one input" standalone program for AFL
// very similar to the libFuzzer version

#include <sstream>
#include <string>
#include <iostream>

#include <boost/locale.hpp>

#include "ciere/json/value.hpp"
#include "ciere/json/io.hpp"

int main() {
   using namespace boost::locale::conv;
   using namespace ciere;

    // stream std::cin into a string
    std::string s((std::istreambuf_iterator<char>(std::cin)),
                  (std::istreambuf_iterator<char>()));

   // reject inputs with non-ASCII characters outside of strings
   for (auto sbeg = s.begin(); sbeg != s.end(); ++sbeg) {
      if (*sbeg & 0x80) {
         // bit 7 set -> non-ASCII
         return 0;
      }
      // quoted strings can have non-ASCII if they are valid UTF-8
      if (*sbeg == '"') {
         // a string begins here
         auto scur = sbeg + 1;
         while ((scur != s.end()) && (*scur != '"')) {
            if ((*scur == '\\') && ((scur+1) != s.end())) {
               // next character is quoted; skip it
               scur++;
            }
            scur++;
         }
         if (scur != s.end()) {
            // we found a complete quoted string
            std::string quoted_string(sbeg+1, scur);
            try {
               // throw if not a valid UTF-8 string:
               std::wstring ws=to_utf<wchar_t>(quoted_string,"UTF-8",stop);
            } catch (conversion_error) {
               return 0;  // this is not an interesting string
            }
         } else {
            // we arrived at the end of the string without finding a terminating double quote; reject
            return 0;
         }
         sbeg = scur;   // move forward
      }
   }

   // this input is valid. Let's try using it
   try {
      auto v = json::construct(s);
      // We had valid input and we constructed a value.  Let's see what we got:
      std::stringstream ss;
      try {
         ss << v;   // activate recursive visitors etc.
      } catch (std::out_of_range) {
         // most likely an invalid Unicode string etc. (thrown by UTF converter)
         return 0;
      }
      if (!s.empty()) {
         assert(ss.str().size() > 0);   // use the result to avoid getting optimized out
      }
   } catch (json::parse_error e) {
      // this is fine for fuzzing purposes
      return 0;
   }

   return 0;   // the only valid code, for now
}
