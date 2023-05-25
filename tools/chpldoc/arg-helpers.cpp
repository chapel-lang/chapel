/*
 * Copyright 2022-2023 Hewlett Packard Enterprise Development LP
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

#include "arg-helpers.h"

#include "chpl/framework/Context.h"
#include "chpl/util/filesystem.h"

#include <iostream>
#include <cstring>

extern chpl::Context* gContext;

bool developer = false;


bool startsWith(const char* str, const char* prefix) {
  return (0 == strncmp(str, prefix, strlen(prefix)));
}

void clean_exit(int status) {
  if (gContext) {
    gContext->cleanupTmpDirIfNeeded();
  }
  exit(status);
}



uint64_t hexStr2uint64(const char* str, bool userSupplied,
                       const char* filename, int line) {
  if (!str) {
    std::cerr << "error: NULL string passed to hexStrToUint64()" << std::endl;
    clean_exit(1);
  }
  int len = strlen(str);
  if (len < 3 || str[0] != '0' || (str[1] != 'x' && str[1] != 'X')) {
    std::cerr << "error: Illegal string passed to hexStrToUint64()" << std::endl;
    clean_exit(1);
  }
  /* Remove leading 0s */
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }

  if (strlen(str+startPos) > 16) {
    std::cerr << "error: Integer literal overflow: '" + std::string(str) +
                  "' is too big for a 64-bit unsigned integer" << std::endl;
    clean_exit(1);
  }

  uint64_t val;
  int numitems = sscanf(str+2, "%" SCNx64, &val);
  if (numitems != 1) {
    std::cerr << "error: Illegal string passed to hexStrToUint64" << std::endl;
    clean_exit(1);
  }
  return val;
}




// Find the path to the running program
// (or return NULL if we couldn't figure it out).
std::string findProgramPath(const char* argv0, void* mainAddr)
{
  return chpl::getExecutablePath(argv0, mainAddr);
}



#define define_str2Int(type, format)                              \
  type##_t str2##type(const char* str,                            \
                      bool userSupplied,                          \
                      const char* filename,                       \
                      int line) {                                 \
    if (!str) {                                                   \
      std::cerr << "error: NULL string passed to strTo_" #type "()" << std::endl;       \
      clean_exit(1);                                              \
    }                                                             \
    int len = strlen(str);                                        \
    if (len < 1) {                                                \
      std::cerr << "error: empty string passed to strTo_" #type "()" << std::endl;      \
      clean_exit(1);                                              \
    }                                                             \
    type##_t val;                                                 \
    int numitems = sscanf(str, format, &val);                     \
    char checkStr[len+1];                                         \
    snprintf(checkStr, len+1, format, val);                       \
    if (numitems != 1) {                                          \
      std::cerr<<"error: Illegal string passed to strTo_" #type "()" << std::endl;    \
      clean_exit(1);                                              \
    }                                                             \
    /* Remove leading 0s */                                       \
    int startPos = 0;                                             \
    while (str[startPos] == '0' && startPos < len-1) {            \
      startPos++;                                                 \
    }                                                             \
    if (strcmp(str+startPos, checkStr) != 0) {                    \
                                  \
        std::cerr<< "error: Integer literal overflow: '" + std::string(str) + "' is too" \
                  " big for type '" #type "'" << std::endl;              \
        clean_exit(1);                                            \
          \
    }                                                             \
    return val;                                                   \
  }


define_str2Int(int64, "%" SCNd64)

define_str2Int(uint64, "%" SCNu64)

