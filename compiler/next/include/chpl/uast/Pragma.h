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

#ifndef CHPL_UAST_PRAGMA_H
#define CHPL_UAST_PRAGMA_H

namespace chpl {
namespace uast {
namespace pragmatags {


enum PragmaTag {
  PRAGMA_UNKNOWN,
// Discard everything but the name when building enum values.
#define PRAGMA(name__, canParse__, parseStr__, desc__) \
  PRAGMA_ ## name__,
#include "chpl/uast/PragmaList.h"
#undef PRAGMA 
  NUM_KNOWN_PRAGMAS
};

/** Return the string name associated with a PragmaTag */
const char* pragmaTagToName(PragmaTag tag);

/** Return the PragmaTag associated with a const char* */
PragmaTag pragmaNameToTag(const char* name);

} // end namespace pragmatags 

// Make pragmas available in `uast` namespace, e.g. uast::PRAGMA_REF
using namespace pragmatags;

} // end namespace uast
} // end namespace chpl

#endif
