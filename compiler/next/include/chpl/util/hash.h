/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_HASH_H
#define CHPL_UTIL_HASH_H

#include <cstddef>
#include <functional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace chpl {

// Combine two hash functions
inline size_t hash_combine(size_t hash, size_t other) {
  return hash ^ (other + 0x9e3779b9 + (hash << 6) + (hash >> 2));
}

constexpr uint64_t FNV_offset_basis = 0xcbf29ce484222325;
constexpr uint64_t FNV_prime        = 0x100000001b3;

// FNV-1a hash function for null-terminated C strings
inline size_t hash(const char* s)
{
  uint64_t seed = FNV_offset_basis;
  for(char c; (c = *s) != '\0'; ++s)
    seed = (seed ^ static_cast<unsigned char>(c)) * FNV_prime;
  return (size_t) seed;
}

// FNV-1a hash function for strings with length
inline size_t hash(const char* s, size_t len)
{
  uint64_t seed = FNV_offset_basis;
  for(size_t i = 0; i < len; i++)
    seed = (seed ^ static_cast<unsigned char>(s[i])) * FNV_prime;
  return (size_t) seed;
}

// FNV-1a hash function for C++ std::string
inline size_t hash(const std::string& s)
{
  uint64_t seed = FNV_offset_basis;
  for(char c : s)
    seed = (seed ^ static_cast<unsigned char>(c)) * FNV_prime;
  return (size_t) seed;
}

// Default hash function for one argument
template<typename T>
inline size_t hash(const T& x) {
  std::hash<T> hasher;
  return hasher(x);
}

// Default hash function for zero or more than one argument
template<typename... Ts>
inline size_t hash(const Ts&... data) {
  size_t ret = 0;
  for (auto h : { hash(data)... })
    ret = hash_combine(ret, h);
  return ret;
}

// Hash function for std::tuple
template<typename TUP, size_t... I>
inline size_t hash_tuple_impl(const TUP& tuple, std::index_sequence<I...>) {
  return hash(std::get<I>(tuple)...);
}

template<typename... Ts>
inline size_t hash(const std::tuple<Ts...>& tuple) {
  return hash_tuple_impl(tuple, std::index_sequence_for<Ts...>{});
}
static inline size_t hash(const std::tuple<>& tuple) {
  return 0;
}

// Hash function for vector
template<typename T>
inline size_t hashVector(const std::vector<T>& key) {
  size_t ret = 0;
  for (const auto& elt : key) {
    ret = hash_combine(ret, hash(elt));
  }
  return ret;
}

// Hash function for set
template<typename T>
inline size_t hashSet(const std::set<T>& key) {
  size_t ret = 0;
  for (const auto& elt : key) {
    ret = hash_combine(ret, hash(elt));
  }
  return ret;
}

// Hash function for pair
template<typename T, typename U>
inline size_t hashPair(const std::pair<T, U>& key) {
  size_t ret = 0;
  ret = hash_combine(ret, hash(key.first));
  ret = hash_combine(ret, hash(key.second));
  return ret;
}



} // end namespace chpl

namespace std {

// These std::hash functions are here b/c the hash functions
// above can't have partial template specialization.
template<typename T> struct hash<std::vector<T>> {
  size_t operator()(const std::vector<T>& key) const {
    return chpl::hashVector(key);
  }
};
template<typename T> struct hash<std::set<T>> {
  size_t operator()(const std::set<T>& key) const {
    return chpl::hashSet(key);
  }
};
template<typename T, typename U> struct hash<std::pair<T,U>> {
  size_t operator()(const std::pair<T,U>& key) const {
    return chpl::hashPair(key);
  }
};



} // end namespace std


#endif
