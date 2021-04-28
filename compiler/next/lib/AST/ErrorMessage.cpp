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

#include "chpl/AST/ErrorMessage.h"

#include <cassert>
#include <cstdarg>
#include <cstdlib>

namespace chpl {
namespace uast {


static std::string vprint_to_string(const char* format, va_list vl) {
  int size = 128;
  char* buf = (char*) malloc(size);
  int got = vsnprintf(buf, size, format, vl);
  if (got >= size) {
    // output was truncated, so try again
    size = got+1;
    buf = (char*) realloc(buf, size);
    got = vsnprintf(buf, size, format, vl);
    assert(got < size);
    if (got >= size) return "<internal error in saving error>";
  }
  std::string ret(buf);
  free(buf);
  return ret;
}

ErrorMessage::ErrorMessage()
  : level_(-1), location_(), message_() {
}
ErrorMessage::ErrorMessage(Location location, std::string message)
  : level_(0), location_(location), message_(message) {
}
ErrorMessage::ErrorMessage(Location location, const char* message)
  : level_(0), location_(location), message_(message) {
}

ErrorMessage ErrorMessage::build(Location loc, const char* fmt, ...) {
  std::string ret;
  va_list vl;
  va_start(vl, fmt);
  ret = vprint_to_string(fmt, vl);
  va_end(vl);
  return ErrorMessage(loc, ret);
}

void ErrorMessage::addDetail(ErrorMessage err) {
  details.push_back(std::move(err));
}

void ErrorMessage::swap(ErrorMessage& other) {
  int oldThisLevel = this->level_;
  this->level_ = other.level_;
  other.level_ = oldThisLevel;
  this->location_.swap(other.location_);
  this->message_.swap(other.message_);
}


} // namespace uast
} // namespace chpl
