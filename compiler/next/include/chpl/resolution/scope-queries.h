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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/scope-types.h"

namespace chpl {
namespace resolution {

  /**
   Returns the AST node of the parent Scoping Symbol given an ID.
   For example, inside of a Function, this will return the Function.
   Inside of a Module, it will return the Module.

   If given the ID for a top-level Module, returns nullptr.
   */
  //const uast::ASTNode* parentScopingSymbol(Context* context, ID id);


  /**
    Returns the ID of the parent uAST for a Scoping Symbol -- that is,
    something for which Builder::astTagIndicatesNewIdScope returns true,
    such as Modules, Functions, and Classes.

    This is different from parentScopingSymbol because it might return,
    say, the ID of an inner Block.
   */
  //ID parentExprForScopingSymbol(Context* context, ID id);

  /**
    Returns the Scope for a Scoping Symbol -- that is,
    something for which Builder::astTagIndicatesNewIdScope returns true,
    such as Modules, Functions, and Classes.
   */
  //const Scope* scopeForScopingSymbol(Context* context, ID id);

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
    Given a Scope, compute the resolved imports
    by processing the use/import statements in order.

    If the scope didn't have use/imports, returns nullptr.
   */
  const ResolvedImportScope* resolveImports(Context* context,
                                            const Scope* scope);

  /**
    Given a name and a Scope, return the innermost and first ID
    for a definition of that name.
   */
  const std::pair<ID, int>& findInnermostDecl(Context* context,
                                              const Scope* scope,
                                              UniqueString name);

  /**
    Returns the parent Scope for an expression. The parent Scope contains
    NamedDecls defined there as well as use/import statements.
   */
  //const Scope& parentScope(Context* context, const uast::ASTNode* ast);

  /*
  // Resolves the top-level declarations in a module
  const ResolvedSymbol&
  resolveModule(Context* context, const uast::Module* mod);

  const ResolvedSymbolVec& resolveFile(Context* context, UniqueString path);
   */

  /*
  const DefinedTopLevelNamesVec& moduleLevelDeclNames(Context* context,
                                                      UniqueString path);
   */


} // end namespace resolution
} // end namespace chpl
#endif
