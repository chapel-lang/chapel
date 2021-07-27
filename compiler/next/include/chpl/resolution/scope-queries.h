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
    Returns true if this AST type can create a scope.
   */
  bool createsScope(uast::ASTTag tag);

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
    Given a Scope, compute the ResolvedVisibilityScope
    by processing the use/import statements in order.

    If the scope didn't have use/imports, returns nullptr.
   */
  const ResolvedVisibilityScope* resolveVisibilityStmts(Context* context,
                                                        const Scope* scope);

  /**
    Given an Expression and a Scope, return the things
    that Expression might refer to.

    * If checkDecls is true, looks for symbols declared in this Scope.
    * If checkUseImport is true, looks for symbols from use/import statements in
      this Scope.
    * If checkParents is true, looks for symbols from parent scopes
      (but not parent modules of a module) including looking for declarations
      and handling imports, and including finding declarations in the root
      module.
    * If checkToplevel is true, checks for a toplevel module with this name.
    * If findOne is true, limits search to the innermost scope with a match.

    Expression can be Identifier or Dot.
   */
  std::vector<BorrowedIdsWithName> lookupInScope(Context* context,
                                                 const Scope* scope,
                                                 const uast::Expression* expr,
                                                 bool checkDecls,
                                                 bool checkUseImport,
                                                 bool checkParents,
                                                 bool checkToplevel,
                                                 bool findOne);

  /**
    Same as lookupInScope above but uses a name instead of an
    Expression.
   */
  std::vector<BorrowedIdsWithName> lookupNameInScope(Context* context,
                                                     const Scope* scope,
                                                     UniqueString name,
                                                     bool checkDecls,
                                                     bool checkUseImport,
                                                     bool checkParents,
                                                     bool checkToplevel,
                                                     bool findOne);

  /**
    Same as lookupInScope but includes a set tracking visited scopes.
   */
  std::vector<BorrowedIdsWithName>
  lookupInScopeWithSet(Context* context,
                       const Scope* scope,
                       const uast::Expression* expr,
                       bool checkDecls,
                       bool checkUseImport,
                       bool checkParents,
                       bool checkToplevel,
                       bool findOne,
                       std::unordered_set<const Scope*>& visited);

  /**
    Same as lookupNameInScope but includes a set tracking visited scopes.
   */
  std::vector<BorrowedIdsWithName>
  lookupNameInScopeWithSet(Context* context,
                           const Scope* scope,
                           UniqueString name,
                           bool checkDecls,
                           bool checkUseImport,
                           bool checkParents,
                           bool checkToplevel,
                           bool findOne,
                           std::unordered_set<const Scope*>& visited);

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
  const PoiScope* poiScope(Context* context,
                           const Scope* scope,
                           const PoiScope* parentPoiScope);

 // TODO: lookupInScope accepting visited set
  // for use in POI traversal

  /**
    Given a name and a Scope, return the innermost and first ID
    for a definition of that name, and an indication of
    whether 0, 1, or more matches were found.
   */
  const InnermostMatch& findInnermostDecl(Context* context,
                                          const Scope* scope,
                                          UniqueString name);


} // end namespace resolution
} // end namespace chpl
#endif
