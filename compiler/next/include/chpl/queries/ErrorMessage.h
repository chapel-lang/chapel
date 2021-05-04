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

#ifndef CHPL_QUERIES_ERRORMESSAGE_H
#define CHPL_QUERIES_ERRORMESSAGE_H

#include "chpl/queries/Location.h"

#include <string>
#include <vector>

namespace chpl {


/**
  This class represents an error/warning message. The message
  is saved (in the event it needs to be reported again).
 */
class ErrorMessage final {
 private:
  int level_; // error? warning? performance hint?
  Location location_;
  std::string message_;

  // sometimes an error message wants to point to a bunch of
  // related line numbers. That can go here.
  std::vector<ErrorMessage> details_;

  // TODO: how to handle a callstack of sorts?

 public:
  ErrorMessage();
  ErrorMessage(Location location, std::string message);
  ErrorMessage(Location location, const char* message);

  static ErrorMessage build(Location loc, const char* fmt, ...)
#ifndef DOXYGEN
    // docs generator has trouble with the attribute applied to 'build'
    // so the above ifndef works around the issue.
    __attribute__ ((format (printf, 2, 3)))
#endif
  ;

  void addDetail(ErrorMessage err);

  Location location() const { return location_; }

  UniqueString path() const { return location_.path(); }
  int firstLine() const { return location_.firstLine(); }
  int firstColumn() const { return location_.firstColumn(); }
  int lastLine() const { return location_.lastLine(); }
  int lastColumn() const { return location_.lastColumn(); }
  int line() const { return location_.line(); }

  const std::string& message() const { return message_; }

  const std::vector<ErrorMessage>& details() const { return details_; }

  inline bool operator==(const ErrorMessage& other) const {
    return this->level_ == other.level_ &&
           this->location_ == other.location_ &&
           this->message_ == other.message_;
  }
  inline bool operator!=(const ErrorMessage& other) const {
    return !(*this == other);
  }

  void swap(ErrorMessage& other);

  void markUniqueStrings(Context* context) const;
};


} // end namespace chpl

#endif
