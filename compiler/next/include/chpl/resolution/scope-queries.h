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

#ifndef CHPL_RESOLUTION_SCOPE_QUERIES_H
#define CHPL_RESOLUTION_SCOPE_QUERIES_H

#include "chpl/resolution/scope-types.h"

namespace chpl {
namespace resolution {


  /**
    Returns the Scope for an ID.
    It does this by finding the parent ID and then computing the Scope.
    Things that don't have a scope?
   */
  const Scope* scopeForId(Context* context, ID id);

  /**
    Given an ID for a Module, returns a Scope that represents
    the Module scope (and what symbols are defined in it).
   */
  const Scope* scopeForModule(Context* context, ID moduleId);

  /**
    Given a Scope, compute the ResolvedVisibilityScope
    by processing the use/import statements in order.

    If the scope didn't have use/imports, returns nullptr.
   */
  const ResolvedVisibilityScope* resolveVisibilityStmts(Context* context,
                                                        const Scope* scope);

  /**
    Given a name and a Scope, return the innermost and first ID
    for a definition of that name, and an integer indicating:
     * 0 if no such name was found
     * 1 if just one match for the name was found
     * 2 if multiple matches for the name were found
   */
  const std::pair<ID, int>& findInnermostDecl(Context* context,
                                              const Scope* scope,
                                              UniqueString name);


} // end namespace resolution
} // end namespace chpl
#endif
