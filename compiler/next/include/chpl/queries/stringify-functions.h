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
  This file implements the generic chpl::stringify
  as well as specializations for some common types.
 */

#ifndef CHPL_QUERIES_STRINGIFY_FUNCTIONS_H
#define CHPL_QUERIES_STRINGIFY_FUNCTIONS_H

#include <cassert>
#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "chpl/util/memory.h"

namespace chpl {
class Context;

enum StringifyKind {
  DEBUG_SUMMARY,
  DEBUG_DETAIL,
  CHPL_SYNTAX
};

template<typename T> struct stringify {
  std::string operator()(StringifyKind stringKind) const = 0;
};

// template<typename T>
// static inline std::string defaultStringify(StringifyKind stringKind) {
//   std::hash<T> hs;

//   // if (eq(keep, addin)) {
//   //   return false;
//   // } else {
//   //   keep.swap(addin);
//   //   return true;
//   // }
// }
// template<typename T>
// static inline std::string defaultStringifyBasic(StringifyKind stringKind) {
//   if (keep == addin) {
//     return false;
//   } else {
//     keep = addin;
//     return true;
//   }
// }
// template<typename T>
// static inline std::string defaultStringifyVec(std::vector<T>& keep, std::vector<T>& addin)
// {
//   if (keep.size() == addin.size()) {
//     bool anyUpdated = false;
//     // try updating the elements indivdually
//     size_t nElts = keep.size();
//     for (size_t i = 0; i < nElts; i++) {
//       chpl::update<T> combiner;
//       bool updated = combiner(keep[i], addin[i]);
//       anyUpdated |= updated;
//     }
//     return anyUpdated;
//   } else {
//     keep.swap(addin);
//     return true; // updated
//   }
// }
// template<typename T>
// static inline std::string defaultStringifyOwned(owned<T>& keep, owned<T>& addin) {
//   // are they both null?
//   if (keep.get() == nullptr && addin.get() == nullptr)
//     return false;

//   // is one null but not the other?
//   if (keep.get() == nullptr || addin.get() == nullptr) {
//     keep.swap(addin);
//     return true;
//   }

//   // at this point they both are not null, so check if their values match
//   // by calling == on the values.
//   bool match = (*keep.get() == *addin.get());
//   if (match) {
//     return false;
//   } else {
//     // call swap on the owned pointers (not the values)
//     keep.swap(addin);
//     return true;
//   }
// }

// template<typename A, typename B>
// static inline std::string defaultStringifyPair(std::pair<A,B>& keep,
//                                      std::pair<A,B>& addin)
// {
//   chpl::update<A> aCombiner;
//   chpl::update<B> bCombiner;

//   bool anyUpdated = false;
//   anyUpdated |= aCombiner(keep.first, addin.first);
//   anyUpdated |= bCombiner(keep.second, addin.second);
//   return anyUpdated;
// }

// /// \cond DO_NOT_DOCUMENT
// template<> struct stringify<std::string> {
//   std::string operator()() const {
//     return this;
//   }
// };

// template<typename T> struct stringify<T*> {
//   std::string operator()(T*& keep, T*& addin) const {
//     return defaultStringifyBasic(keep, addin);
//   }
// };

// template<typename T> struct stringify<owned<T>> {
//   std::string operator()(owned<T>& keep, owned<T>& addin) const {
//     return defaultStringifyOwned(keep, addin);
//   }
// };

// template<> struct stringify<int> {
//   std::string operator()(int& keep, int& addin) const {
//     return defaultStringifyBasic(keep, addin);
//   }
// };

// template<> struct stringify<bool> {
//   std::string operator()(StringifyKind stringKind) const {
//     return defaultStringifyBasic(keep, addin);
//   }
// };

// template<typename T> struct stringify<std::vector<T>> {
//   std::string operator()(std::vector<T>& keep, StringifyKind stringKind) const {
//     return defaultStringifyVec(keep, addin);
//   }
// };

// template<typename K, typename V> struct stringify<std::unordered_map<K,V>> {
//   std::string operator()(std::unordered_map<K,V>& keep,
//                   std::unordered_map<K,V>& addin) const {
//     return defaultStringify(keep, addin);
//   }
// };

// template<typename A, typename B> struct stringify<std::pair<A,B>> {
//   std::string operator()(std::pair<A,B>& keep,
//                   std::pair<A,B>& addin) const {
//     return defaultStringifyPair(stringKind);
//   }
// };
/// \


} // end namespace chpl

#endif
