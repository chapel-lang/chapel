/*
  This file implements a global table of strings which enables easier string
  comparison and memory management.
 */

#ifndef CHPL_AST_UNIQUE_STRING_H
#define CHPL_AST_UNIQUE_STRING_H

#include <string>
#include <cstring>

namespace chpl {
namespace ast {

class ASTContext;

/**
  This class represents a unique'd NULL-terminated string.
  Unique'd strings allow:
    * fast == and !=
    * not worrying about freeing them

  Use ASTContext to create one.

 */
class UniqueString final {
 friend class ASTContext;

 private:
  const char* s;

  explicit UniqueString(const char* str) : s(str) { }

 public:
  /** return the null-terminated string */
  const char* c_str() const {
    return s;
  }

  bool isEmpty() const {
    return s[0] == '\0'; 
  }

  bool startsWith(const char* prefix) const;
  bool startsWith(const UniqueString prefix) const;
  bool startsWith(const std::string& prefix) const;

  inline bool operator==(const UniqueString other) const {
    return this->s == other.s;
  }
  inline bool operator!=(const UniqueString other) const {
    return this->s != other.s;
  }
  int compare(const UniqueString other) const {
    if (this->s == other.s)
      return 0;
    else
      return strcmp(this->c_str(), other.c_str());
  }
};

} // end namespace ast
} // end namespace chpl

namespace std {
  template<> struct less<chpl::ast::UniqueString> {
    bool operator()(const chpl::ast::UniqueString lhs,
                    const chpl::ast::UniqueString rhs) const {
      return lhs.compare(rhs) < 0;
    }
  };
} // end namespace std

#endif
