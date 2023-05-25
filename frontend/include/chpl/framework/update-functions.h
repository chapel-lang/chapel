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

/*
  This file implements the generic chpl::update
  as well as specializations for some common types.
 */

#ifndef CHPL_QUERIES_UPDATE_FUNCTIONS_H
#define CHPL_QUERIES_UPDATE_FUNCTIONS_H

#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "chpl/util/memory.h"

namespace chpl {
class Context;

template<typename T> struct update {
  bool operator()(T& keep, T& addin) const {
    // run update static method
    return T::update(keep, addin);
  }
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

template<typename K, typename V>
static inline bool
defaultUpdateMap(std::map<K, V>& keep, std::map<K, V>& addin) {
  return defaultUpdate(keep, addin);
}

template<typename T>
static inline bool
defaultUpdateVec(std::vector<T>& keep, std::vector<T>& addin) {
  if (keep.size() == addin.size()) {
    bool anyUpdated = false;
    // try updating the elements individually
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

template<typename T>
static inline bool defaultUpdateOwned(owned<T>& keep, owned<T>& addin) {
  // are they both null?
  if (keep.get() == nullptr && addin.get() == nullptr)
    return false;

  // is one null but not the other?
  if (keep.get() == nullptr || addin.get() == nullptr) {
    keep.swap(addin);
    return true;
  }

  // at this point they both are not null, so
  // call == on the values.
  bool match = (*keep.get() == *addin.get());
  if (match) {
    return false;
  }

  // call swap on the owned pointers (not the values)
  keep.swap(addin);
  return true;
}

template<typename A, typename B>
static inline bool defaultUpdatePair(std::pair<A,B>& keep,
                                     std::pair<A,B>& addin)
{
  chpl::update<A> aCombiner;
  chpl::update<B> bCombiner;

  bool anyUpdated = false;
  anyUpdated |= aCombiner(keep.first, addin.first);
  anyUpdated |= bCombiner(keep.second, addin.second);
  return anyUpdated;
}

/// \cond DO_NOT_DOCUMENT
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

template<typename T> struct update<owned<T>> {
  bool operator()(owned<T>& keep, owned<T>& addin) const {
    // call the static update method will usually use updateEqSwap
    return T::update(keep, addin);
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

template<typename K, typename V> struct update<std::map<K, V>> {
  bool operator()(std::map<K, V>& keep, std::map<K, V>& addin) const {
    return defaultUpdateMap(keep, addin);
  }
};

template<typename K, typename V> struct update<std::unordered_map<K,V>> {
  bool operator()(std::unordered_map<K,V>& keep,
                  std::unordered_map<K,V>& addin) const {
    return defaultUpdate(keep, addin);
  }
};

template<typename A, typename B> struct update<std::pair<A,B>> {
  bool operator()(std::pair<A,B>& keep,
                  std::pair<A,B>& addin) const {
    return defaultUpdatePair(keep, addin);
  }
};
/// \endcond


} // end namespace chpl

#endif
