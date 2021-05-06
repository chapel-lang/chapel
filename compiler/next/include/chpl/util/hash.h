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

#ifndef CHPL_UTIL_HASH_H
#define CHPL_UTIL_HASH_H

#include <cstddef>
#include <functional>
#include <string>
#include <tuple>
#include <utility>

namespace chpl {

// Combine two hash functions
inline size_t hash_combine(size_t hash, size_t other) {
  return hash ^ (other + 0x9e3779b9 + (hash << 6) + (hash >> 2));
}

constexpr size_t FNV_offset_basis = 0xcbf29ce484222325;
constexpr size_t FNV_prime        = 0x100000001b3;

// FNV-1a hash function for null-terminated C strings
inline size_t hash(const char* s)
{
  size_t seed = FNV_offset_basis;
  for(char c; c = *s; ++s)
    seed = (seed ^ static_cast<unsigned char>(c)) * FNV_prime;
  return seed;
}

// FNV-1a hash function for C++ std::string
inline size_t hash(const std::string& s)
{
  size_t seed = FNV_offset_basis;
  for(char c : s)
    seed = (seed ^ static_cast<unsigned char>(c)) * FNV_prime;
  return seed;
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
  size_t seed = 0;
  for (auto h : { hash(data)... })
    seed = hash_combine(seed, h);
  return seed;
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

} // end namespace chpl

#endif
