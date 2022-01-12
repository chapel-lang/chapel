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
  This file implements the generic chpl::mark.
 */

#ifndef CHPL_QUERIES_MARK_FUNCTIONS_H
#define CHPL_QUERIES_MARK_FUNCTIONS_H

#include <cassert>
#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "chpl/queries/Context.h"
#include "chpl/util/memory.h"

namespace chpl {


template<typename T> struct mark {
  void operator()(Context* context, const T& keep) const {
    // run the mark method
    return keep.mark(context);
  }
};

/// \cond DO_NOT_DOCUMENT
template<typename T> struct mark<T*> {
  void operator()(Context* context, const T* keep) const {
    context->markUnownedPointer(keep);
  }
};

template<typename T> struct mark<owned<T>> {
  void operator()(Context* context, owned<T>& keep) const {
    context->markOwnedPointer(keep.get());
  }
};

template<> struct mark<int> {
  void operator()(Context* context, const int& keep) const {
    // nothing to do
  }
};

template<> struct mark<bool> {
  void operator()(Context* context, const bool& keep) const {
    // nothing to do
  }
};

template<> struct mark<std::string> {
  void operator()(Context* context, const std::string& keep) const {
    // nothing to do
  }
};

template<typename T> struct mark<std::vector<T>> {
  void operator()(Context* context, const std::vector<T>& keep) const {
    for (auto const &elt : keep) {
      chpl::mark<T> marker;
      marker(context, elt);
    }
  }
};

template<typename K, typename V> struct mark<std::unordered_map<K,V>> {
  void operator()(Context* context, std::unordered_map<K,V>& keep) const {
    for (auto const &pair : keep) {
      chpl::mark<K> keyMarker;
      chpl::mark<V> valMarker;
      keyMarker(context, pair.first);
      valMarker(context, pair.second);
    }
  }
};

template<typename A, typename B> struct mark<std::pair<A,B>> {
  void operator()(Context* context, std::pair<A,B>& keep) const {
    chpl::mark<A> firstMarker;
    chpl::mark<B> secondMarker;
    firstMarker.mark(context, keep.first);
    secondMarker.mark(context, keep.second);
  }
};
/// \endcond


} // end namespace chpl

#endif
