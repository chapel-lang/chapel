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

#include "chpl/uast/Pragma.h"
#include <cstring>

namespace chpl {
namespace uast {
namespace pragmatags {


const char* pragmaTagToName(PragmaTag tag) {
  const char* ret = "";
  switch (tag) {
    default: break;
#define PRAGMA(name__, canParse__, parseStr__, desc__) \
    case PRAGMA_ ## name__: ret = parseStr__; break;
#include "chpl/uast/PragmaList.h"
#undef PRAGMA
  }

  return ret;
}

PragmaTag pragmaNameToTag(const char* name) {
#define PRAGMA(name__, canParse__, parseStr__, desc__) \
  if (0 == strcmp(name, parseStr__)) \
    return PRAGMA_ ## name__;
#include "chpl/uast/PragmaList.h"
#undef PRAGMA
  return PRAGMA_UNKNOWN;
}


} // namespace primtags
} // namespace uast
} // namespace chpl
