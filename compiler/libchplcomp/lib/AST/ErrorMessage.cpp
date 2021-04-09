#include "chpl/AST/ErrorMessage.h"

#include <cassert>
#include <cstdarg>
#include <cstdlib>

namespace chpl {
namespace ast {

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

ErrorMessage ErrorMessage::build(Location loc, const char* fmt, ...) {
  std::string ret;
  va_list vl;
  va_start(vl, fmt);
  ret = vprint_to_string(fmt, vl);
  va_end(vl);
  return ErrorMessage(loc, ret);
}

} // namespace ast
} // namespace chpl
