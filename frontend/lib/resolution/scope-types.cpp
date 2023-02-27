/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/Function.h"
#include "chpl/uast/NamedDecl.h"

#include "scope-help.h"

namespace chpl {
namespace resolution {


void OwnedIdsWithName::stringify(std::ostream& ss,
                                 chpl::StringifyKind stringKind) const {
  if (auto ptr = moreIdvs_.get()) {
    for (const auto& elt : *ptr) {
      elt.id_.stringify(ss, stringKind);
    }
  } else {
    if (!idv_.id_.isEmpty()) {
      idv_.id_.stringify(ss, stringKind);
    }
  }
}

llvm::Optional<BorrowedIdsWithName>
OwnedIdsWithName::borrow(IdAndVis::SymbolTypeFlags filterFlags) const {
  // Are all of the filter flags present in flagsOr?
  // If not, it is not possible for this to match.
  if ((flagsOr_ & filterFlags) != filterFlags) {
    return llvm::None;
  }

  if (BorrowedIdsWithName::isIdVisible(idv_, filterFlags)) {
    return BorrowedIdsWithName(*this, filterFlags);
  }
  // The first ID isn't visible; are others?
  if (moreIdvs_.get() == nullptr) {
    return llvm::None;
  }

  for (auto& idv : *moreIdvs_) {
    if (!BorrowedIdsWithName::isIdVisible(idv, filterFlags))
      continue;

    // Found a visible ID! Return a BorrowedIds referring to the whole thing
    return BorrowedIdsWithName(*this, filterFlags);
  }

  // No ID was visible, so we can't borrow.
  return llvm::None;
}

int BorrowedIdsWithName::countVisibleIds(IdAndVis::SymbolTypeFlags flagsAnd) {
  if (moreIdvs_ == nullptr) {
    return 1;
  }

  // if the current filter is a subset of flagsAnd, then all of the
  // found symbols will included in this borrowedIds, so we don't have
  // to consider them individually.
  if ((flagsAnd & filterFlags_) == filterFlags_) {
    // all of the found symbols will match
    return moreIdvs_->size();
  }

  // Otherwise, consider the individual IDs to count those that are included.
  int count = 0;
  for (const auto& idv : *moreIdvs_) {
    if (isIdVisible(idv)) {
      count += 1;
    }
  }
  return count;
}

bool BorrowedIdsWithName::containsOnlyMethodsOrFields() const {
  if (moreIdvs_ == nullptr) {
    if (isIdVisible(idv_)) {
      return idv_.isMethodOrField();
    }
  }

  for (const auto& idv : *moreIdvs_) {
    if (isIdVisible(idv)) {
      if (!idv.isMethodOrField()) {
        return false;
      }
    }
  }

  return true;
}

void BorrowedIdsWithName::stringify(std::ostream& ss,
                                    chpl::StringifyKind stringKind) const {
  for (const auto& elt : *this) {
    elt.stringify(ss, stringKind);
  }
}

Scope::Scope(const uast::AstNode* ast, const Scope* parentScope,
             bool autoUsesModules) {
  bool containsUseImport = false;
  bool containsFunctionDecls = false;
  bool containsExternBlock = false;
  bool isMethodScope = false;

  parentScope_ = parentScope;
  tag_ = ast->tag();
  id_ = ast->id();
  if (auto decl = ast->toNamedDecl()) {
    name_ = decl->name();
  }
  if (auto fn = ast->toFunction()) {
    isMethodScope = fn->isMethod();
  }
  gatherDeclsWithin(ast, declared_,
                    containsUseImport,
                    containsFunctionDecls,
                    containsExternBlock);

  // compute the flags storing a few settings
  ScopeFlags flags = 0;
  if (containsFunctionDecls) { flags |= CONTAINS_FUNCTION_DECLS; }
  if (containsUseImport) {     flags |= CONTAINS_USE_IMPORT; }
  if (autoUsesModules) {       flags |= AUTO_USES_MODULES; }
  if (isMethodScope) {         flags |= METHOD_SCOPE; }
  if (containsExternBlock) {   flags |= CONTAINS_EXTERN_BLOCK; }
  flags_ = flags;
}

void Scope::addBuiltin(UniqueString name) {
  // Just refer to empty ID since builtin type declarations don't
  // actually exist in the AST.
  // The resolver knows that the empty ID means a builtin thing.
  declared_.emplace(name,
                    OwnedIdsWithName(ID(),
                                     uast::Decl::PUBLIC,
                                     /*isMethodOrField*/ false));
}

const Scope* Scope::moduleScope() const {
  const Scope* cur;
  for (cur = this;
       cur != nullptr && !isModule(cur->tag());
       cur = cur->parentScope()) {
    ; // search for the containing module
  }
  return cur;
}

const Scope* Scope::parentModuleScope() const {
  auto modScope = this->moduleScope();
  if (auto ps = modScope->parentScope()) return ps->moduleScope();
  return nullptr;
}

bool Scope::lookupInScope(UniqueString name,
                          std::vector<BorrowedIdsWithName>& result,
                          IdAndVis::SymbolTypeFlags filterFlags) const {
  auto search = declared_.find(name);
  if (search != declared_.end()) {
    // There might not be any IDs that are visible to us, so borrow returns
    // an optional list.
    auto borrowedIds = search->second.borrow(filterFlags);
    if (borrowedIds.hasValue()) {
      result.push_back(std::move(borrowedIds.getValue()));
      return true;
    }
  }
  return false;
}

bool Scope::contains(UniqueString name) const {
  auto search = declared_.find(name);
  return (search != declared_.end());
}

std::set<UniqueString> Scope::gatherNames() const {
  std::set<UniqueString> orderedNames;
  for (const auto& pair : declared_) {
    orderedNames.insert(pair.first);
  }

  return orderedNames;
}

void Scope::stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
  ss << "Scope ";
  ss << tagToString(tag());
  ss << " kind=";
  id().stringify(ss, stringKind);
  ss << " numDeclared=";
  ss << std::to_string(numDeclared());
}

void VisibilitySymbols::stringify(std::ostream& ss,
                                  chpl::StringifyKind stringKind) const {
  ss << "VisibilitySymbols";
  if (scope_) {
    ss << " mod-scope=(";
    scope_->stringify(ss, stringKind);
    ss << " )";
  }
  const char* kindStr = "<unknown>";
  switch (kind_) {
    case SYMBOL_ONLY:
      kindStr = "symbol-only";
      break;
    case ALL_CONTENTS:
      kindStr = "all-contents";
      break;
    case ONLY_CONTENTS:
      kindStr = "only-contents";
      break;
    case CONTENTS_EXCEPT:
      kindStr = "contents-except";
      break;
  }
  ss << " kind=" << kindStr;

  if (isPrivate_) {
    ss << " private";
  } else {
    ss << " public";
  }

  const char* shadowStr = "<unknown>";
  switch (shadowScopeLevel_) {
    case REGULAR_SCOPE:
      shadowStr = "regular";
      break;
    case SHADOW_SCOPE_ONE:
      shadowStr = "shadow-scope-one";
      break;
    case SHADOW_SCOPE_TWO:
      shadowStr = "shadow-scope-two";
      break;
  }

  if (shadowScopeLevel_ != REGULAR_SCOPE) {
    ss << " " << shadowStr;
  }

  for (auto pair : names_) {
    ss << " " << pair.first.c_str();
    if (pair.first != pair.second) {
      ss << " as " << pair.second.c_str();
    }
  }
}

void ResolvedVisibilityScope::stringify(std::ostream& ss,
                                        chpl::StringifyKind stringKind) const {

  if (scope_) {
    ss << " cur-scope=(";
    scope_->stringify(ss, stringKind);
    ss << " )";
  }

  int i = 0;
  for (const auto& clause : visibilityClauses_) {
    ss << "  clause " << i << "(";
    clause.stringify(ss, stringKind);
    ss << ")";
    i++;
  }
}

IMPLEMENT_DUMP(OwnedIdsWithName);
IMPLEMENT_DUMP(BorrowedIdsWithName);
IMPLEMENT_DUMP(Scope);
IMPLEMENT_DUMP(VisibilitySymbols);
IMPLEMENT_DUMP(ResolvedVisibilityScope);
IMPLEMENT_DUMP(PoiScope);
IMPLEMENT_DUMP(InnermostMatch);

} // end namespace resolution
} // end namespace chpl
