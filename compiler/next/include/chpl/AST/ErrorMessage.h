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
