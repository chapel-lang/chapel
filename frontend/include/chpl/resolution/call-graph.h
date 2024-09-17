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

#ifndef CHPL_RESOLUTION_CALL_GRAPH_H
#define CHPL_RESOLUTION_CALL_GRAPH_H

#include "chpl/framework/ID.h"
#include "chpl/resolution/resolution-types.h"

#include <unordered_set>

namespace chpl {
namespace resolution {


using CalledFnsSet = std::unordered_set<const ResolvedFunction*>;

/* Gather ResolvedFunctions called directly by this function into a set.
   This function does not consider transitive calls. */
void gatherFnsCalledByFn(Context* context,
                         const ResolvedFunction* fn,
                         CalledFnsSet& called);

/* Gather ResolvedFunctions called transitively by this function into a set. */
void gatherTransitiveFnsCalledByFn(Context* context,
                                   const ResolvedFunction* fn,
                                   CalledFnsSet& called);

/* Gather ResolvedFunctions called directly by module initialization code
   for this module into a set.
   This function does not consider transitive calls. */
void gatherFnsCalledByModInit(Context* context,
                              ID moduleId,
                              CalledFnsSet& called);

/* Gather ResolvedFunctions called transitively by module initialization code
   for this module into a set. */
void gatherTransitiveFnsCalledByModInit(Context* context,
                                        ID moduleId,
                                        CalledFnsSet& called);


} // end namespace resolution
} // end namespace chpl

#endif
