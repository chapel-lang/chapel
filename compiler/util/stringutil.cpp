/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "stringutil.h"

#include "baseAST.h"
#include "map.h"
#include "misc.h"
#include "symbol.h"

#include <algorithm>
#include <climits>
#include <functional>
#include <sstream>
#include <unordered_map>

#include <inttypes.h>

struct MyStrEqual {
  bool operator()(const char* lhs, const char* rhs) const {
    return strcmp(lhs, rhs) == 0;
  }
};

struct MyStrHash : StringHashFns {
  std::size_t operator()(const char* k) const {
    return hash(k);
  }
};

std::unordered_map<const char*, const char*, MyStrHash, MyStrEqual> chapelStringsTable;

static const char* canonicalize_allocated_string(char *s) {
  // add s to the table if it is not present
  // if s is already in the present, get the value from the table
  auto pair = chapelStringsTable.insert( {s, s} );
  const char* ret = pair.first->second; // return the value now in the table

  if (ret != s) {
    // free the argument if we found an existing string in the table
    free(s);
  }

  return ret;
}

const char*
astr(const char* s1, const char* s2, const char* s3, const char* s4,
     const char* s5, const char* s6, const char* s7, const char* s8,
     const char* s9) {
  int len, len1, len2, len3, len4, len5, len6, len7, len8, len9;
  len = len1 = len2 = len3 = len4 = len5 = len6 = len7 = len8 = len9 = 0;

  if (s1)
    len1 = strlen(s1);
  if (s2)
    len2 = strlen(s2);
  if (s3)
    len3 = strlen(s3);
  if (s4)
    len4 = strlen(s4);
  if (s5)
    len5 = strlen(s5);
  if (s6)
    len6 = strlen(s6);
  if (s7)
    len7 = strlen(s7);
  if (s8)
    len8 = strlen(s8);
  if (s9)
    len9 = strlen(s9);

  len = len1 + len2 + len3 + len4 + len5 + len6 + len7 + len8 + len9;

  // allocate the total memory
  char* s = (char*)malloc(len+1);

  len = 0;

  // copy each non-empty string
  if (len1 > 0) {
    memcpy(&s[len], s1, len1);
    len += len1;
  }
  if (len2 > 0) {
    memcpy(&s[len], s2, len2);
    len += len2;
  }
  if (len3 > 0) {
    memcpy(&s[len], s3, len3);
    len += len3;
  }
  if (len4 > 0) {
    memcpy(&s[len], s4, len4);
    len += len4;
  }
  if (len5 > 0) {
    memcpy(&s[len], s5, len5);
    len += len5;
  }
  if (len6 > 0) {
    memcpy(&s[len], s6, len6);
    len += len6;
  }
  if (len7 > 0) {
    memcpy(&s[len], s7, len7);
    len += len7;
  }
  if (len8 > 0) {
    memcpy(&s[len], s8, len8);
    len += len8;
  }
  if (len9 > 0) {
    memcpy(&s[len], s9, len9);
    len += len9;
  }

  // add the null terminator
  s[len] = '\0';

  return canonicalize_allocated_string(s);
}

const char* astr(const char* s1)
{
  auto search = chapelStringsTable.find(s1);
  if (search != chapelStringsTable.end()) {
    // return an existing entry
    return search->second;
  }

  // add a new entry - always a fresh malloc
  char* s = strdup(s1);
  chapelStringsTable.insert(search, {s, s});
  return s;
}

const char* astr(const std::string& s)
{
  return astr(s.c_str());
}

const char*
istr(int i) {
  char s[64];
  if (sprintf(s, "%d", i) > 63)
    INT_FATAL("istr buffer overflow");
  return astr(s);
}

//
// returns a canonicalized substring that contains the first part of
// 's' up to 'e'
// note: e must be a pointer that points within in s
//
const char* asubstr(const char* s, const char* e) {
  char* ss = (char*)malloc(e-s+1);
  strncpy(ss, s, e-s);
  ss[e-s] = '\0';
  return canonicalize_allocated_string(ss);
}


void deleteStrings() {
  for (auto pair: chapelStringsTable) {
    free(const_cast<char*>(pair.second));
  }
}


