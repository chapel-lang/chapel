// This file implements a global table of strings which enables easier string
// comparison and memory management.

#ifndef CHPL_AST_UNIQUE_STRING_H
#define CHPL_AST_UNIQUE_STRING_H

#include <string>

namespace chpl {

// This class represents a unique'd NULL-terminated string.
// unique'd strings allow:
//   fast == and !=
//   not worrying about freeing them
class UniqueString final {
 private:
  const char* s;

 public:
  UniqueString(const char* str);
  UniqueString(const std::string& str);

  // return the string
  const char* c_str() {
    return s;
  }

  inline bool operator==(const UniqueString other) const {
    return this->s == other.s;
  }
  inline bool operator!=(const UniqueString other) const {
    return this->s != other.s;
  }
};

}

#endif
