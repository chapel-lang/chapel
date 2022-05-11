/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/scope-types.h"

#include "chpl/uast/NamedDecl.h"

#include "scope-help.h"

namespace chpl {
namespace resolution {


Scope::Scope(const uast::AstNode* ast, const Scope* parentScope,
             bool autoUsesModules) {
  parentScope_ = parentScope;
  tag_ = ast->tag();
  autoUsesModules_ = autoUsesModules;
  id_ = ast->id();
  if (auto decl = ast->toNamedDecl()) {
    name_ = decl->name();
  }
  gatherDeclsWithin(ast, declared_, containsUseImport_, containsFunctionDecls_);
}

void Scope::addBuiltin(UniqueString name) {
  // Just refer to empty ID since builtin type declarations don't
  // actually exist in the AST.
  // The resolver knows that the empty ID means a builtin thing.
  declared_.emplace(name, ID());
}

IMPLEMENT_DUMP(BorrowedIdsWithName);
IMPLEMENT_DUMP(Scope);
IMPLEMENT_DUMP(PoiScope);
IMPLEMENT_DUMP(InnermostMatch);

} // end namespace resolution
} // end namespace chpl
