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

#include "chpl/resolution/scope-types.h"

#include "chpl/uast/AstTag.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/NamedDecl.h"

#include "scope-help.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Config/llvm-config.h"

namespace chpl {
namespace resolution {

IdAndFlags::IdAndFlags(ID id, bool isPublic, bool isMethodOrField,
                       bool isParenfulFunction, bool isMethod,
                       bool isModule, bool isType)
  : id_(std::move(id)) {

  // setup the flags
  Flags flags = 0;
  if (isPublic) {
    flags |= PUBLIC;
  } else {
    flags |= NOT_PUBLIC;
  }
  if (isMethodOrField) {
    flags |= METHOD_FIELD;
  } else {
    flags |= NOT_METHOD_FIELD;
  }
  if (isParenfulFunction) {
    flags |= PARENFUL_FUNCTION;
  } else {
    flags |= NOT_PARENFUL_FUNCTION;
  }
  if (isMethod) {
    flags |= METHOD;
  } else {
    flags |= NOT_METHOD;
  }
  if (isModule) {
    flags |= MODULE;
  } else {
    flags |= NOT_MODULE;
  }
  if (isType) {
    flags |= TYPE;
  } else {
    flags |= NOT_TYPE;
  }

  flags_ = flags;
}

std::string IdAndFlags::flagsToString(Flags flags) {
  std::string ret;
  if ((flags & PUBLIC) != 0)                ret += "public ";
  if ((flags & NOT_PUBLIC) != 0)            ret += "!public ";

  if ((flags & METHOD_FIELD) != 0)          ret += "method/field ";
  if ((flags & NOT_METHOD_FIELD) != 0)      ret += "!method/field ";

  if ((flags & PARENFUL_FUNCTION) != 0)     ret += "parenful-fn ";
  if ((flags & NOT_PARENFUL_FUNCTION) != 0) ret += "!parenful-fn ";

  if ((flags & METHOD) != 0)     ret += "method ";
  if ((flags & NOT_METHOD) != 0) ret += "!method ";

  if ((flags & MODULE) != 0)     ret += "module ";
  if ((flags & NOT_MODULE) != 0) ret += "!module ";

  if ((flags & TYPE) != 0)     ret += "type ";
  if ((flags & NOT_TYPE) != 0) ret += "!type ";
  return ret;
}

void IdAndFlags::stringify(std::ostream& ss,
                           chpl::StringifyKind stringKind) const {
  id_.stringify(ss, stringKind);
  ss << "[";
  ss << flagsToString(flags_);
  ss << "]";
}

using Flags = IdAndFlags::Flags;
using FlagSet = IdAndFlags::FlagSet;

FlagSet FlagSet::singleton(Flags flags) {
  FlagSet toReturn;
  toReturn.addDisjunction(flags);
  return toReturn;
}

FlagSet FlagSet::empty() {
  return FlagSet();
}

void FlagSet::addDisjunction(Flags excludeFlags) {
  // booleans, like all lattices, follow the absorption law:
  //
  //     a ∧ (a ∨ b) = a   and   a ∨ (a ∧ b) = a
  //
  // Since Flags elements represent conjunction, if a & b = a,
  // we know that b has all of flags in a, and maybe more. Thus,
  // logically, b = a ∧ b', where b' is the "more". But then, by the
  // absorption law,
  //
  //     a ∨ b = a ∨ (a ∧ b') = a
  //
  // In other words, if a & b = a, then only a needs to be in the
  // FlagSet (which is a disjunction of flags). First try finding such a pair,
  // so that we can keep the size of the set small.
  for (auto& otherFlags : flagVec) {
    if ((otherFlags & excludeFlags) == otherFlags) {
      // excludeFlags is subsumed, we're done.
      return;
    }
    if ((excludeFlags & otherFlags) == excludeFlags) {
      // Existing entry is subsumed.
      otherFlags = excludeFlags;
      return;
    }
  }

  // Performance: another possible optimization is the detection of two disjuncts
  // being logical negations of each other (particularly if each disjunct is
  // a single flag). For instance, if we have
  //
  //    IdAndFlags::METHOD_FIELD ∨ ...
  //
  // And we add IdAndFlags::NOT_METHOD_FIELD. In that case, we get:
  //
  //    IdAndFlags::METHOD_FIELD ∨ IdAndFlags::NOT_METHOD_FIELD ∨ ...
  //
  // Which is logically equivalent to:
  //
  //    True ∨ ... = True
  //
  // This means we can replace the whole disjunction with a single term,
  // True, aka the empty Flags value 0.

  // We didn't find a pair eligible for merging, so just insert.
  flagVec.push_back(excludeFlags);
}

bool FlagSet::subsumes(Flags mightBeSubsumed) const {
  for (auto& otherFlags : flagVec) {
    if ((otherFlags & mightBeSubsumed) == otherFlags) {
      // excludeFlags is subsumed, we're done.
      return true;
    }
  }
  return false;
}

bool FlagSet::noneMatch(Flags match) const {
  return std::all_of(flagVec.begin(), flagVec.end(), [&](auto excludeFlags) {
    return (match & excludeFlags) != excludeFlags;
  });
}

bool FlagSet::operator==(const FlagSet& other) const {
  return flagVec == other.flagVec;
}

bool FlagSet::operator!=(const FlagSet& other) const {
  return !(*this == other);
}

size_t FlagSet::hash() const {
  size_t ret = 0;
  for (auto excludeFlags : flagVec) {
    ret = hash_combine(ret, chpl::hash(excludeFlags));
  }
  return ret;
}

void FlagSet::mark(Context* context) const {
  // nothing, because flags don't need to be marked.
}

bool
OwnedIdsWithName::gatherMatches(MatchingIdsWithName& dst,
                                IdAndFlags::Flags filterFlags,
                                const IdAndFlags::FlagSet& excludeFlagSet) const
{
  const OwnedIdsWithName& ownedIds = *this;

  // Are all of the filter flags present in flagsOr?
  // If not, it is not possible for this to match.
  if ((ownedIds.flagsOr_ & filterFlags) != filterFlags) {
    return false;
  }

  const IdAndFlags* beginIds = nullptr;
  const IdAndFlags* endIds = nullptr;
  if (ownedIds.moreIdvs_ == nullptr) {
    beginIds = &ownedIds.idv_;
    endIds = beginIds + 1;
  } else {
    const IdAndFlags* last = nullptr;
    beginIds = &ownedIds.moreIdvs_->front();
    last = &ownedIds.moreIdvs_->back();
    endIds = last + 1;
  }

  bool anyAppended = false;
  for (auto cur = beginIds; cur != endIds; ++cur) {
    if (cur->matchesFilter(filterFlags, excludeFlagSet)) {
      dst.idvs_.push_back(*cur);
      anyAppended = true;
    }
  }

  return anyAppended;

}

void OwnedIdsWithName::stringify(std::ostream& ss,
                                 chpl::StringifyKind stringKind) const {
  if (auto ptr = moreIdvs_.get()) {
    for (const auto& elt : *ptr) {
      elt.id_.stringify(ss, stringKind);
      ss << " ";
    }
  } else {
    if (!idv_.id_.isEmpty()) {
      idv_.id_.stringify(ss, stringKind);
    }
  }
}

void MatchingIdsWithName::removeDuplicateIds() {
  std::unordered_set<IdAndFlags> s;

  // remove duplicate IDs in the idvs_ vector
  size_t end = idvs_.size();
  size_t cur = 0; // the position to fill in
  for (size_t i = 0; i < end; i++) {
    auto pair = s.insert(idvs_[i]);
    if (pair.second == true) {
      // It was inserted, so it was unique.
      // Include it in the result
      // by storing it into the element 'cur'
      if (i != cur) {
        idvs_[cur] = idvs_[i];
      }
      cur++;
    }
  }

  if (cur != end) {
#if LLVM_VERSION_MAJOR >= 14
    idvs_.truncate(cur);
#else
    idvs_.resize(cur);
#endif
  }
}

void MatchingIdsWithName::truncate(int sz) {
  CHPL_ASSERT(0 <= sz && sz <= (int) idvs_.size());
#if LLVM_VERSION_MAJOR >= 14
  idvs_.truncate(sz);
#else
  idvs_.resize(sz);
#endif

}

void MatchingIdsWithName::clear() {
  idvs_.clear();
}

bool MatchingIdsWithName::containsOnlyMethodsOrFields() const {
  for (const auto& idv : idvs_) {
    if (!idv.isMethodOrField()) {
      return false;
    }
  }

  return true;
}

void MatchingIdsWithName::stringify(std::ostream& ss,
                                    chpl::StringifyKind stringKind) const {
  for (const auto& elt : *this) {
    elt.stringify(ss, stringKind);
  }
}

bool lookupInDeclMap(const DeclMap& declared,
                     UniqueString name,
                     MatchingIdsWithName& result,
                     IdAndFlags::Flags filterFlags,
                     const IdAndFlags::FlagSet& excludeFlags) {
  auto search = declared.find(name);
  if (search != declared.end()) {
    return search->second.gatherMatches(result, filterFlags, excludeFlags);
  }
  return false;
}


Scope::Scope(Context* context,
             const uast::AstNode* ast, const Scope* parentScope,
             bool autoUsesModules) {
  bool containsUseImport = false;
  bool containsFunctionDecls = false;
  bool containsExternBlock = false;
  bool containsRequire = false;
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
  gatherDeclsWithin(context, ast, declared_,
                    containsUseImport,
                    containsFunctionDecls,
                    containsExternBlock,
                    containsRequire);

  // compute the flags storing a few settings
  ScopeFlags flags = 0;
  if (containsFunctionDecls) { flags |= CONTAINS_FUNCTION_DECLS; }
  if (containsUseImport) {     flags |= CONTAINS_USE_IMPORT; }
  if (containsRequire) {       flags |= CONTAINS_REQUIRE; }
  if (autoUsesModules) {       flags |= AUTO_USES_MODULES; }
  if (isMethodScope) {         flags |= METHOD_SCOPE; }
  if (containsExternBlock) {   flags |= CONTAINS_EXTERN_BLOCK; }
  flags_ = flags;
}

void Scope::addBuiltinVar(UniqueString name) {
  // Just refer to empty ID since builtin type declarations don't
  // actually exist in the AST.
  // The resolver knows that the empty ID means a builtin thing.
  declared_.emplace(name, OwnedIdsWithName(IdAndFlags::createForBuiltinVar()));
}

void Scope::addBuiltinType(UniqueString name) {
  // Just refer to empty ID since builtin type declarations don't
  // actually exist in the AST.
  // The resolver knows that the empty ID means a builtin thing.
  declared_.emplace(name, OwnedIdsWithName(IdAndFlags::createForBuiltinType()));
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

void Scope::collectNames(std::set<UniqueString>& namesDefined,
                         std::set<UniqueString>& namesDefinedMultiply) const {
  for (const auto& decl : declared_) {
    UniqueString name = decl.first;
    if (!name.isEmpty() && name != USTR("_")) {
      auto p = namesDefined.insert(name);
      if (p.second == false || decl.second.numIds() > 1) {
        // it was already present or multiply defined here
        namesDefinedMultiply.insert(name);
      }
    }
  }
}

void Scope::stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
  ss << "Scope ";
  ss << tagToString(tag()) << " ";
  id().stringify(ss, stringKind);

  if (stringKind != StringifyKind::DEBUG_SUMMARY) {
    ss << " numDeclared=";
    ss << std::to_string(numDeclared());
  }
}

bool VisibilitySymbols::mightHaveName(UniqueString name) const {
  UniqueString unused;

  switch (kind_) {
    case SYMBOL_ONLY:
    case ONLY_CONTENTS:
      return lookupName(name, unused);
    case ALL_CONTENTS:
      return true;
    case CONTENTS_EXCEPT:
      return !lookupName(name, unused);
  }
  return false;
}


bool VisibilitySymbols::lookupName(UniqueString name,
                                   UniqueString &declared) const {
  for (const auto &p : names_) {
    if (p.second == name) {
      declared = p.first;
      return true;
    }
  }
  return false;
}

const std::vector<std::pair<UniqueString,UniqueString>>&
VisibilitySymbols::names() const {
  return names_;
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

  for (const auto& id : modulesNamedInUseOrImport_) {
    ss << "  names " << id.str();
  }
}

void ModulePublicSymbols::stringify(std::ostream& ss,
                                    chpl::StringifyKind stringKind) const {
  ss << "ModulePublicSymbols {";
  for (const auto& pair: syms_) {
    ss << pair.first.str() << " ";
    pair.second.stringify(ss, stringKind);
    ss << "\n";
  }
  ss << "}\n";
}

MethodLookupHelper::~MethodLookupHelper() { }

ReceiverScopeHelper::~ReceiverScopeHelper() { }


IMPLEMENT_DUMP(IdAndFlags);
IMPLEMENT_DUMP(OwnedIdsWithName);
IMPLEMENT_DUMP(MatchingIdsWithName);
IMPLEMENT_DUMP(Scope);
IMPLEMENT_DUMP(VisibilitySymbols);
IMPLEMENT_DUMP(ResolvedVisibilityScope);
IMPLEMENT_DUMP(PoiScope);
IMPLEMENT_DUMP(InnermostMatch);
IMPLEMENT_DUMP(ModulePublicSymbols);


} // end namespace resolution
} // end namespace chpl
