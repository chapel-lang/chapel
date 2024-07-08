/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_COPY_ELISION_H
#define CHPL_RESOLUTION_COPY_ELISION_H

#include "chpl/framework/ID.h"
#include "chpl/resolution/resolution-types.h"

#include <set>

namespace chpl {
namespace resolution {


/* Computes the set of IDs of initialization points
   that can be 'move's rather than '=' or 'copy-init'.
     * If the elided initialization point is a variable initialization expr,
       the ID of the initialized variable is stored in the set.
     * If the elided initialization point is an '=' call,
       the ID of the OpCall is stored in the set.
     * If the elided initialization point is an actual passed by 'in'
       intent, the ID of the actual is stored in the set.
     * If the elided initialization point is a 'yield' statement, the
       ID of the 'yield' is stored in the set.

   Does not consider copy elision that only work with temporary
   variables (e.g. acceptsWithIn(returnsByValue())).

   This is not a query. It may raise errors within the currently running query.

   allSplitInitedVars can be computed by computeSplitInits.
 */
std::set<ID>
computeElidedCopies(Context* context,
                    const uast::AstNode* symbol,
                    const ResolutionResultByPostorderID& byPostorder,
                    const PoiScope* poiScope,
                    const std::set<ID>& allSplitInitedVars,
                    types::QualifiedType fnYieldedType);


} // end namespace resolution
} // end namespace chpl

#endif
