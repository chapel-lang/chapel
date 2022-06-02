/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
  : isDefaultConstructed_(true), kind_(ERROR), location_(), message_() {
}

ErrorMessage::ErrorMessage(ID id, Location location, std::string message,
                           Kind kind)
  : isDefaultConstructed_(false), kind_(kind), location_(location),
    message_(message),
    id_(id) {
  gdbShouldBreakHere();
}

ErrorMessage::ErrorMessage(ID id, Location location, const char* message,
                           Kind kind)
  : isDefaultConstructed_(false), kind_(kind), location_(location),
    message_(message),
    id_(id) {
  gdbShouldBreakHere();
}

ErrorMessage ErrorMessage::vbuild(ID id, Location loc, Kind kind,
                                  const char* fmt,
                                  va_list vl) {
  std::string ret;
  ret = vprint_to_string(fmt, vl);
  return ErrorMessage(id, loc, ret, kind);
}

ErrorMessage ErrorMessage::build(ID id, Location loc, Kind kind,
                                 const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(id, loc, kind, fmt, vl);
  va_end(vl);
  return ret;
}

void ErrorMessage::addDetail(ErrorMessage err) {
  details_.push_back(std::move(err));
}

void ErrorMessage::swap(ErrorMessage& other) {
  std::swap(isDefaultConstructed_, other.isDefaultConstructed_);
  std::swap(id_, other.id_);
  std::swap(kind_, other.kind_);
  location_.swap(other.location_);
  message_.swap(other.message_);
  details_.swap(other.details_);
}

void ErrorMessage::mark(Context* context) const {
  location_.mark(context);
  for (auto& em : details_) {
    em.mark(context);
  }
  id_.mark(context);
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
