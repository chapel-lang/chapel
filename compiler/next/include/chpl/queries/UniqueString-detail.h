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

#ifndef CHPL_QUERIES_UNIQUE_STRING_DETAIL_H
#define CHPL_QUERIES_UNIQUE_STRING_DETAIL_H

#include <cassert>
#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

/// \cond DO_NOT_DOCUMENT
namespace chpl {
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

struct InlinedString {

  // This union is only used to make the debugger output more reasonable.
  // All reads/writes here go through the 'v' variable.
  // Performance: Should we "inline" strings up to 14 bytes
  // by using a 16 byte data region? To try that, some of the
  // surrounding code will need adjustment.
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
    return (val == 0) || ((val & 0xff) == INLINE_TAG);
  }

  static char* dataAssumingTag(void* vptr) {
    char* ptr = (char*) vptr;

    // assuming the tag is present, where is the string data?
    // on a little endian system, need to pass over the tag.
    // on big endian systems, the tag is after the null terminator,
    // so no action is necessary.

    if (little_endian())
      ptr += 1; // pass over the tag

    return ptr;
  }

  /**
    Creates an InlinedString from a pointer.
    If strlen(s) <= MAX_SIZE_INLINED, it will store the data inline.
    Otherwise, it will point to s and this code assumes
    that alignmentIndicatesTag(s)==false.
   */
  static inline InlinedString buildFromAligned(const char* s, size_t len) {
    // Store empty strings as nullptr
    if (s == nullptr || len == 0)
      return { nullptr };

    // Would the tag, null terminator, and data fit?
    if (len <= MAX_SIZE_INLINED) {
      uintptr_t val = INLINE_TAG; // store the tag in the low-order bits, 0s
      char* dst = dataAssumingTag(&val);
      // store the data (possibly after the tag), not including null byte
      // (since null byte will come from the zeros in val)
      memcpy(dst, s, len);
      // create a struct with the value we created and return it
      return { (const char*) val };
    }

    assert(!alignmentIndicatesTag(s));
    return { s };
  }

  static InlinedString buildUsingContextTable(Context* context,
                                              const char* s, size_t len);

  static InlinedString build(Context* context, const char* s, size_t len) {
    if (len <= MAX_SIZE_INLINED) {
      // if it fits inline, just return that
      return InlinedString::buildFromAligned(s, len);
    } else {
      // otherwise, use the unique strings table
      // which produces a string with even alignment
      return InlinedString::buildUsingContextTable(context, s, len);
    }
  }
  static InlinedString build(Context* context, const char* s) {
    size_t len = 0;
    if (s != NULL) len = strlen(s);
    return InlinedString::build(context, s, len);
  }
  static InlinedString build() {
    return InlinedString::buildFromAligned("", 0);
  }

  bool isInline() const {
    return alignmentIndicatesTag(this->v);
  }
  const char* c_str() const {
    if (this->isInline()) {
      return dataAssumingTag((void*) &this->v);
    }

    // otherwise, s is a real pointer
    return this->v;
  }
};

// This class is POD and has only the trivial constructor to help the parser
// (which uses it in a union).
// All UniqueStrings are actually POD; the difference is that this one
// does not have a user-defined constructor.
// TODO: rename it
struct PODUniqueString {
  InlinedString i;
  static PODUniqueString build(Context* context, const char* s, size_t len) {
    return { InlinedString::build(context, s, len) };
  }
  static PODUniqueString build(Context* context, const char* s) {
    return { InlinedString::build(context, s) };
  }
  static inline PODUniqueString build() {
    PODUniqueString ret = {InlinedString::build()};
    return ret;
  }
  const char* c_str() const {
    return i.c_str();
  }
  bool isEmpty() const {
    return i.c_str()[0] == '\0';
  }
};


} // end namespace detail

template<typename T> struct update {
  bool operator()(T& keep, T& addin) const = 0;
};

template<typename T>
static inline bool defaultUpdate(T& keep, T& addin) {
  std::equal_to<T> eq;
  if (eq(keep, addin)) {
    return false;
  } else {
    keep.swap(addin);
    return true;
  }
}
template<typename T>
static inline bool defaultUpdateBasic(T& keep, T& addin) {
  if (keep == addin) {
    return false;
  } else {
    keep = addin;
    return true;
  }
}
template<typename T>
static inline bool defaultUpdateVec(std::vector<T>& keep, std::vector<T>& addin)
{
  if (keep.size() == addin.size()) {
    bool anyUpdated = false;
    // try updating the elements indivdually
    size_t nElts = keep.size();
    for (size_t i = 0; i < nElts; i++) {
      chpl::update<T> combiner;
      bool updated = combiner(keep[i], addin[i]);
      anyUpdated |= updated;
    }
    return anyUpdated;
  } else {
    keep.swap(addin);
    return true; // updated
  }
}
template<> struct update<std::string> {
  bool operator()(std::string& keep, std::string& addin) const {
    return defaultUpdate(keep, addin);
  }
};
template<typename T> struct update<T*> {
  bool operator()(T*& keep, T*& addin) const {
    return defaultUpdateBasic(keep, addin);
  }
};
template<> struct update<int> {
  bool operator()(int& keep, int& addin) const {
    return defaultUpdateBasic(keep, addin);
  }
};
template<> struct update<bool> {
  bool operator()(bool& keep, bool& addin) const {
    return defaultUpdateBasic(keep, addin);
  }
};
template<typename T> struct update<std::vector<T>> {
  bool operator()(std::vector<T>& keep, std::vector<T>& addin) const {
    return defaultUpdateVec(keep, addin);
  }
};
template<typename K, typename V> struct update<std::unordered_map<K,V>> {
  bool operator()(std::unordered_map<K,V>& keep,
                  std::unordered_map<K,V>& addin) const {
    return defaultUpdate(keep, addin);
  }
};


template<typename T> struct mark {
  void operator()(Context* context, const T& keep) const { }
};


} // end namespace chpl
/// \endcond

#endif
