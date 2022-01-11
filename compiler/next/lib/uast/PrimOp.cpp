/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/PrimOp.h"

#include <cstring>

namespace chpl {
namespace uast {
namespace primtags {


const char* primTagToName(PrimitiveTag tag) {
  switch (tag) {
#define PRIMITIVE(NAME, str) \
    case PRIM_ ## NAME : \
      return str;

#define PRIMITIVE_G(NAME, str) PRIMITIVE(NAME, str)
#define PRIMITIVE_R(NAME, str) PRIMITIVE(NAME, str)
#include "chpl/uast/PrimOpsList.h"
    case NUM_KNOWN_PRIMS:
      return "";
#undef PRIMITIVE_G
#undef PRIMITIVE_R
#undef PRIMITIVE
  }

  return "";
}

PrimitiveTag primNameToTag(const char* name) {
#define PRIMITIVE(NAME, str) \
  if (0 == strcmp(str, name)) \
    return PRIM_ ## NAME;

#define PRIMITIVE_G(NAME, str) PRIMITIVE(NAME, str)
#define PRIMITIVE_R(NAME, str) PRIMITIVE(NAME, str)
#include "chpl/uast/PrimOpsList.h"

#undef PRIMITIVE_G
#undef PRIMITIVE_R
#undef PRIMITIVE
  return PRIM_UNKNOWN;
}


} // namespace primtags
} // namespace uast
} // namespace chpl
