/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
#include "chpl/util/printf.h"
#include "chpl/util/assertions.h"

namespace chpl {

std::string vprintToString(const char* format, ...) {
  va_list vl;
  va_start(vl, format);
  std::string result = vprintToString(format, vl);
  va_end(vl);
  return result;
}

std::string vprintToString(const char* format, va_list vl) {
  // using an argument list after va_end is undefined
  // so in other words, can't use 'vl' more than once,
  // so make a copy for the 1st call.
  va_list vlCopy;
  va_copy(vlCopy, vl);

  int size = 128;
  char* buf = (char*) malloc(size);
  // this call destructively consumes 'vlCopy'
  int got = vsnprintf(buf, size, format, vlCopy);
  // each va_copy should be matched to a va_end
  va_end(vlCopy);

  // 'got' is number of characters to write not including terminating null byte
  if (got >= size) {
    // output was truncated, so try again
    size = got+1; // include '\0' terminator
    char* newbuf = (char*) realloc(buf, size);
    CHPL_ASSERT(newbuf != nullptr);
    if (newbuf == nullptr) {
      free(buf);
      return "<internal error in saving error>";
    }
    buf = newbuf;
    // this call destructively consumes 'vl'
    got = vsnprintf(buf, size, format, vl);
    CHPL_ASSERT(got < size);
    if (got >= size) {
      free(buf);
      return "<internal error in saving error>";
    }
  }
  std::string ret(buf);
  free(buf);
  return ret;
}

} // end namespace chpl
