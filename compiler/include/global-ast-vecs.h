/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _GLOBAL_AST_VECS_
#define _GLOBAL_AST_VECS_

#include "baseAST.h"
#include "PassManager.h"

// NOTE: these global vectors are declared separately so that uses of
// them are now explicit in each TU. Additionally, the include can be
// pushed down so that they are only in scope after that point. This
// is to enable us to begin transitioning passes away from accessing
// the global vectors without having to break out the pass into a
// separate file for old and new

//
// declare global vectors for all AST node types
//
// These global vectors, named gSymExprs, gCallExprs, gFnSymbols, ...,
// contain all existing nodes of the given AST node type; they are
// updated automatically as new AST nodes are constructed.  Nodes are
// removed from these vectors between passes.
//
#define decl_gvecs(type) extern Vec<type*> g##type##s
foreach_ast(decl_gvecs);
#undef decl_gvecs

// This function is a workaround for not being able to '.join()' multiple
// containers together into a sort of "concat range" etc (that's what
// the new C++26 edition will call it). There are some passes that need to
// run over all symbols, and this is a convenient way to do that. This is
// defined here as opposed to in 'PassManager.h' because of the need to
// "push down" this include.
template <typename T, typename R>
void runPassOverAllSymbols(PassManager& pm, PassT<T, R>&& pass) {
  // TODO
  // This specific iteration order keeps us from iterating over new types
  // and fields that are created as a result of this pass (e.g., new 'ref'
  // types and their fields). A more robust solution should be built into
  // the pass manager that keeps it from working on newly created AST:
  //
  //  -- We could record the last created AST ID when we start iterating
  //     and skip any AST that is '>' than that ID
  //  -- We could have a more robust system of keeping track of when AST
  //     is created by a pass, and use that to check
  //  -- ...
  //
  pm.runPass<Symbol*>(pass, gVarSymbols);
  pm.runPass<Symbol*>(pass, gShadowVarSymbols);
  pm.runPass<Symbol*>(pass, gTypeSymbols);
  pm.runPass<Symbol*>(pass, gArgSymbols);
  pm.runPass<Symbol*>(pass, gFnSymbols);
}

#endif
