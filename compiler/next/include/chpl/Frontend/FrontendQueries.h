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

#ifndef CHPL_FRONTEND_FRONTENDQUERIES_H
#define CHPL_FRONTEND_FRONTENDQUERIES_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/ID.h"
#include "chpl/AST/Location.h"
#include "chpl/AST/ModuleDecl.h"
#include "chpl/Queries/Context.h"

#include <vector>

namespace chpl {

// This could alternatively be a class with static methods
namespace FrontendQueries {
  const std::string& fileText(Context* context, UniqueString path);
  const ast::Builder::Result* parseFile(Context* context, UniqueString path);

  using LocationsMap = std::unordered_map<ID, Location>;
  const LocationsMap& fileLocations(Context* context, UniqueString path);
  Location locate(Context* context, ID id);

  using ModuleDeclVec = std::vector<const ast::ModuleDecl*>;
  const ModuleDeclVec& parse(Context* context, UniqueString path);

  /*
  typedef std::unordered_map<UniqueString, Symbol*> SymbolsByName;
  const SymbolsByName& symbolsDeclaredIn(Context* context, Expr* expr);

  struct ResolutionResult {
    // the expr that is resolved
    Expr* expr;
    // in simple cases, this is set
    Symbol* symbol;
    // TODO:
    //  return-intent overloading
    //  generic instantiation
    //  establish concrete intents
    //  establish copy-init vs move
    ResolutionResult() : expr(NULL), symbol(NULL) { }
  };

  typedef std::vector<ResolutionResult> ResolutionResultByPostorderID;

    If resolve is called on a Decl, it will traverse into the declared
    Symbol. So to resolve a function, run resolve on the FunctionDecl.
  const ResolutionResultByPostorderID& resolve(Context* context, Expr* e);
  */

  /*
  struct DeclaredByName {
    std::unordered_map<UniqueString, Symbol*> declaredSymbols;
  };*/


  struct DefinedTopLevelNames {
    // the module
    const ast::Module* module;
    // these are in program order
    std::vector<UniqueString> topLevelNames;
    DefinedTopLevelNames(const ast::Module* module,
                         std::vector<UniqueString> topLevelNames)
      : module(module), topLevelNames(std::move(topLevelNames)) {
    }
  };
  using DefinedTopLevelNamesVec = std::vector<DefinedTopLevelNames>;

  const DefinedTopLevelNamesVec& moduleLevelDeclNames(Context* context,
                                                      UniqueString path);

  /*struct ResolutionGroup {
    // index is the postorder ID
    std::vector<Symbol*> idToSymbol;
  };
  using ModuleInitResolutionResult = std::unordered_map<Module*,owned<ResolutionGroup>>;
    */
  /*using FunctionResolutionResult =
    std::unordered_map<FnSymbol*,owned<ResolutionResult>>; */

  //const ast::BaseAST* ast(Context* context, ID id);
};

} // end namespace chpl


#endif
