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
  This file implements the generic chpl::stringify
  as well as specializations for some common types.

  stringify can be used to get a string representation of an object
  for debugging purposes or whenever a string representation of an
  object is needed
 */

#ifndef CHPL_QUERIES_STRINGIFY_FUNCTIONS_H
#define CHPL_QUERIES_STRINGIFY_FUNCTIONS_H

#include "chpl/util/memory.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>


namespace chpl {
class Context;

/*
 * TODO: Implement these definitions
 * chpl::StringifyKind describes different ways to format the stringification
 * of an object.
 * DEBUG_SUMMARY - summary debug output, hash of query,
                    type information, id/name, ast tag
 * DEBUG_DETAIL - summary data plus all fields and values, etc.
 * CHPL_SYNTAX - the chapel syntax necessary to generate the object,
 *                or DEBUG_DETAIL if unable
*/
// TODO: move this into its own header
enum StringifyKind {
  DEBUG_SUMMARY,
  DEBUG_DETAIL,
  CHPL_SYNTAX
};

// define the generic stringify template
template<typename T> struct stringify {
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const T& stringMe) {
                    stringMe.stringify(streamOut, stringKind);
                  };
};

// define stringify for pointers to call stringify on a reference to avoid
//  duplication for types that appear both as references and as pointers
template<typename T> struct stringify<const T*> {
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const T* stringMe) const {
    stringify<T> stringifier;
    if (stringMe == nullptr) {
      streamOut << "nullptr";
    } else {
      stringifier(streamOut, stringKind, *stringMe);
    }
  }
};

// stringify a vector by stringifying each element; uses [a, b, c] formatting
template<typename T>
static inline void defaultStringifyVec(std::ostream& streamOut,
                                       StringifyKind stringKind,
                                       const std::vector<T>& stringVec) {
  if (stringVec.empty()) {
    streamOut << "[ ]";
  } else {
    streamOut << "[" ;
    std::string separator;
    stringify<T> vecString;
    for (const auto &vecVal : stringVec ) {
      vecString(streamOut << separator, stringKind, vecVal);
      separator = ", ";
    }
    streamOut << "]";
  }
}

// stringify an unordered map by stringifying each key, value pair
// uses {k1: v1, k2: v2} formatting
template<typename K, typename V>
static inline void defaultStringifyMap(std::ostream& streamOut,
                                       StringifyKind stringKind,
                                       const std::unordered_map<K,V>& stringMap)
{
  if (stringMap.size() == 0) {
    streamOut << "{ }";
  } else {
    std::string separator;
    stringify<K> keyString;
    stringify<V> valString;
    streamOut << "{";
    // since it's an unordered map, this iteration will occur in a
    // nondeterministic order.
    // it's important to sort the keys / iterate in a deterministic order here,
    // so we create a vector of pair<K,V> and sort that instead
    std::vector<std::pair<K,V>> mapVec(stringMap.begin(), stringMap.end());
    std::sort(mapVec.begin(), mapVec.end());
    for (auto const& x : mapVec)
    {
      streamOut << separator;
      keyString(streamOut, stringKind, x.first);
      streamOut << ": ";
      valString(streamOut, stringKind, x.second);

      separator = ", ";
    }
    streamOut << "}";
  }
}

// stringify a map by stringifying each key, value pair
// uses {k1: v1, k2: v2} formatting
  template<typename K, typename V>
  static inline void defaultStringifyMap(std::ostream& streamOut,
                                         StringifyKind stringKind,
                                         const std::map<K,V>& stringMap)
  {
    if (stringMap.size() == 0) {
      streamOut << "{ }";
    } else {
      std::string separator;
      stringify<K> keyString;
      stringify<V> valString;
      streamOut << "{";
      for (auto const& x : stringMap)
      {
        streamOut << separator;
        keyString(streamOut, stringKind, x.first);
        streamOut << ": ";
        valString(streamOut, stringKind, x.second);

        separator = ", ";
      }
      streamOut << "}";
    }
  }

// stringify a set by stringifying each element; uses {a, b, c} formatting
template<typename T>
static inline void defaultStringifySet(std::ostream& streamOut,
                                       StringifyKind stringKind,
                                       const std::set<T>& stringSet) {
  if (stringSet.empty()) {
    streamOut << "{ }";
  } else {
    std::string separator;
    stringify<T> tString;
    streamOut << "{";
    for (auto const& x : stringSet)
    {
      tString(streamOut << separator, stringKind, x);
      separator = ", ";
    }
    streamOut << "}";
  }
}

