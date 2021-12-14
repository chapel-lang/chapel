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

  stringify can be used to get a string representation of an object
  for debugging purposes or whenever a string representation of an
  object is needed
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
#include <sstream>
#include <algorithm>


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

enum StringifyKind {
  DEBUG_SUMMARY,
  DEBUG_DETAIL,
  CHPL_SYNTAX
};

// force this to always build so it is available during debug
template<typename T>
void debugPrint(const T &arg) {
stringify<T> stringTemplate;
stringTemplate(std::cout, chpl::StringifyKind::DEBUG_DETAIL, arg);
}

// define the generic stringify template which will cause a compilation error
//  if used. Query argument and return types need to have a specialization of
//  stringify
template<typename T> struct stringify {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const T& stringMe) {
                    stringOut << stringMe.toString(stringKind);
                  };
};

// define stringify for pointers to call stringify on a reference to avoid
//  duplication for types that appear both as references and as pointers
template<typename T> struct stringify<const T*> {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const T* stringMe) const {
    stringify<T> stringifier;
    stringifier(stringOut, stringKind, *stringMe);
  }
};

// stringify a vector by stringifying each element; uses [a, b, c] formatting
template<typename T>
static inline void defaultStringifyVec(std::ostream &stringOut,
                                       StringifyKind stringKind,
                                       const std::vector<T>& stringVec) {
  if (stringVec.empty()) {
   stringOut << "[ ]";
  } else {
   std::ostringstream ss;
   std::string separator;
   stringify<T> vecString;
   for (const auto &vecVal : stringVec ) {
     vecString(ss << separator, stringKind, vecVal);
     separator = ", ";
   }
   stringOut << "[" << ss.str() << "]";
  }
}

// stringify a map by stringifying each key, value pair
// uses {k1: v1, k2: v2} formatting
template<typename K, typename V>
static inline void defaultStringifyMap(std::ostream &stringOut,
                                       StringifyKind stringKind,
                                       const std::unordered_map<K,V>& stringMap)
{
  if (stringMap.size() == 0) {
    stringOut << "{ }";
  } else {
    std::ostringstream ss;
    std::string separator;
    stringify<K> keyString;
    stringify<V> valString;

    // since it's an unordered map, this iteration will occur in a
    // nondeterministic order.
    // it's important to sort the keys / iterate in a deterministic order here,
    // so we create a vector of pair<K,V> and sort that instead
    std::vector<std::pair<K,V>> mapVec(stringMap.begin(), stringMap.end());
    std::sort(mapVec.begin(), mapVec.end());
    for (auto const& x : mapVec)
    {
      ss << separator
         << keyString(stringKind, x.first)
         << ": "
         << valString(stringKind, x.second);

      separator = ", ";
    }
    stringOut << "{" << ss.str() << "}";
  }
}

// stringify a set by stringifying each element; uses {a, b, c} formatting
template<typename T>
static inline void defaultStringifySet(std::ostream &stringOut,
                                       StringifyKind stringKind,
                                       const std::set<T>& stringSet) {
  if (stringSet.size() == 0) {
    stringOut << "{ }";
  } else {
    std::ostringstream ss;
    std::string separator;
    stringify<T> tString;

    for (auto const& x : stringSet)
    {
      tString(ss << separator, stringKind, x);
      separator = ", ";
    }
    stringOut << "{" << ss.str() << "}";
  }
}

// stringify a tuple by stringifying each element; uses (a, b, c) formatting
template<typename TUP, size_t... I>
static inline void defaultStringifyTuple(std::ostream &stringOut,
                                         StringifyKind stringKind,
                                         const TUP& tuple,
                                         std::index_sequence<I...>) {
  // lambda to convert
  auto convert = [](bool printsep, StringifyKind stringKind, auto& elem) {
    // we don't know what the type of `elem` is going to be, so we use
    // std::decay_t<decltype(x)> to get the type, so we can instantiate
    // the proper stringify struct
    chpl::stringify<std::decay_t<decltype(elem)>> stringifier;
    std::ostringstream ss;
    stringifier(ss, stringKind, elem);
    if (printsep) {
      ss << ", ";
    }
    return ss.str();
  };

  std::ostringstream ss;
  auto dummy = {(ss << convert(I!=0, stringKind, std::get<I>(tuple)),0)...};
  (void) dummy; // avoid unused variable warning
  stringOut << "(" << ss.str() << ")";
}

