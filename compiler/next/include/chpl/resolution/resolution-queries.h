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

#ifndef CHPL_RESOLUTION_RESOLUTION_QUERIES_H
#define CHPL_RESOLUTION_RESOLUTION_QUERIES_H

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace resolution {

  struct ResolutionResult {
    // the expr that is resolved
    const uast::Expression* exp;
    // in simple cases, this is set
    const uast::NamedDecl* decl;
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


} // end namespace resolution
} // end namespace chpl
#endif
