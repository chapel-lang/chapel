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

#ifndef CHPL_UAST_PRIM_OP_H
#define CHPL_UAST_PRIM_OP_H

namespace chpl {
namespace uast {
namespace primtags {


/** An enum containing the various PrimOp values e.g. PRIM_MOVE. */
enum PrimitiveTag {
#define PRIMITIVE_G(NAME, str) PRIM_ ## NAME ,
#define PRIMITIVE_R(NAME, str) PRIM_ ## NAME ,
#include "chpl/uast/PrimOpsList.h"
  NUM_KNOWN_PRIMS
#undef PRIMITIVE_G
#undef PRIMITIVE_R
};

/** Return the string name associated with a PrimitiveTag */
const char* primTagToName(PrimitiveTag tag);

/** Return the PrimitiveTag associated with a const char* */
PrimitiveTag primNameToTag(const char* name);

} // end namespace primtags

// make PRIM_MOVE etc values available as uast::PRIM_MOVE e.g.
using namespace primtags;

} // end namespace uast
} // end namespace chpl

#endif
