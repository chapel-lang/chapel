/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_BITMAP_H
#define CHPL_UTIL_BITMAP_H

#include "chpl/queries/stringify-functions.h"

#include <cassert>
#include <cstddef>
#include <vector>

namespace chpl {


class Context;

/**
  Bitmap just stores a bunch of bits. It's more or less
  like vector<bool> but we have a different type to make
  using it less confusing.
 */
class Bitmap {
  std::vector<bool> bits_;
 public:
  /** Construct a Bitmap storing no bits */
  Bitmap() : bits_() { }
  /** Resizes this Bitmap to store n bits */
  void resize(size_t n) {
    bits_.resize(n);
  }
  /** Returns the number of bits stored in this Bitmap */
  size_t size() const {
    return bits_.size();
  }
  /** Returns the value of bit index i */
  bool operator[](size_t i) const {
    assert(0 <= i && i < bits_.size());
    return bits_[i];
  }
  /** Set the value of bit index i */
  void setBit(size_t i, bool value) {
    assert(0 <= i && i < bits_.size());
    bits_[i] = value;
  }

  bool operator==(const Bitmap& other) const {
    return bits_ == other.bits_;
  }
  bool operator!=(const Bitmap& other) const {
    return !(*this == other);
  }
  void swap(Bitmap& other) {
    bits_.swap(other.bits_);
  }
  static bool update(Bitmap& keep, Bitmap& addin) {
    if (keep == addin)
      return false;

    keep.swap(addin);
    return true;
  }
  void mark(Context* context) const {
    (void) bits_; // nothing to mark here
  }

  // defined in the .cpp file to avoid ambiguity in template instantiation
  size_t hash() const;

  void stringify(std::ostream& s, StringifyKind stringKind) const;
};


} // end namespace chpl


namespace std {


// These std::hash functions are here b/c the hash functions
// above can't have partial template specialization.
template<> struct hash<chpl::Bitmap> {
  size_t operator()(const chpl::Bitmap& bitmap) const {
    return bitmap.hash();
  }
};


} // end namespace std

#endif
