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

#ifndef CHPL_QUERIES_ERRORMESSAGE_H
#define CHPL_QUERIES_ERRORMESSAGE_H

#include "chpl/queries/Location.h"
#include "chpl/queries/ID.h"

#include <cstdarg>
#include <string>
#include <utility>
#include <vector>

namespace chpl {


/**
  This class represents an error/warning message. The message
  is saved (in the event it needs to be reported again).
 */
class ErrorMessage final {
 public:
  enum Kind {
    NOTE,
    WARNING,
    SYNTAX,
    ERROR
  };

 private:
  bool isDefaultConstructed_;
  Kind kind_;
  Location location_;
  std::string message_;
  ID id_;

  // sometimes an error message wants to point to a bunch of
  // related line numbers. That can go here.
  std::vector<ErrorMessage> details_;

  // TODO: how to handle a callstack of sorts?

 public:
  ErrorMessage();
  ErrorMessage(ID id, Location location, std::string message, Kind kind);
  ErrorMessage(ID id, Location location, const char* message, Kind kind);

  static ErrorMessage vbuild(ID id, Location loc, Kind kind,
                             const char* fmt,
                             va_list vl);

  static ErrorMessage build(ID id, Location loc, Kind kind,
                            const char* fmt, ...)
#ifndef DOXYGEN
    // docs generator has trouble with the attribute applied to 'build'
    // so the above ifndef works around the issue.
    __attribute__ ((format (printf, 4, 5)))
#endif
  ;

  void addDetail(ErrorMessage err);

  /**
    Returns true is this error message has no message and no details. Even
    if the error is empty, it may still be meaningful in the case of e.g.,
    a syntax error (where the location offers useful info).
  */
  bool isEmpty() const { return message_.empty() && details_.empty(); }

  /**
    Returns true if this error message was default constructed, in
    which case its contents are not meaningful.
  */
  bool isDefaultConstructed() const { return isDefaultConstructed_; }

  /**
    Return the location in the source code where this error occurred.
  */
  Location location() const { return location_; }

  UniqueString path() const { return location_.path(); }
  int firstLine() const { return location_.firstLine(); }
  int firstColumn() const { return location_.firstColumn(); }
  int lastLine() const { return location_.lastLine(); }
  int lastColumn() const { return location_.lastColumn(); }
  int line() const { return location_.line(); }

  const std::string& message() const { return message_; }

  const std::vector<ErrorMessage>& details() const { return details_; }

  Kind kind() const { return kind_; }

  inline bool operator==(const ErrorMessage& other) const {
    return isDefaultConstructed_ == other.isDefaultConstructed_ &&
           kind_ == other.kind_ &&
           location_ == other.location_ &&
           message_ == other.message_ &&
           details_ == other.details_ &&
           id_ == other.id_;
  }
  inline bool operator!=(const ErrorMessage& other) const {
    return !(*this == other);
  }

  void swap(ErrorMessage& other);

  void mark(Context* context) const;
  void updateLocation(Context *context);
};


} // end namespace chpl

#endif
