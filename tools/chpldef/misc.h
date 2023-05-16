/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef CHPL_TOOLS_CHPLDEF_MISC_H
#define CHPL_TOOLS_CHPLDEF_MISC_H

#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/framework/query-impl.h"
#include "chpl/util/filesystem.h"
#include "chpl/util/printf.h"
#include "llvm/Config/llvm-config.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include <cstdlib>
#include <string>
#include <type_traits>

#if __cplusplus < 201703L
  #error "Expected C++17 standard!"
#endif

// Enable a declaration if 'U' is derived from 'T', for return type 'RT'.
#define CHPLDEF_ENABLE_IF_DERIVED(child__, parent__, rtype__) \
  std::enable_if_t<std::is_base_of<parent__, child__>::value, rtype__>

#define CHPLDEF_IMPOSSIBLE() \
  do { CHPL_ASSERT(false && "Should not be possible!"); } while (0)

#define CHPLDEF_TODO() \
  do { CHPL_ASSERT(false && "Not implemented yet!"); } while (0)

#define CHPLDEF_FATAL(server__, ...) \
  do { \
    server__->message("FATAL [%s:%d]...\n", __FUNCTION__, __LINE__); \
    server__->message(__VA_ARGS__); \
    std::abort(); \
  } while (0)

#ifndef DOXYGEN
  #define CHPLDEF_PFMT(p1__, p2__, ...) \
    __VA_ARGS__ __attribute__ ((format (printf, p1__, p2__)))
#else
  #define CHPLDEF_PFMT(p1__, p2__, ...) __VA_ARGS__
#endif

namespace chpldef {

/** Types are convenience wrappers around LLVM's JSON types. */
using JsonValue = llvm::json::Value;
using JsonObject = llvm::json::Object;
using JsonPath = llvm::json::Path;
using JsonMapper = llvm::json::ObjectMapper;

/** Wrapper around LLVM's optional type. */
#if LLVM_VERSION_MAJOR >= 16
  #include <optional>
  static_assert(std::is_same<std::optional<T>, llvm::Optional<T>>::value);
  template <typename T> using opt = std::optional<T>;
#else
  template <typename T> using opt = llvm::Optional<T>;
#endif

template <typename T>
inline opt<T> option(T&& t) { return opt<T>(std::move(t)); }

template <typename T>
inline opt<T> option(const T& t) { return opt<T>(t); }

/** Cast a string to an integer, return 'false' if there was an error. */
inline bool cast(std::string str, int& out) {
  try {
    out = std::stoi(str);
    return true;
  } catch (const std::exception& ex) { std::ignore = ex; }
  return false;
}

/** Print a JSON value's tag as a string. */
const char* jsonTagStr(const JsonValue& json);

/** Print a JSON value. */
std::string jsonToString(const JsonValue& json, bool pretty=false);

} // end namespace 'chpldef'

#endif
