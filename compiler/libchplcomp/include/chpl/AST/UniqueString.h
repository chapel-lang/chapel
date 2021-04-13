/*
  This file implements a global table of strings which enables easier string
  comparison and memory management.
 */

#ifndef CHPL_AST_UNIQUE_STRING_H
#define CHPL_AST_UNIQUE_STRING_H

#include <cassert>
#include <cstring>
#include <string>

namespace chpl {
namespace ast {

class Context;

namespace detail {
// We can make it store 6 bytes in line this way:
// alloc all such strings aligned to 2 bytes
// store in the low-order byte an odd number
// store in the last (memory order) byte, 6-length
//   (so we store 0 if the length is 6, which is the terminator)a
// fill any unused bytes with 0s
//
// This should work on both big and little endian systems.
// Q: what is the string distribution in Chapel programs?
//   compiling distributions primer with something like 1.24 we see:
//
//            ~1,075,000 table queries            vs English text (moby dick)
//   of these,  ~526,000 (49%) are < 7 bytes      (78%)
//              ~875,000 (81%) are < 13 bytes     (98%)
//            ~1,008,000 (94%) are < 23 bytes
//            ~1,034,000 (96%) are < 30 bytes
//            ~1,050,000 (98%) are < 39 bytes
//
//            ~45,000 unique strings
//   of these,  ~8,000 (18%) are < 7 bytes
//             ~12,000 (25%) are < 13 bytes
//             ~24,000 (53%) are < 23 bytes
//             ~28,000 (62%) are < 30 bytes
//             ~33,000 (73%) are < 39 bytes
//
// Q: does this improve performance?
//   Yes, by about 5-10% in testUniqueString.cpp
//   It might be more than that when the string references are
//   not recently used.

extern const char* emptyString;

struct InlinedString {

  // This union is only used to make the debugger output more reasonable.
  // All reads/writes here go through the 'v' variable.
  union {
    const char* v;
    char data[sizeof(const char*)];
  };

  enum {
    INLINE_TAG=0xbb, // could be any odd value
    MAX_SIZE_INLINED=sizeof(uintptr_t)-2
  };

  static inline bool alignmentIndicatesTag(const char* s) {
    uintptr_t val = (uintptr_t) s;
    // check if the low-order bits are the tag indicating inline
    return (val & 0xff) == INLINE_TAG;
  }

  static char* dataAssumingTag(void* vptr);

  /**
    Creates an InlinedString from a pointer.
    If strlen(s) <= MAX_SIZE_INLINED, it will store the data inline.
    Otherwise, it will point to s and this code assumes
    that alignmentIndicatesTag(s)==false.
   */
  static inline InlinedString buildFromAligned(const char* s, int len) {
    // Store empty strings as nullptr
    if (s == nullptr || len == 0) {
      InlinedString ret;
      ret.v = nullptr;
      return ret;
    }

    // Would the tag, null terminator, and data fit?
    if (len <= MAX_SIZE_INLINED) {
      uintptr_t val = INLINE_TAG; // store the tag in the low-order bits, 0s
      char* dst = dataAssumingTag(&val);
      // store the data (possibly after the tag), not including null byte
      // (since null byte will come from the zeros in val)
      memcpy(dst, s, len);
      // store the value we created into the struct and return it
      InlinedString ret;
      ret.v = (const char*) val;
      return ret;
    }

    assert(!alignmentIndicatesTag(s));
    InlinedString ret;
    ret.v = s;
    return ret;
  }

  static InlinedString buildUsingContextTable(Context* context,
                                              const char* s, int len);

  static InlinedString build(Context* context, const char* s) {
    int len = 0;
    if (s != NULL) len = strlen(s);

    if (len <= MAX_SIZE_INLINED) {
      // if it fits inline, just return that
      return InlinedString::buildFromAligned(s, len);
    } else {
      // otherwise, use the unique strings table
      // which produces a string with even alignment
      return InlinedString::buildUsingContextTable(context, s, len);
    }
  }

  bool isInline() const {
    return alignmentIndicatesTag(this->v);
  }
  const char* c_str() const {
    if (this->v == nullptr) {
      return emptyString;
    }
    if (this->isInline()) {
      return dataAssumingTag((void*) &this->v);
    }

    // otherwise, s is a real pointer
    return this->v;
  }
};

// this class is POD and has only the trivial constructor to help the parser
// (which uses it in a union).
struct PODUniqueString {
  InlinedString i;
  static inline PODUniqueString build(Context* context, const char* s) {
    PODUniqueString ret;
    ret.i = InlinedString::build(context, s);
    return ret;
  }
  const char* c_str() const {
    return this->i.c_str();
  }
};

} // end namespace detail

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
    If NULL is provided, this function will return uniqueString("").
   */
  static inline UniqueString build(Context* context, const char* s) {
    detail::PODUniqueString ret = detail::PODUniqueString::build(context, s);
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
