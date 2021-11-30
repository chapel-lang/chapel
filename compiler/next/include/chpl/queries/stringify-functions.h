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
#include <set>

namespace chpl {
class Context;

enum StringifyKind {
  DEBUG_SUMMARY,
  DEBUG_DETAIL,
  CHPL_SYNTAX
};


template<typename T> struct stringify {
  std::string operator()(StringifyKind stringKind, const T& stringMe) const = 0;
};


template<typename T>
static inline std::string defaultStringify(StringifyKind stringKind, const T& stringMe) {

  return std::string("HAVE NOT IMPLEMENTED STRINGIFY YET");

}

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

 template<typename A, typename B>
 static inline std::string defaultStringifyPair(StringifyKind stringKind, const std::pair<A,B>& stringPair)
 {
   stringify<A> stringA;
   stringify<B> stringB;

   return std::string("(" + stringA(stringKind, stringPair[0]) + ", " + stringB(stringKind, stringPair[1]));
 }

 /// \cond DO_NOT_DOCUMENT
 template<> struct stringify<std::string> {
   std::string operator()(StringifyKind stringKind, const std::string val) const {
     return val;
   }
 };


// template<typename T> struct stringify<T*> {
//   std::string operator()(StringifyKind stringKind, const T*& stringMe) const {
//     return defaultStringify(stringKind, stringMe);
//   }
// };


template<typename T> struct stringify<owned<T>> {
 std::string operator()(StringifyKind stringKind, const owned<T>& stringMe) const {
   return defaultStringify(stringKind, stringMe);
 }
};

 template<> struct stringify<int> {
   std::string operator()(StringifyKind stringKind, const int val) const {
     return std::to_string(val);
   }
 };

template<> struct stringify<long int> {
  std::string operator()(StringifyKind stringKind, const long int val) const {
    return std::to_string(val);
  }
};

template<> struct stringify<double> {
  std::string operator()(StringifyKind stringKind, const double val) const {
    return std::to_string(val);
  }
};

template<> struct stringify<long unsigned int> {
  std::string operator()(StringifyKind stringKind, const long unsigned int val) const {
    return std::to_string(val);
  }
};

 template<> struct stringify<bool> {
   std::string operator()(StringifyKind stringKind, const bool val) const {
     return defaultStringify(stringKind, val);
   }
 };

 template<typename T> struct stringify<std::vector<T>> {
   std::string operator()(StringifyKind stringKind, const std::vector<T>& stringMe) const {
     return defaultStringify(stringKind, stringMe);
   }
 };

 template<typename K, typename V> struct stringify<std::unordered_map<K,V>> {
   std::string operator()(StringifyKind stringKind, const std::unordered_map<K,V>& stringMe) const {
     return defaultStringify(stringKind, stringMe);
   }
 };

 template<typename A, typename B> struct stringify<std::pair<A,B>> {
   std::string operator()(StringifyKind stringKind,
                   const std::pair<A,B>& stringMe) const {

     return defaultStringifyPair(stringKind, stringMe);
   }
 };

template<typename A, typename B> struct stringify<std::set<std::pair<A,B>>> {
  std::string operator()(StringifyKind stringKind,
                         const std::set<std::pair<A,B>>& stringMe) const {

    return std::string("set<> stringify not yet implemented");
  }
};
/// \


} // end namespace chpl

#endif
