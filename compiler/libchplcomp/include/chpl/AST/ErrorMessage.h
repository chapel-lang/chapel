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
    __attribute__ ((format (printf, 2, 3)));
};

} // end namespace ast
} // end namespace chpl

#endif
