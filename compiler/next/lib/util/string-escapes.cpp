/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chpl/util/string-escapes.h"

namespace chpl {

// Returns the hexadecimal character for 0-16.
static char toHex(char c) {
  return (0 <= c && c <= 9) ? '0' + c : 'A' + (c - 10);
}

static void addCharEscapeNonprint(std::string& s, char c) {
  int escape  = !(isascii(c) && isprint(c));
  //
  // If the previous character sequence was a hex escape and the current
  // character is a hex digit, escape it also.  Otherwise, conforming
  // target C compilers interpret this character as a continuation of
  // the previous hex escape.
  //
  if (isxdigit(c)) {
    size_t len = s.length();
    if (len >= 4 && s[len - 4] == '\\' &&
        (s[len - 3] == 'x' || s[len - 3] == 'X') &&
        isxdigit(s[len - 2]) && isxdigit(s[len - 1])) {
      escape = 1;
    }
  }

  if (escape) {
    s.push_back('\\');
    s.push_back('x');
    s.push_back(toHex(((unsigned char)c) >> 4));
    s.push_back(toHex(c & 0xf));
  } else {
    s.push_back(c);
  }
}

// Convert C escape characters into two characters: '\\' and the other character
static void addCharEscapingC(std::string& s, char c) {
  switch (c) {
    case '\"' :
      s.push_back('\\');
      s.push_back('"');
      break;
    case '?' :
      s.push_back('\\');
      s.push_back('?');
      break;
    case '\\' :
      s.push_back('\\');
      s.push_back('\\');
      break;
    case '\a' :
      s.push_back('\\');
      s.push_back('a');
      break;
    case '\b' :
      s.push_back('\\');
      s.push_back('b');
      break;
    case '\f' :
      s.push_back('\\');
      s.push_back('f');
      break;
    case '\n' :
      s.push_back('\\');
      s.push_back('n');
      break;
    case '\r' :
      s.push_back('\\');
      s.push_back('r');
      break;
    case '\t' :
      s.push_back('\\');
      s.push_back('t');
      break;
    case '\v' :
      s.push_back('\\');
      s.push_back('v');
      break;
    default :
      addCharEscapeNonprint(s, c);
      break;
  }
}

std::string quoteStringForC(const std::string& unescaped) {
  std::string s;
  for (char c : unescaped) {
    addCharEscapingC(s, c);
  }
  return s;
}


} // end namespace chpl
