/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

/* this sets up the necessaary macros to code-generate methods from method tables. */
#include "core-types-gen-help.h"

static const char* blockStyleToString(BlockStyle blockStyle) {
  switch (blockStyle) {
    case BlockStyle::EXPLICIT: return "explicit";
    case BlockStyle::IMPLICIT: return "implicit";
    case BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK: return "unnecessary";
    default: return "";
  }
}

static const char* opKindToString(Range::OpKind kind) {
  switch (kind) {
    case Range::DEFAULT: return "..";
    case Range::OPEN_HIGH: return "..<";
    default: return "";
  }
}

static std::optional<chpl::Location> getValidLocation(const chpl::Location& loc) {
  /*isEmpty doesn't work since that only relies upon path, which is set*/
  if (loc.line() != -1) {
    return loc;
  }
  return std::nullopt;
}

/* This invokes the macros, performing the code-generation. */
#include "method-tables/uast-methods.h"
