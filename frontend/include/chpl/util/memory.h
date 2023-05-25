/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_MEMORY_H
#define CHPL_UTIL_MEMORY_H

#include <memory>
#include "llvm/Option/Option.h"

namespace chpl {

// Below, the underlying type of optional is selected based on the available
// LLVM version (which implies an available C++ version, since LLVM16 requires
// C++17). The two types (llvm::Optional and std::optional) don't have an
// exactly equal API: the former has getValue while the latter has get_value,
// and so on. To use either without knowing which implementation is in use, you
// can instead use unary operator* (dereference) and operator bool (boolean
// coercion). Thus, `o.getValue()` becomes `*o`, and `o.hasValue()` becomes
// `(bool) o`, or even just `o` in some contexts like if conditions.

template <typename T>
#if LLVM_VERSION_MAJOR >= 16
/**
 optional<T> is just a synonym for 'std::optional<T>'.

 It allows for easy migration in the event that we switch
 underlying optional types.
 */
using optional = std::optional<T>;

/**
  This is the "empty" value for the above optional<T> type.
 */
static const auto empty = std::nullopt;
#else
/**
 optional<T> is just a synonym for 'llvm::Optional<T>'.

 It allows for easy migration in the event that we switch
 underlying optional types.
 */
using optional = llvm::Optional<T>;

/**
  This is the "empty" value for the above optional<T> type.
 */
static const auto empty = llvm::None;
#endif

/**
 owned<T> is just a synonym for 'std::unique_ptr<T>'.
 It is shorter and uses the Chapel term for it.
 */
template<typename T>
using owned = std::unique_ptr<T>;

/**
 give a raw pointer to an owned<T> to manage it.
 */
template<typename T>
static inline owned<T> toOwned(T* takeFrom) {
  return owned<T>(takeFrom);
}

/**
 shared<T> is just a synonym for 'std::shared_ptr<T>'.
 It is shorter and uses the Chapel term for it.
 */
template<typename T>
using shared = std::shared_ptr<T>;

/**
 give a raw pointer to a shared<T> to manage it.
 */
template<typename T>
static inline shared<T> toShared(T* takeFrom) {
  return shared<T>(takeFrom);
}

// Determine endianness
// Although not technically allowed to be determinable at compile-time (e.g.,
// as a constexpr function), most compilers will optimize away this function as
// always returning true or false. Verified that both GCC and Clang optimize
// this away at compile-time.
//
// This could alternatively be implemented in terms of the macro test
//   #if __BYTE_ORDER == __LITTLE_ENDIAN
// but that is less portable.
inline bool little_endian()
{
  unsigned endian  =  1;
  static_assert(sizeof(endian) > 1,
                "endian must be more than one byte in size");
  return *(unsigned char*)(&endian) != 0;
}

} // end namespace chpl

#endif
