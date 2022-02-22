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


Scope::Scope(const uast::ASTNode* ast, const Scope* parentScope) {
  parentScope_ = parentScope;
  tag_ = ast->tag();
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

void
Scope::stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {

  // Print out header information (type of AST node, num entries).
  ss << tagToString(tag()) << " '";
  name_.stringify(ss, stringKind);
  ss << "' (id: " ;
  id().stringify(ss, stringKind);
  ss << ", " << std::to_string(numDeclared()) << " entries)";
  ss << std::endl;

  // Find the largest entry name to pretty print...
  int largestEntrySize = 0;
  for (const auto& entry : declared_) {
    int entryLength = entry.first.length();
    largestEntrySize = entryLength > largestEntrySize
        ? entryLength
        : largestEntrySize;
  }

  // Print out each entry.
  for (const auto& entry : declared_) {
    auto borrowedIds = BorrowedIdsWithName(entry.second);
    ss << "  ";
    entry.first.stringify(ss, stringKind);

    // Add spaces to align columns.
    const int entryLength = entry.first.length();
    const int numSpaces = largestEntrySize - entryLength;
    for (int i = 0; i < numSpaces; i++) ss << " ";

    ss << " : " << std::to_string(borrowedIds.numIds()) << " ids";
    ss << std::endl;
  }
}
IMPLEMENT_DUMP(BorrowedIdsWithName);
IMPLEMENT_DUMP(Scope);
IMPLEMENT_DUMP(PoiScope);
IMPLEMENT_DUMP(InnermostMatch);

} // end namespace resolution
} // end namespace chpl
