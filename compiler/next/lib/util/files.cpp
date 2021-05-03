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

#include "files.h"

#include "./mystrerror.h"

#include "chpl/uast/ErrorMessage.h"
#include "chpl/uast/Location.h"

#include <cerrno>

namespace chpl {

FILE* openfile(const char* path, const char* mode, ErrorMessage& errorOut) {
  FILE* fp = fopen(path, mode);
  if (fp == nullptr) {
    std::string strerr = my_strerror(errno);
    auto emptyLocation = Location();
    // set errorOut. NULL will be returned.
    errorOut = ErrorMessage::build(emptyLocation, "opening %s: %s",
                                   path, strerr.c_str());
  }

  return fp;
}

bool closefile(FILE* fp, const char* path, ErrorMessage& errorOut) {
  int rc = fclose(fp);
  if (rc != 0) {
    std::string strerr = my_strerror(errno);
    auto emptyLocation = Location();
    errorOut = ErrorMessage::build(emptyLocation, "closing %s: %s",
                                   path, strerr.c_str());
    return false;
  }
  return true;
}

bool readfile(const char* path,
              std::string& strOut,
              ErrorMessage& errorOut) {

  FILE* fp = openfile(path, "r", errorOut);
  if (fp == nullptr) {
    return false;
  }

  // Try to get the file length in order to optimize string allocation
  long len = 0;
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (len > 0) strOut.reserve(len);

  char buf[256];
  while (true) {
    size_t got = fread(buf, 1, sizeof(buf), fp);
    if (got > 0) {
      strOut.append(buf, got);
    } else {
      if (ferror(fp)) {
        auto emptyLocation = Location();
        errorOut = ErrorMessage::build(emptyLocation, "reading %s", path);
        ErrorMessage ignored;
        closefile(fp, path, ignored);
        return false;
      }
      // otherwise, end of file reached
      break;
    }
  }
  bool ok = closefile(fp, path, errorOut);
  if (!ok) {
    return false;
  }

  return true;
}

}
