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

#include "llvm/ADT/Optional.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"

#include <cstdlib>
#include <string>

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

/** Wrapper around LLVM's optional type. */
template <typename T> using opt = llvm::Optional<T>;

template <typename T>
inline opt<T> option(T&& t) { return opt<T>(std::move(t)); }

template <typename T>
inline opt<T> option(const T& t) { return opt<T>(t); }

/** Cast a string to an integer, return 'true' if there was an error. */
inline bool cast(std::string str, int& out) {
  try {
    out = std::stoi(str);
    return false;
  } catch (const std::exception& ex) { std::ignore = ex; }
  return true;
}

/** Print a JSON value's tag as a string. */
const char* jsonTagStr(const JsonValue& json);

/** Print a JSON value. */
std::string jsonToString(const JsonValue& json, bool pretty=false);

} // end namespace 'chpldef'

#endif
