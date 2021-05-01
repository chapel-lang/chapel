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

/*
  This file implements a global table of strings which enables easier string
  comparison and memory management.
 */

#ifndef CHPL_AST_UNIQUE_STRING_H
#define CHPL_AST_UNIQUE_STRING_H

#include "chpl/AST/UniqueStringDetail.h"

#include <cassert>
#include <cstring>
#include <string>


namespace chpl {
class Context;

namespace ast {

/**
  This class represents a unique'd NULL-terminated string.
  Unique'd strings allow:
    * fast == and !=
    * not worrying about freeing them

 */
class UniqueString final {
 friend class Context;

 private:
  detail::PODUniqueString s;

 public:
  /** create a UniqueString storing the empty string */
  UniqueString() {
    this->s.i = detail::InlinedString::buildFromAligned("", 0);
  }
  /** create a UniqueString from a PODUniqueString.
      this constructor intentionally allows implicit conversion.
   */
  UniqueString(detail::PODUniqueString s) {
    this->s = s;
  }

  /**
    Get or create a unique string for a NULL-terminated C string.
    If NULL is provided, this function will return the UniqueString
    representing "".
   */
  static inline UniqueString build(Context* context, const char* s) {
    detail::PODUniqueString ret = detail::PODUniqueString::build(context, s);
    return UniqueString(ret);
  }

  /**
    Get or create a unique string for a string from a pointer
    and a length. If the length is 0, this function will return
    the UniqueString representing "".
   */
  static inline UniqueString build(Context* context,
                                   const char* s, size_t len) {
    detail::PODUniqueString ret =
      detail::PODUniqueString::build(context, s, len);
    return UniqueString(ret);
  }


  /**
    Get or create a unique string for a C++ string
    \rst
    .. note::

      will not handle strings with embedded ``'\0'`` bytes
    \endrst
   */
  static inline UniqueString build(Context* context, const std::string& s) {
    return UniqueString::build(context, s.c_str());
  }

  /** return the null-terminated string */
  const char* c_str() const {
    return s.i.c_str();
  }

  bool isEmpty() const {
    return s.i.c_str()[0] == '\0';
  }

  bool startsWith(const char* prefix) const {
    return (0 == strncmp(this->c_str(), prefix, strlen(prefix)));
  }
  bool startsWith(const UniqueString prefix) const {
    return this->startsWith(prefix.c_str());
  }
  bool startsWith(const std::string& prefix) const {
    return this->startsWith(prefix.c_str());
  }

  inline bool operator==(const UniqueString other) const {
    return this->s.i.v == other.s.i.v;
  }
  inline bool operator!=(const UniqueString other) const {
    return this->s.i.v != other.s.i.v;
  }
  int compare(const UniqueString other) const {
    if (this->s.i.v == other.s.i.v)
      return 0;
    else
      return strcmp(this->c_str(), other.c_str());
  }
  int compare(const char* other) const {
    return strcmp(this->c_str(), other);
  }
  size_t hash() const {
    std::hash<size_t> hasher;
    return hasher((size_t) s.i.v);
  }
  void swap(UniqueString& other) {
    UniqueString oldThis = *this;
    *this = other;
    other = oldThis;
  }
};


} // end namespace ast

template<> struct update<chpl::ast::UniqueString> {
  bool operator()(chpl::ast::UniqueString& keep,
                  chpl::ast::UniqueString& addin) const {
    return defaultUpdate(keep, addin);
  }
};

// Allow chpl::ast::UniqueString to be just called chpl::UniqueString
// TODO: Should it be moved out of the ast namespace? What directory
// should it go in?
using chpl::ast::UniqueString;


} // end namespace chpl

namespace std {
  template<> struct less<chpl::ast::UniqueString> {
    bool operator()(const chpl::ast::UniqueString lhs,
                    const chpl::ast::UniqueString rhs) const {
      return lhs.compare(rhs) < 0;
    }
  };
  template<> struct hash<chpl::ast::UniqueString> {
    size_t operator()(const chpl::ast::UniqueString key) const {
      return (size_t) key.hash();
    }
  };
  template<> struct equal_to<chpl::ast::UniqueString> {
    bool operator()(const chpl::ast::UniqueString lhs,
                    const chpl::ast::UniqueString rhs) const {
      return lhs == rhs;
    }
  };
} // end namespace std

#endif