#define define_str2Int(type, format)                              \
  type##_t str2##type(const char* str,                            \
                      bool userSupplied,                          \
                      const char* filename,                       \
                      int line) {                                 \
    if (!str) {                                                   \
      INT_FATAL("NULL string passed to strTo_" #type "()");       \
    }                                                             \
    int len = strlen(str);                                        \
    if (len < 1) {                                                \
      INT_FATAL("empty string passed to strTo_" #type "()");      \
    }                                                             \
    type##_t val;                                                 \
    int numitems = sscanf(str, format, &val);                     \
    char checkStr[len+1];                                         \
    snprintf(checkStr, len+1, format, val);                       \
    if (numitems != 1) {                                          \
      INT_FATAL("Illegal string passed to strTo_" #type "()");    \
    }                                                             \
    /* Remove leading 0s */                                       \
    int startPos = 0;                                             \
    while (str[startPos] == '0' && startPos < len-1) {            \
      startPos++;                                                 \
    }                                                             \
    if (strcmp(str+startPos, checkStr) != 0) {                    \
      if (userSupplied) {                                         \
        astlocT astloc(line, filename);                           \
        USR_FATAL(astloc, "Integer literal overflow: %s is too"   \
                  " big for type " #type, str);                   \
      } else {                                                    \
        INT_FATAL("Integer literal overflow: %s is too "          \
                  "big for type " #type, str);                    \
      }                                                           \
    }                                                             \
    return val;                                                   \
  }

define_str2Int(int8, "%" SCNd8)
define_str2Int(int16, "%" SCNd16)
define_str2Int(int32, "%" SCNd32)
define_str2Int(int64, "%" SCNd64)
define_str2Int(uint8, "%" SCNu8)
define_str2Int(uint16, "%" SCNu16)
define_str2Int(uint32, "%" SCNu32)
define_str2Int(uint64, "%" SCNu64)


uint64_t binStr2uint64(const char* str, bool userSupplied,
                       const char* filename, int line) {
  if (!str) {
    INT_FATAL("NULL string passed to binStrToUint64()");
  }
  int len = strlen(str);

  if (len < 3 || str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) {
    INT_FATAL("Illegal string passed to binStrToUint64()");
  }
  /* Remove leading 0s */
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }
  if (strlen(str+startPos) > 64) {
    if (userSupplied) {
      astlocT astloc(line, filename);
      USR_FATAL(astloc, "Integer literal overflow: '%s' is too big "
                "for type uint64", str);
    } else {
      INT_FATAL("Integer literal overflow: '%s' is too big "
                "for type uint64", str);
    }
  }
  uint64_t val = 0;
  for (int i=startPos; i<len; i++) {
    val <<= 1;
    switch (str[i]) {
    case '0':
      break;
    case '1':
      val += 1;
      break;
    default:
      INT_FATAL("illegal character in binary string: '%c'", str[i]);
    }
  }
  return val;
}

uint64_t octStr2uint64(const char* str, bool userSupplied,
                       const char* filename, int line) {
  if (!str) {
    INT_FATAL("NULL string passed to octStrToUint64()");
  }
  int len = strlen(str);
  if (len < 3 || str[0] != '0' || (str[1] != 'o' && str[1] != 'O')) {
    INT_FATAL("Illegal string passed to octStrToUint64()");
  }

  /* Remove leading 0s */
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }

  if (len-startPos > 22 || (len-startPos == 22 && str[startPos] != '1')) {
    if (userSupplied) {
      astlocT astloc(line, filename);
      USR_FATAL(astloc, "Integer literal overflow: '%s' is too big "
                "for type uint64", str);
    } else {
      INT_FATAL("Integer literal overflow: '%s' is too big "
                "for type uint64", str);
    }
  }

  uint64_t val;
  int numitems = sscanf(str+2, "%" SCNo64, &val);
  if (numitems != 1) {
    INT_FATAL("Illegal string passed to octStrToUint64");
  }

  return val;
}

uint64_t hexStr2uint64(const char* str, bool userSupplied,
                       const char* filename, int line) {
  if (!str) {
    INT_FATAL("NULL string passed to hexStrToUint64()");
  }
  int len = strlen(str);
  if (len < 3 || str[0] != '0' || (str[1] != 'x' && str[1] != 'X')) {
    INT_FATAL("Illegal string passed to hexStrToUint64()");
  }
  /* Remove leading 0s */
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }

  if (strlen(str+startPos) > 16) {
    if (userSupplied) {
      astlocT astloc(line, filename);
      USR_FATAL(astloc, "Integer literal overflow: '%s' is too big "
                "for type uint64", str);
    } else {
      INT_FATAL("Integer literal overflow: '%s' is too big "
                "for type uint64", str);
    }
  }

  uint64_t val;
  int numitems = sscanf(str+2, "%" SCNx64, &val);
  if (numitems != 1) {
    INT_FATAL("Illegal string passed to hexStrToUint64");
  }
  return val;
}


inline int countLeadingSpaces(const std::string& s) {
  int leadingSpaces = 0;
  for (size_t i=0; i < s.length(); i++) {
    if (std::isspace(s[i]))
      leadingSpaces++;
    else
      break;
  }
  return leadingSpaces;
}

/*
 * Trim spaces from start of string.
 *
 * From: http://stackoverflow.com/a/217605
 */
inline std::string ltrim(std::string s) {
  s.erase(s.begin(), s.begin() + countLeadingSpaces(s));
  return s;
}


/*
 * Return true if 's' is empty or only has whitespace characters.
 */
inline bool isEmpty(const std::string& s) {
  return s.end() == s.begin()+countLeadingSpaces(s);
}


/*
 * Erase 'count' number of characters from beginning of each line in 's'. Just
 * ltrim() the first line, though.
 */
std::string erasePrefix(std::string s, int count) {
  std::stringstream sStream(s);
  std::string line;
  bool first = true;
  std::string result = std::string("");
  while (std::getline(sStream, line)) {
    if (first) {
      result += ltrim(line);
      result += std::string("\n");
      first = false;
      continue;
    }

    // Check that string has at least 'count' characters to erase. If there are
    // fewer than 'count', erase all characters in line.
    size_t endIndex;
    if (line.length() >= (size_t)count) {
      endIndex = count;
    } else {
      endIndex = line.length();
    }

    line.erase(line.begin(), line.begin() + endIndex);
    result += line;
    result += std::string("\n");
  }
  return result;
}


/*
 * Returns first non empty line of the string after ltrimming it. "Empty lines"
 * are those with no characters or only whitespace characters.
 */
std::string firstNonEmptyLine(const std::string& s) {
  std::stringstream sStream(s);
  std::string line;
  std::string result;
  while (std::getline(sStream, line)) {
    if (!isEmpty(line)) {
      result = ltrim(line);
      break;
    }
  }
  return result;
}


/*
 * Iterate through string, skipping the first line, finding the minimum amount
 * of whitespace before each line.
 *
 * FIXME: Find minimum prefix also if every single line begins with
 *        "\s+*\s". (thomasvandoren, 2015-02-04)
 */
int minimumPrefix(const std::string& s) {
  std::stringstream sStream(s);
  std::string line;
  bool first = true;
  int minPrefix = INT_MAX;
  while (std::getline(sStream, line)) {
    // Skip the first line. It is a special case that often has been trimmed to
    // some extent.
    if (first) {
      first = false;
      continue;
    }

    // If line only contains blanks, do not include it in this
    // computation. Especially in the case that the string is empty.
    if (isEmpty(line)) {
      continue;
    }

    // Find the first non-space character. Record if it is the new minimum.
    for (size_t i=0; (int)i < minPrefix && i < line.length(); i++) {
      if (!std::isspace(line[i])) {
        minPrefix = i;
        break;
      }
    }
  }
  return minPrefix;
}


/*
 * Find and remove same amount of whitespace from all lines of s.
 */
std::string ltrimAllLines(std::string s) {
  return erasePrefix(s, minimumPrefix(s));
}

/*
 * Split a string separated by the given delimiters into a vector of substrings.
 */
void splitString(const std::string& s, std::vector<std::string>& vec, const char* delimiters) {
  if (!s.empty()) {
    char* cStr = strdup(s.c_str());
    char* arg = strtok(cStr, delimiters);
    while (arg) {
      if (strlen(arg) > 0) {
        vec.push_back(std::string(arg));
      }
      arg = strtok(NULL, delimiters);
    }
    free(cStr);
  }
}

/*
 * Split a string by all whitespace characters into a vector of substrings.
 */
void splitStringWhitespace(const std::string& s, std::vector<std::string>& vec) {
  splitString(s, vec, " \t\n\r\f\v");
}

void removeTrailingNewlines(std::string& str) {
  while (str.size() > 0 && *str.rbegin() == '\n') {
    str.erase(str.end() - 1);
  }
}

bool startsWith(const char* str, const char* prefix) {
  return (0 == strncmp(str, prefix, strlen(prefix)));
}
