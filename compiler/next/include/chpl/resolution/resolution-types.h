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

#ifndef CHPL_RESOLUTION_RESOLUTION_TYPES_H
#define CHPL_RESOLUTION_RESOLUTION_TYPES_H

#include "chpl/types/Type.h"
#include "chpl/uast/ASTNode.h"

namespace chpl {
namespace resolution {


struct ResolutionResult {
  // the expr that is resolved
  const uast::Expression* expr = nullptr;
  // For simple cases, which named decl does it refer to?
  const uast::NamedDecl* decl = nullptr;
  // What is its type?
  const types::Type* type = nullptr;
  // For a function call, it might refer to several Functions
  // and we might not know which return intent to choose yet.
  std::vector<const uast::Function*> otherFns;
  // TODO:
  //  establishing types
  //  return-intent overloading
  //  generic instantiation
  //  establish concrete intents
  //  establish copy-init vs move
  ResolutionResult() { }
};

// postorder ID (int) -> ResolutionResult *within* a Function etc
// an inner Function would not be covered here since it would get
// a different ResolvedSymbol entry.
using ResolutionResultByPostorderID = std::vector<ResolutionResult>;

// A resolution result for a Function, Module, or TypeDecl (Record/Class/etc)
struct ResolvedSymbol {
  // the following are input for the resolution process but these
  // are repeated here in case they are needed in follow-on processing.

  // the NamedDecl that is resolved
  const uast::NamedDecl* decl = nullptr;
  // substitutions (only used for generic functions/types)
  std::unordered_map<uast::NamedDecl*, types::Type*> typeSubs;
  // TODO: int -> Immediate
  std::unordered_map<uast::NamedDecl*, int> paramSubs;
  // the point of instantiation
  const ResolvedSymbol* instantiationPoint = nullptr;

  // this is the output of the resolution process
  ResolutionResultByPostorderID resolutionById;
};

using ResolvedSymbolVec = std::vector<const ResolvedSymbol*>;

/*
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
*/


} // end namespace resolution
} // end namespace chpl
#endif
