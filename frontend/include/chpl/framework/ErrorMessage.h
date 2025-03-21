/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include <cstdarg>
#include <string>
#include <utility>
#include <vector>

#include "chpl/framework/ID.h"
#include "chpl/framework/Location.h"
#include "chpl/util/printf.h"

namespace chpl {

class IdOrLocation {
 protected:
  // if id_ is set, it is used instead of location_
  ID id_;
  // location_ should only be used if id_ is empty which happens for
  // parser errors
  Location location_;

 public:
  IdOrLocation() = default;
  // enable implicit conversion from ID
  IdOrLocation(ID id) : id_(std::move(id)) {}
  // enable implicit conversion from Location
  IdOrLocation(Location location) : location_(std::move(location)) {}

  /**
    Return the location in the source code where this error occurred.
  */
  Location computeLocation(Context* context) const;
  const ID& id() const { return id_; }
  const Location& location() const { return location_; }

  bool operator==(const IdOrLocation& other) const {
    return id_ == other.id_ &&
           location_ == other.location_;
  }

  void mark(Context* context) const {
    id_.mark(context);
    location_.mark(context);
  }

  static IdOrLocation createForCommandLineLocation(Context* context);
};

/**
  This class represents an error/warning message. The message
  is saved (in the event it needs to be reported again).
 */
class ErrorMessage final : public IdOrLocation {
 public:
  enum Kind {
    NOTE,
    WARNING,
    SYNTAX,
    ERROR
  };

 private:
  Kind kind_;

  std::string message_;

  // sometimes an error message wants to point to a bunch of
  // related line numbers. That can go here.
  std::vector<ErrorMessage> details_;

  // TODO: how to handle a callstack of sorts?

 public:
  ErrorMessage(Kind kind, IdOrLocation idOrLoc, std::string message);

  /** Add an ErrorMessage as detail information to this ErrorMessage. */
  void addDetail(ErrorMessage err);

  /**
    Returns true is this error message has no message and no details. Even
    if the error is empty, it may still be meaningful in the case of e.g.,
    a syntax error (where the location offers useful info).
  */
  bool isEmpty() const { return message_.empty() && details_.empty(); }

  const std::string& message() const { return message_; }

  const std::vector<ErrorMessage>& details() const { return details_; }

  Kind kind() const { return kind_; }

  inline bool operator==(const ErrorMessage& other) const {
    return IdOrLocation::operator==(other) &&
           kind_ == other.kind_ &&
           message_ == other.message_ &&
           details_ == other.details_;
  }
  inline bool operator!=(const ErrorMessage& other) const {
    return !(*this == other);
  }
};

} // end namespace chpl

#endif
