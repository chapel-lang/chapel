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
#include "chpl/uast/AstNode.h"

#include <cassert>
#include <cstdarg>
#include <cstdlib>

namespace chpl {


static std::string vprint_to_string(const char* format, va_list vl) {
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
    assert(newbuf != nullptr);
    if (newbuf == nullptr) {
      free(buf);
      return "<internal error in saving error>";
    }
    buf = newbuf;
    // this call destructively consumes 'vl'
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
  : isDefaultConstructed_(true), kind_(ERROR), id_(), location_(), message_() {
}

ErrorMessage::ErrorMessage(Kind kind, Location location, std::string message)
  : isDefaultConstructed_(false), kind_(kind), id_(), location_(location),
    message_(message) {
  gdbShouldBreakHere();
}

ErrorMessage::ErrorMessage(Kind kind, Location location, const char* message)
  : isDefaultConstructed_(false), kind_(kind), id_(), location_(location),
    message_(message) {
  gdbShouldBreakHere();
}

ErrorMessage::ErrorMessage(Kind kind, ID id, std::string message)
  : isDefaultConstructed_(false), kind_(kind), id_(id), location_(),
    message_(message) {
  gdbShouldBreakHere();
}

ErrorMessage::ErrorMessage(Kind kind, ID id, const char* message)
  : isDefaultConstructed_(false), kind_(kind), id_(id), location_(),
    message_(message) {
  gdbShouldBreakHere();
}

ErrorMessage ErrorMessage::vbuild(Kind kind, ID id,
                                  const char* fmt,
                                  va_list vl) {
  std::string str;
  str = vprint_to_string(fmt, vl);
  return ErrorMessage(kind, id, str);
}

ErrorMessage ErrorMessage::vbuild(Kind kind, Location location,
                                  const char* fmt,
                                  va_list vl) {
  std::string str;
  str = vprint_to_string(fmt, vl);
  return ErrorMessage(kind, location, str);
}

ErrorMessage ErrorMessage::note(ID id, const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(NOTE, id, fmt, vl);
  va_end(vl);
  return ret;
}

ErrorMessage ErrorMessage::note(const uast::AstNode* ast,
                                const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(NOTE, ast->id(), fmt, vl);
  va_end(vl);
  return ret;
}

ErrorMessage ErrorMessage::warning(ID id, const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(WARNING, id, fmt, vl);
  va_end(vl);
  return ret;
}

ErrorMessage ErrorMessage::warning(const uast::AstNode* ast,
                                   const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(WARNING, ast->id(), fmt, vl);
  va_end(vl);
  return ret;
}

ErrorMessage ErrorMessage::error(ID id, const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(ERROR, id, fmt, vl);
  va_end(vl);
  return ret;
}

ErrorMessage ErrorMessage::error(const uast::AstNode* ast,
                                 const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(ERROR, ast->id(), fmt, vl);
  va_end(vl);
  return ret;
}

ErrorMessage ErrorMessage::error(Location loc,
                                 const char* fmt, ...) {
  ErrorMessage ret;
  va_list vl;
  va_start(vl, fmt);
  ret = ErrorMessage::vbuild(ERROR, loc, fmt, vl);
  va_end(vl);
  return ret;
}


void ErrorMessage::addDetail(ErrorMessage err) {
  details_.push_back(std::move(err));
}

Location ErrorMessage::location(Context* context) const {
  // if the ID is set, determine the location from that
  if (!id_.isEmpty()) {
    Location loc = parsing::locateId(context, id_);
    return loc;
  }

  // otherwise, use the location stored here
  return location_;
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


} // namespace chpl