// stringify a tuple by stringifying each element; uses (a, b, c) formatting
template<typename TUP, size_t... I>
static inline void defaultStringifyTuple(std::ostream& streamOut,
                                         StringifyKind stringKind,
                                         const TUP& tuple,
                                         std::index_sequence<I...>) {
  // lambda to convert
  auto convert = [&streamOut, stringKind](bool printsep, auto& elem) {
    // we don't know what the type of `elem` is going to be, so we use
    // std::decay_t<decltype(x)> to get the type, so we can instantiate
    // the proper stringify struct
    chpl::stringify<std::decay_t<decltype(elem)>> stringifier;
    if (printsep) {
      streamOut << ", ";
    }
    stringifier(streamOut, stringKind, elem);
  };

  streamOut << "(";
  auto dummy = {(convert(I!=0, std::get<I>(tuple)),0)...};
  (void) dummy; // avoid unused variable warning
  streamOut << ")";
}

// stringify a pair by stringifying each item; uses (a, b) formatting
template<typename A, typename B>
static inline void defaultStringifyPair(std::ostream& streamOut,
                                        StringifyKind stringKind,
                                        const std::pair<A,B>& stringPair) {
  stringify<A> stringA;
  stringify<B> stringB;

  streamOut << "(";
  stringA(streamOut, stringKind, stringPair.first);
  stringB(streamOut << ", ", stringKind, stringPair.second);
  streamOut << ")";
}

template<> struct stringify<std::string> {
void operator()(std::ostream& streamOut,
               StringifyKind stringKind,
               const std::string& val) const {
  streamOut << "\"" << val << "\"";
}
};

/// \cond DO_NOT_DOCUMENT

/*
  Templates for integral types start here
*/

template<> struct stringify<int> {
void operator()(std::ostream& streamOut,
               StringifyKind stringKind,
               const int val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<unsigned int> {
void operator()(std::ostream& streamOut,
               StringifyKind stringKind,
               const unsigned int val) const {
  streamOut << std::to_string(val);
}
};
template<> struct stringify<long int> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const long int val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<unsigned long int> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const unsigned long int val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<long long int> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const long long int val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<unsigned long long int> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const unsigned long long int val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<short int> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const short int val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<unsigned short int> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const unsigned short int val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<char> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const char val) const {
  streamOut << val;
}
};

// end integral types

/*
  Floating Point Types
*/
template<> struct stringify<double> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const double val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<long double> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const long double val) const {
  streamOut << std::to_string(val);
}
};

template<> struct stringify<float> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const float val) const {
  streamOut << std::to_string(val);
}
};

// end floating-point types

template<> struct stringify<bool> {
void operator()(std::ostream& streamOut,
                StringifyKind stringKind,
                const bool val) const {
  streamOut << (val ? "true" : "false");
 }
};

template<typename T> struct stringify<std::vector<T>> {
 void operator()(std::ostream& streamOut,
                 StringifyKind stringKind,
                 const std::vector<T>& stringMe) const {
   defaultStringifyVec(streamOut, stringKind, stringMe);
 }
};

template<typename K, typename V> struct stringify<std::unordered_map<K,V>> {
 void operator()(std::ostream& streamOut,
                 StringifyKind stringKind,
                 const std::unordered_map<K,V>& stringMe) const {
   defaultStringifyMap(streamOut, stringKind, stringMe);
 }
};

template<typename K, typename V> struct stringify<std::map<K,V>> {
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const std::map<K,V>& stringMe) const {
    defaultStringifyMap(streamOut, stringKind, stringMe);
  }
};

template<typename A, typename B> struct stringify<std::pair<A,B>> {
 void operator()(std::ostream& streamOut,
                 StringifyKind stringKind,
                 const std::pair<A,B>& stringMe) const {
   defaultStringifyPair(streamOut, stringKind, stringMe);
 }
};

template<typename T> struct stringify<std::set<T>> {
 void operator()(std::ostream& streamOut,
                 StringifyKind stringKind,
                 const std::set<T>& stringMe) const {
   defaultStringifySet(streamOut, stringKind, stringMe);
}
};

template<typename... ArgTs> struct stringify<std::tuple<ArgTs...>> {
 void operator()(std::ostream& streamOut,
                 StringifyKind stringKind,
                 const std::tuple<ArgTs...>& stringMe) const {
   defaultStringifyTuple(streamOut,
                         stringKind,
                         stringMe,
                         std::index_sequence_for<ArgTs...>{});
  }
};

/**
 macros that define methods to write the object to std::cerr
 */
#define DECLARE_DUMP \
  void dump() const; \
  void dump(chpl::StringifyKind debug_level) const

#define IMPLEMENT_DUMP(T) \
  void T::dump() const { \
    dump(DEBUG_DETAIL); \
  } \
  void T::dump(chpl::StringifyKind debug_level) const { \
    stringify(std::cout, debug_level); \
    std::cout << std::endl; \
  }


/// \endcond


} // end namespace chpl

#endif
