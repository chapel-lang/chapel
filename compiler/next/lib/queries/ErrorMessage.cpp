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

#include "chpl/queries/ErrorMessage.h"
#include "chpl/parsing/parsing-queries.h"

#include <cassert>
#include <cstdarg>
#include <cstdlib>

namespace chpl {


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
    if (got >= size) {
      free(buf);
      return "<internal error in saving error>";
    }
  }
  std::string ret(buf);
  free(buf);
  return ret;
}

ErrorMessage::ErrorMessage()
  : level_(-1), location_(), message_() {
}
ErrorMessage::ErrorMessage(ID id, Location location, std::string message)
  : level_(0), location_(location), message_(message), id_(id) {
}
ErrorMessage::ErrorMessage(ID id, Location location, const char* message)
  : level_(0), location_(location), message_(message), id_(id) {
}

ErrorMessage ErrorMessage::vbuild(ID id, Location loc, const char* fmt, va_list vl) {
  std::string ret;
  ret = vprint_to_string(fmt, vl);
  return ErrorMessage(id, loc, ret);
}

ErrorMessage ErrorMessage::build(ID id, Location loc, const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(id, loc, fmt, vl);
  va_end(vl);
  return ret;
}

void ErrorMessage::addDetail(ErrorMessage err) {
  details_.push_back(std::move(err));
}

void ErrorMessage::swap(ErrorMessage& other) {
  std::swap(level_, other.level_);
  location_.swap(other.location_);
  message_.swap(other.message_);
  details_.swap(other.details_);
}

void ErrorMessage::mark(Context* context) const {
  this->location_.mark(context);
}

void ErrorMessage::updateLocation(Context* context) {
  if (!id_.isEmpty()) {
    location_ = parsing::locateId(context, id_);
  }
  for (auto& err : details_) {
    err.updateLocation(context);
  }
}


} // namespace chpl