// stringify a pair by stringifying each item; uses (a, b) formatting
template<typename A, typename B>
static inline void defaultStringifyPair(std::ostream &stringOut,
                                        StringifyKind stringKind,
                                        const std::pair<A,B>& stringPair) {
 stringify<A> stringA;
 stringify<B> stringB;

 stringOut << "(";
 stringA(stringOut, stringKind, stringPair.first);
 stringB(stringOut << ", ", stringKind, stringPair.second);
 stringOut << ")";
}

/// \cond DO_NOT_DOCUMENT
template<> struct stringify<std::string> {
void operator()(std::ostream &stringOut,
               StringifyKind stringKind,
               const std::string& val) const {
 stringOut << "\"" << val << "\"";
}
};


/*
  Templates for integral types start here
*/

template<> struct stringify<int> {
void operator()(std::ostream &stringOut,
               StringifyKind stringKind,
               const int val) const {
 stringOut << std::to_string(val);
}
};

template<> struct stringify<unsigned int> {
void operator()(std::ostream &stringOut,
               StringifyKind stringKind,
               const unsigned int val) const {
 stringOut << std::to_string(val);
}
};
template<> struct stringify<long int> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const long int val) const {
  stringOut << std::to_string(val);
}
};

template<> struct stringify<unsigned long int> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const unsigned long int val) const {
  stringOut << std::to_string(val);
}
};

template<> struct stringify<long long int> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const long long int val) const {
  stringOut << std::to_string(val);
}
};

template<> struct stringify<unsigned long long int> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const unsigned long long int val) const {
  stringOut << std::to_string(val);
}
};

template<> struct stringify<short int> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const short int val) const {
  stringOut << std::to_string(val);
}
};

template<> struct stringify<unsigned short int> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const unsigned short int val) const {
  stringOut << std::to_string(val);
}
};

// end integral types

/*
  Floating Point Types
*/
template<> struct stringify<double> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const double val) const {
  stringOut << std::to_string(val);
}
};

template<> struct stringify<long double> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const long double val) const {
  stringOut << std::to_string(val);
}
};

template<> struct stringify<float> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const float val) const {
  stringOut << std::to_string(val);
}
};

// end floating-point types

template<> struct stringify<bool> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const bool val) const {
  stringOut << (val ? "true" : "false");
 }
};

template<typename T> struct stringify<std::vector<T>> {
 void operator()(std::ostream &stringOut,
                 StringifyKind stringKind,
                 const std::vector<T>& stringMe) const {
   defaultStringifyVec(stringOut, stringKind, stringMe);
 }
};

template<typename K, typename V> struct stringify<std::unordered_map<K,V>> {
 void operator()(std::ostream &stringOut,
                 StringifyKind stringKind,
                 const std::unordered_map<K,V>& stringMe) const {
   defaultStringifyMap(stringOut, stringKind, stringMe);
 }
};

template<typename A, typename B> struct stringify<std::pair<A,B>> {
 void operator()(std::ostream &stringOut,
                 StringifyKind stringKind,
                 const std::pair<A,B>& stringMe) const {
   defaultStringifyPair(stringOut, stringKind, stringMe);
 }
};

template<typename T> struct stringify<std::set<T>> {
void operator()(std::ostream &stringOut,
                StringifyKind stringKind,
                const std::set<T>& stringMe) const {
  defaultStringifySet(stringOut, stringKind, stringMe);
}
};

template<typename... ArgTs> struct stringify<std::tuple<ArgTs...>> {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const std::tuple<ArgTs...>& stringMe) const {
    defaultStringifyTuple(stringOut,
                          stringKind,
                          stringMe,
                          std::index_sequence_for<ArgTs...>{});
  }
};
/// \endcond

} // end namespace chpl

#endif
