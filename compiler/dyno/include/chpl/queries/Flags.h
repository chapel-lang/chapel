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

#ifndef CHPL_QUERIES_FLAGS_H
#define CHPL_QUERIES_FLAGS_H

#include "chpl/queries/UniqueString.h"
#include "chpl/queries/update-functions.h"
#include "chpl/util/hash.h"

namespace chpl {


/**
  This class represents a collection of compiler flags. Not every flag
  present in an invocation of 'chpl --help' will be present here. This
  class is intended to be a convenient place to store various different
  boolean flags the compiler may need to check.
*/
class Flags final {
public:
  enum Name {
    UNKNOWN,
    WARN_UNSTABLE,
    NUM_FLAGS
  };

private:
  std::set<Flags::Name> active_;

public:

  Flags() = default;

  /**
    Set a compiler flag.
  */
  inline void set(Flags::Name flag, bool val) {
    if (val) active_.insert(flag); else active_.erase(flag);
  }

  /**
    Returns true if the given compiler flag is currently set.
  */
  inline bool get(Flags::Name flag) const {
    return active_.find(flag) != active_.end();
  }

  inline bool operator==(const Flags& other) const {
    return this->active_ == other.active_;
  }

  inline bool operator!=(const Flags& other) const {
    return !(*this == other);
  }

  void swap(Flags& other) {
    std::swap(active_, other.active_);
  }

  static bool update(Flags& keep, Flags& addin) {
    return defaultUpdate(keep, addin);
  }

  void mark(Context* context) const {}

  size_t hash() const {
    return chpl::hash(active_);
  }
};
/// \cond DO_NOT_DOCUMENT

/// \endcond


} // end namespace chpl

#endif
