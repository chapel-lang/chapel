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

class Context;

/**
  This class represents a unique'd NULL-terminated string.
  Unique'd strings allow:
    * fast == and !=
    * not worrying about freeing them

  Use ASTContext to create one.

 */
class UniqueString final {
 friend class Context;

 private:
  // We could consider applying a short-string optimization here
  // if we thought it was valuable to have strings < 7 bytes be
  // stored directly.
  const char* s;

  explicit UniqueString(const char* str) : s(str) { }

 public:
  /** create a UniqueString storing the empty string */
  UniqueString();

  /**
    Get or create a unique string for a NULL-terminated C string.
    If NULL is provided, this function will return uniqueString("").
   */
  static UniqueString build(Context* context, const char* s);
  /**
    Get or create a unique string for a C++ string
    \rst
    .. note::

      will not handle strings with embedded ``'\0'`` bytes
    \endrst
   */
  static UniqueString build(Context* context, const std::string& s);

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
