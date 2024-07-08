/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_FRAMEWORK_COMPILER_FLAGS_H
#define CHPL_FRAMEWORK_COMPILER_FLAGS_H

#include "chpl/framework/UniqueString.h"
#include "chpl/framework/update-functions.h"
#include "chpl/util/hash.h"

namespace chpl {


/**
  This class represents a collection of compiler flags. Not every flag
  present in an invocation of 'chpl --help' will be present here. This
  class is intended to be a convenient place to store various different
  boolean flags the compiler may need to check.
*/
class CompilerFlags final {
public:
  enum Name {
    UNKNOWN,
    WARN_UNSTABLE,
    WARN_UNSTABLE_INTERNAL,
    WARN_UNSTABLE_STANDARD,   // warn in all bundled modules except internal
    WARN_ARRAY_OF_RANGE,
    WARN_UNKNOWN_TOOL_SPACED_ATTRS,
    PERMIT_UNHANDLED_MODULE_ERRORS,
    WARN_INT_TO_UINT,
    NUM_FLAGS
  };

private:
  std::set<CompilerFlags::Name> active_;

public:

  CompilerFlags() = default;

  /**
    Set a compiler flag.
  */
  inline void set(CompilerFlags::Name flag, bool val) {
    if (val) active_.insert(flag); else active_.erase(flag);
  }

  /**
    Returns true if the given compiler flag is currently set.
  */
  inline bool get(CompilerFlags::Name flag) const {
    return active_.find(flag) != active_.end();
  }

  inline bool operator==(const CompilerFlags& other) const {
    return this->active_ == other.active_;
  }

  inline bool operator!=(const CompilerFlags& other) const {
    return !(*this == other);
  }

  void swap(CompilerFlags& other) {
    std::swap(active_, other.active_);
  }

  static bool update(CompilerFlags& keep, CompilerFlags& addin) {
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

namespace std {
  template<> struct hash<chpl::CompilerFlags::Name> {
    inline size_t operator()(const chpl::CompilerFlags::Name& k) const{
      return (size_t) k;
    }
  };
} // end namespace std


#endif
