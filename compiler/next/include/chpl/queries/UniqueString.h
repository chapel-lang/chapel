/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_QUERIES_UNIQUE_STRING_H
#define CHPL_QUERIES_UNIQUE_STRING_H

#include "chpl/queries/UniqueString-detail.h"
#include "chpl/queries/stringify-functions.h"
#include "chpl/util/hash.h"

#include <cassert>
#include <cstring>
#include <string>


namespace chpl {
class Context;


/**
  This class represents a unique'd string.
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
    this->s.i = detail::InlinedString::build();
  }
  /** create a UniqueString from a PODUniqueString.
      this constructor intentionally allows implicit conversion.
   */
  UniqueString(detail::PODUniqueString s)
    : s(s) {
  }

  /**
    Get or create a unique string for a NULL-terminated C string.
    If NULL is provided, this function will return the UniqueString
    representing "".
   */
  static UniqueString build(Context* context, const char* s) {
    detail::PODUniqueString ret = detail::PODUniqueString::build(context, s);
    return UniqueString(ret);
  }

  /**
    Get or create a unique string by concatenating up to 9 input C strings.
    These input strings cannot contain null bytes other than the null
    terminator.
   */
  static UniqueString buildConcat(Context* context,
                                  const char* s1, const char* s2,
                                  const char* s3 = nullptr,
                                  const char* s4 = nullptr,
                                  const char* s5 = nullptr,
                                  const char* s6 = nullptr,
                                  const char* s7 = nullptr,
                                  const char* s8 = nullptr,
                                  const char* s9 = nullptr) {
    auto ret = detail::PODUniqueString::buildConcat(context,
                                                    s1, s2,
                                                    s3, s4,
                                                    s5, s6,
                                                    s7, s8, s9);
    return UniqueString(ret);
  }


  /**
    Get or create a unique string for a string from a pointer
    and a length. If the length is 0, this function will return
    the UniqueString representing "".
    The length can be passed to truncate a string.
    The string can contain zero bytes.
   */
  static UniqueString build(Context* context, const char* s, size_t len);

  /**
    Get or create a unique string for a C++ string.
   */
  static inline UniqueString build(Context* context, const std::string& s) {
    return UniqueString::build(context, s.c_str(), s.size());
  }

  /**
    Return the null-terminated string.
    The returned pointer may refer to invalid memory if the UniqueString
    goes out of scope.
   */
  const char* c_str() const {
    return s.i.c_str();
  }

  /**
    Return the length of the unique string.
   */
  size_t length() const {
    return s.i.length();
  }

  /**
    Return the null-terminated string as a pointer to an entry
    in Context's string table. This pointer is safe to use after
    this UniqueString goes out of scope.
   */
  const char* astr(Context* context) const {
    return s.i.astr(context);
  }

  /** return a std::string containing the string */
  // diffs from stringify, converts the uniquestring into a string
  // representation similar to std::ostream.str()
  std::string str() const {
    return std::string(c_str(), length());
  }


  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  bool isEmpty() const {
    return s.i.c_str()[0] == '\0';
  }

  detail::PODUniqueString podUniqueString() const {
    return s;
  }

  /**
    Checks to see if the string starts with another string.
    \rst
    .. note::

      will not handle prefix strings with embedded ``'\0'`` bytes
    \endrst
   */
  bool startsWith(const char* prefix) const {
    return (0 == strncmp(this->c_str(), prefix, strlen(prefix)));
  }
  /**
    Checks to see if the string starts with another string.
    \rst
    .. note::

      will not handle prefix strings with embedded ``'\0'`` bytes
    \endrst
   */
  bool startsWith(const UniqueString prefix) const {
    return this->startsWith(prefix.c_str());
  }
  /**
    Checks to see if the string starts with another string.
    \rst
    .. note::

      will not handle prefix strings with embedded ``'\0'`` bytes
    \endrst
   */
  bool startsWith(const std::string& prefix) const {
    return this->startsWith(prefix.c_str());
  }

  inline bool operator==(const UniqueString other) const {
    return this->s.i.v == other.s.i.v;
  }
  /**
    Checks to see if the string contents match a C string.
    \rst
    .. note::

      will only compare up to the first null byte.
    \endrst
   */
  inline bool operator==(const char* other) const {
    return 0 == this->compare(other);
  }
  inline bool operator!=(const UniqueString other) const {
    return !(*this == other);
  }
  inline bool operator!=(const char* other) const {
    return 0 != this->compare(other);
  }

  /**
   Returns:
     * -1 if this string is less than the passed string
     * 0 if they are the same
     * 1 if this string is greater

    \rst
    .. note::

      will only compare up to the first null byte.
    \endrst
   */
  int compare(const UniqueString other) const {
    return *this == other ? 0 : compare(other.c_str());
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

  static bool update(UniqueString& keep, UniqueString& addin);

  void mark(Context* context) const {
    s.i.mark(context);
  }
};

/// \cond DO_NOT_DOCUMENT

/// \endcond


} // end namespace chpl

namespace std {
  template<> struct less<chpl::UniqueString> {
    bool operator()(const chpl::UniqueString lhs,
                    const chpl::UniqueString rhs) const {
      return lhs.compare(rhs) < 0;
    }
  };
  template<> struct hash<chpl::UniqueString> {
    size_t operator()(const chpl::UniqueString key) const {
      return key.hash();
    }
  };
  template<> struct hash<chpl::detail::PODUniqueString> {
    size_t operator()(const chpl::detail::PODUniqueString key) const {
      return key.hash();
    }
  };

} // end namespace std

#endif
