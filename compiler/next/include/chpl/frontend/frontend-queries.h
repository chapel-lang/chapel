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

#include "chpl/queries/Context.h"
#include "chpl/queries/ID.h"
#include "chpl/queries/Location.h"
#include "chpl/uast/ASTBase.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/Exp.h"
#include "chpl/uast/ModuleDecl.h"

#include <vector>

namespace chpl {

namespace frontend {
  const std::string& fileText(Context* context, UniqueString path);
  void setFileText(Context* context, UniqueString path, std::string result);

  const uast::Builder::Result& parseFile(Context* context, UniqueString path);

  using LocationsMap = std::unordered_map<const uast::ASTBase*, Location>;
  const LocationsMap& fileLocations(Context* context, UniqueString path);
  const Location& locate(Context* context, const uast::ASTBase* ast);

  using ModuleDeclVec = std::vector<const uast::ModuleDecl*>;
  const ModuleDeclVec& parse(Context* context, UniqueString path);

  struct ResolutionResult {
    // the expr that is resolved
    const uast::Exp* exp;
    // in simple cases, this is set
    const uast::Decl* decl;
    // TODO:
    //  return-intent overloading
    //  generic instantiation
    //  establish concrete intents
    //  establish copy-init vs move
    ResolutionResult() : exp(nullptr), decl(nullptr) { }
  };

  // postorder ID (int) -> ResolutionResult
  using ResolutionResultByPostorderID = std::vector<ResolutionResult>;

  // Resolves the top-level declarations in a module
  const ResolutionResultByPostorderID&
  resolveModule(Context* context, const uast::Module* mod);

  struct ResolvedModule {
    const uast::Module* module;
    const ResolutionResultByPostorderID* resolution;
    ResolvedModule(const uast::Module* module,
                   const ResolutionResultByPostorderID* resolution)
      : module(module), resolution(resolution) {
    }
    static bool update(ResolvedModule& keep, ResolvedModule& addin);
  };
  using ResolvedModuleVec = std::vector<ResolvedModule>;

  const ResolvedModuleVec& resolveFile(Context* context, UniqueString path);

  struct DefinedTopLevelNames {
    // the module
    const uast::Module* module;
    // these are in program order
    std::vector<UniqueString> topLevelNames;
    DefinedTopLevelNames(const uast::Module* module,
                         std::vector<UniqueString> topLevelNames)
      : module(module), topLevelNames(std::move(topLevelNames)) {
    }
  };
  using DefinedTopLevelNamesVec = std::vector<DefinedTopLevelNames>;

  const DefinedTopLevelNamesVec& moduleLevelDeclNames(Context* context,
                                                      UniqueString path);

  //const uast::BaseAST* ast(Context* context, ID id);
};

} // end namespace chpl


#endif
