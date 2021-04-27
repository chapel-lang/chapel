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

#ifndef CHPL_AST_ERRORMESSAGE_H
#define CHPL_AST_ERRORMESSAGE_H

#include "chpl/AST/Location.h"

#include <string>

namespace chpl {
namespace ast {


/**
  This class represents an error/warning message. The message
  is saved (in the event it needs to be reported again).
 */
class ErrorMessage final {
 private:
  int level_; // error? warning? performance hint?
  Location location_;
  std::string message_;

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

  inline bool operator==(const ErrorMessage& other) const {
    return this->level_ == other.level_ &&
           this->location_ == other.location_ &&
           this->message_ == other.message_;
  }
  inline bool operator!=(const ErrorMessage& other) const {
    return !(*this == other);
  }

  void swap(ErrorMessage& other);
};

} // end namespace ast

// Allow chpl::ast::ErrorMessage to be just called chpl::ErrorMessage
// TODO: Should it be moved out of the ast namespace? What directory
// should it go in?
using chpl::ast::ErrorMessage;


} // end namespace chpl

#endif
