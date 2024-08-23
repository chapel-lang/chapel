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

#ifndef CHPL_RESOLUTION_SCOPE_QUERIES_H
#define CHPL_RESOLUTION_SCOPE_QUERIES_H

#include "chpl/resolution/scope-types.h"

#include "llvm/ADT/ArrayRef.h"

namespace chpl {
namespace resolution {

  /**
    Returns true if this AST type can create a scope.
   */
  bool createsScope(uast::AstTag tag);

  /**
    Returns the Scope for an ID.
   */
  const Scope* scopeForId(Context* context, ID id);

  /**
    Given an ID for a Module,
    returns a Scope that represents the Module's scope
    (and notes the symbols that are defined in it).
   */
  const Scope* scopeForModule(Context* context, ID moduleId);


  const bool& isNameBuiltinGenericType(Context* context, UniqueString name);

  /**
    The configuration used to look up a plain identifier in a scope
    when resolving expressions.
   */
  const LookupConfig IDENTIFIER_LOOKUP_CONFIG = LOOKUP_DECLS |
                                                LOOKUP_IMPORT_AND_USE |
                                                LOOKUP_PARENTS |
                                                LOOKUP_EXTERN_BLOCKS;

  /**
    Find what a name might refer to.

    'scope' is the context in which the name occurs (e.g. as an Identifier)

    'receiverScopes' is the scope of a type containing the name, in the case of
    method calls, field accesses, and resolving a name within a method.  It is a
    Scope representing the record/class/union itself for the receiver. If
    provided, the receiverScopes will be consulted after declarations within
    'scope' but before its parents. It accepts multiple scopes in order to
    handle classes with inheritance.

    The config argument is a group of or-ed together bit flags that adjusts the
    behavior of the lookup. Please see 'LookupConfig' and the related constants
    such as 'LOOKUP_DECLS' for further details.
   */
  std::vector<BorrowedIdsWithName>
  lookupNameInScope(Context* context,
                    const Scope* scope,
                    llvm::ArrayRef<const Scope*> receiverScopes,
                    UniqueString name,
                    LookupConfig config);

  /**
    Same as lookupNameInScope but can produce warnings based on
    the ID passed in.
   */

  std::vector<BorrowedIdsWithName>
  lookupNameInScopeWithWarnings(Context* context,
                                const Scope* scope,
                                llvm::ArrayRef<const Scope*> receiverScopes,
                                UniqueString name,
                                LookupConfig config,
                                ID idForWarnings);


  /**
    Same as lookupNameInScope but traces how each symbol was found,
    for error messages.
   */
  std::vector<BorrowedIdsWithName>
  lookupNameInScopeTracing(Context* context,
                           const Scope* scope,
                           llvm::ArrayRef<const Scope*> receiverScopes,
                           UniqueString name,
                           LookupConfig config,
                           std::vector<ResultVisibilityTrace>& traceResult);

  /**
    Same as lookupNameInScope but includes a set tracking visited scopes.
   */
  std::vector<BorrowedIdsWithName>
  lookupNameInScopeWithSet(Context* context,
                           const Scope* scope,
                           const llvm::ArrayRef<const Scope*> receiverScopes,
                           UniqueString name,
                           LookupConfig config,
                           CheckedScopes& visited);

  /**
    Collect all symbols that are available in this scope, including ones
    brought in through visibility statements. This function follows the
    same rules as lookupNameInScope, except it collects all symbols instead
    of one with a specific name.

    Currently, this is only intended for tool support; the resolver itself
    should rely on lookupNameInScope.
   */
  std::map<UniqueString, BorrowedIdsWithName>
  getSymbolsAvailableInScope(Context* context,
                            const Scope* scope);

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
   Given a scope, returns a vector of IDs for all the modules and enums that
   were either used or imported in that scope. May return an empty vector if
   no modules were used or imported in the scope.
   */
  const std::vector<ID> findUsedImportedIds(Context* context,
                                            const Scope* scope);

  /**
   Given a ID for a module, returns a vector of IDs for all the modules
   that are used, imported, or mentioned in that module.
   */
  const std::vector<ID>& findMentionedModules(Context* context, ID modId);

  /**
    Resolve the uses and imports in a given scope.

    If 'skipPrivate' is set, avoids resolving visibility statements that
    only expose scope-private symbols. This helps avoid unnecessary work.
  */
  const ResolvedVisibilityScope*
  resolveVisibilityStmts(Context* context, const Scope* scope, bool skipPrivate = false);

  /**
    Return the scope for the automatically included 'ChapelStandard' module,
    or nullptr if it could not be found.
  */
  const Scope* scopeForAutoModule(Context* context);

  /**
    Given the ID for the main module, compute the order in which
    modules should be initialized. 'commandLineModules' can be provided
    with the list of modules that are named on the command line.

    The result is vector of IDs indicating the order in which
    modules with those IDs should be initialized.
  */
  const std::vector<ID>&
  moduleInitializationOrder(Context* context, ID mainModule,
                            std::vector<ID> commandLineModules);

  /**
    Check for symbol names with multiple definitions within a scope.
    This query only exists to emit errors.
   */
  void emitMultipleDefinedSymbolErrors(Context* context, const Scope* scope);


  /**
    Given a Scope* for a Module, return a DeclMap containing all symbols
    publically available from that module, including those brought in
    transitively by use/import. */
  const ModulePublicSymbols* publicSymbolsForModule(Context* context, const Scope* modScope);


} // end namespace resolution
} // end namespace chpl
#endif
