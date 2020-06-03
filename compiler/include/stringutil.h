/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include <stdint.h>
#include <string>
#include <vector>

const char* astr(const char* s1,
                 const char* s2,
                 const char* s3 = 0,
                 const char* s4 = 0,
                 const char* s5 = 0,
                 const char* s6 = 0,
                 const char* s7 = 0,
                 const char* s8 = 0,
                 const char* s9 = 0);

const char* astr(const char* s1);
const char* astr(const std::string& s);

const char* istr(int i);

const char* asubstr(const char* s, const char* e);

void        deleteStrings();

int8_t      str2int8(const char* str, bool userSupplied = false,
                     const char* file = NULL, int line = -1);
int16_t     str2int16(const char* str, bool userSupplied = false,
                      const char* file = NULL, int line = -1);
int32_t     str2int32(const char* str, bool userSupplied = false,
                      const char* file = NULL, int line = -1);
int64_t     str2int64(const char* str, bool userSupplied = false,
                      const char* file = NULL, int line = -1);
uint8_t     str2uint8(const char* str, bool userSupplied = false,
                      const char* file = NULL, int line = -1);
uint16_t    str2uint16(const char* str, bool userSupplied = false,
                       const char* file = NULL, int line = -1);
uint32_t    str2uint32(const char* str, bool userSupplied = false,
                       const char* file = NULL, int line = -1);
uint64_t    str2uint64(const char* str, bool userSupplied = false,
                       const char* file = NULL, int line = -1);

uint64_t    binStr2uint64(const char* str, bool userSupplied = false,
                          const char* filename = NULL, int line = -1);
uint64_t    octStr2uint64(const char* str, bool userSupplied = false,
                          const char* filename = NULL, int line = -1);
uint64_t    hexStr2uint64(const char* str, bool userSupplied = false,
                          const char* filename = NULL, int line = -1);

// std::string utilities
       std::string erasePrefix(std::string s, int count);
       std::string firstNonEmptyLine(const std::string& s);
inline bool        isEmpty(const std::string& s);
inline std::string ltrim(std::string s);
       std::string ltrimAllLines(std::string s);
       int         minimumPrefix(const std::string& s);

void splitString(const std::string& s, std::vector<std::string>& vec,
                 const char* delimiters);
void splitStringWhitespace(const std::string& s, std::vector<std::string>& vec);

void removeTrailingNewlines(std::string& str);

bool startsWith(const char* str, const char* prefix);

// Unicode-specific utilities
static inline bool isInitialUTF8Byte(unsigned char c)
{
  return (c & 0xc0) != 0x80;
}

#endif
