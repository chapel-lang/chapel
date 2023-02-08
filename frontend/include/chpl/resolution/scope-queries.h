/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallPtrSet.h"

namespace chpl {
namespace resolution {

  using ScopeSet = llvm::SmallPtrSet<const Scope*, 5>;
  using NamedScopeSet = std::unordered_set<std::pair<UniqueString, const Scope*>>;

  /**
    Returns true if this AST type can create a scope.
   */
  bool createsScope(uast::AstTag tag);

  /**
    Returns the Scope for an ID.
   */
  const Scope* scopeForId(Context* context, ID id);

  /**
    Given an ID for a Module, returns a Scope that represents
    the Module scope (and what symbols are defined in it).
   */
  const Scope* scopeForModule(Context* context, ID moduleId);

  /**
    Find what a name might refer to.

    'scope' is the context in which the name occurs (e.g. as an Identifier)

    'receiverScopes' is the scope of a type containing the name, in the case
    of method calls, field accesses, and resolving a name within a method.
    It is a Scope representing the record/class/union itself for the
    receiver. If provided, the receiverScopes will be consulted before
    'scope' and its parents.

    The config argument is a group of or-ed together bit flags
    that adjusts the behavior of the lookup:

    * If LOOKUP_DECLS is set, looks for symbols declared in 'scope'
      and 'receiverScopes'.
    * If LOOKUP_IMPORT_AND_USE is set, looks for symbols from use/import
      statements in this 'scope' and 'receiverScopes'.
    * If LOOKUP_PARENTS is set, looks for symbols from parent scopes (but not
      parent modules of a module) including looking for declarations and
      handling imports, and including finding declarations in the root module.
    * If LOOKUP_TOPLEVEL is set, checks for a toplevel module with this name.
    * If LOOKUP_INNERMOST is true, limits search to the innermost scope with a
      match.
   */
  std::vector<BorrowedIdsWithName>
  lookupNameInScope(Context* context,
                    const Scope* scope,
                    llvm::ArrayRef<const Scope*> receiverScopes,
                    UniqueString name,
                    LookupConfig config);

  /**
    Same as lookupNameInScope but includes a set tracking visited scopes.
   */
  std::vector<BorrowedIdsWithName>
  lookupNameInScopeWithSet(Context* context,
                           const Scope* scope,
                           const llvm::ArrayRef<const Scope*> receiverScopes,
                           UniqueString name,
                           LookupConfig config,
                           NamedScopeSet& visited);

  /**
    Returns true if all of checkScope is visible from fromScope
    due to scope containment or whole-module use statements.
   */
  bool isWholeScopeVisibleFromScope(Context* context,
                                    const Scope* checkScope,
                                    const Scope* fromScope);

  /**
    Returns a unique'd point-of-instantiation scope for the passed
    scope and parent POI scope. Collapses away POI scopes that
    do not affect visible functions.
   */
  const PoiScope* pointOfInstantiationScope(Context* context,
                                            const Scope* scope,
                                            const PoiScope* parentPoiScope);

  /**
    Given a name and a Scope, return the innermost and first ID
    for a definition of that name, and an indication of
    whether 0, 1, or more matches were found.
   */
  const InnermostMatch& findInnermostDecl(Context* context,
                                          const Scope* scope,
                                          UniqueString name);


  /**
   * Given a scope, returns a list of IDs for all the modules that were either
   * used or imported in that scope. May return an empty vector if no modules
   * were used or imported in the scope.
   */
  const std::vector<ID> findUsedImportedModules(Context* context,
                                                const Scope* scope);

  /**
    Resolve the uses and imports in a given scope.
  */
  void resolveUsesAndImportsInScope(Context* context, const Scope* scope);


} // end namespace resolution
} // end namespace chpl
#endif
