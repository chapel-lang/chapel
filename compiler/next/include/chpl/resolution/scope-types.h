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

#ifndef CHPL_RESOLUTION_SCOPE_TYPES_H
#define CHPL_RESOLUTION_SCOPE_TYPES_H

#include "chpl/types/Type.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/util/memory.h"

#include <unordered_map>

namespace chpl {
namespace resolution {

// TODO: Should some/all of these structs be classes
// with getters etc? That would be appropriate for
// use as part of the library API.

/**
  Collects IDs with a particular name.
 */
struct OwnedIdsWithName {
  // If there is just one ID with this name, it is stored here,
  // and moreIds == nullptr.
  ID id;
  // If there is more than one, all are stored in here,
  // and id redundantly stores the first one.
  // This field is 'owned' in order to allow reuse of pointers to it.
  owned<std::vector<ID>> moreIds;

  OwnedIdsWithName(ID id)
    : id(id), moreIds(nullptr)
  { }

  void appendId(ID newId) {
    if (moreIds.get() == nullptr) {
      // create the vector and add the single existing id to it
      moreIds = toOwned(new std::vector<ID>());
      moreIds->push_back(id);
    }
    // add the id passed
    moreIds->push_back(std::move(newId));
  }

  bool operator==(const OwnedIdsWithName& other) const {
    if (id != other.id)
      return false;

    if ((moreIds.get()==nullptr) != (other.moreIds.get()==nullptr))
      return false;

    if (moreIds.get()==nullptr && other.moreIds.get()==nullptr)
      return true;

    // otherwise check the vector elements
    return *moreIds.get() == *other.moreIds.get();
  }
  bool operator!=(const OwnedIdsWithName& other) const {
    return !(*this == other);
  }
};
struct BorrowedIdsWithName {
  // TODO: consider storing a variant of ID here
  // with symbolPath, postOrderId, and tag
  ID id;
  const std::vector<ID>* moreIds = nullptr;
  BorrowedIdsWithName() { }
  BorrowedIdsWithName(ID id) : id(id) { }
  BorrowedIdsWithName(const OwnedIdsWithName& o)
    : id(o.id), moreIds(o.moreIds.get())
  { }
  bool operator==(const BorrowedIdsWithName& other) const {
    return id == other.id &&
           moreIds == other.moreIds;
  }
  bool operator!=(const BorrowedIdsWithName& other) const {
    return !(*this == other);
  }
};

// DeclMap: key - string name,  value - vector of ID of a NamedDecl
// Using an ID here prevents needing to recompute the Scope
// if (say) something in the body of a Function changed
using DeclMap = std::unordered_map<UniqueString, OwnedIdsWithName>;

/**
  A scope roughly corresponds to a `{ }` block. Anywhere a new symbol could be
  defined / is defined is a scope.

  While generic instantiations generate something scope-like, the
  point-of-instantiation reasoning will need to be handled with a different
  type.
 */
// TODO: adjust Conditional to contain Blocks so we can associate
// scopes 1:1 with these blocks.
// TODO: also adjust Function to store body in a separate node from args
// TODO: also adjust Loops to store body separate from index variables
struct Scope {
  const Scope* parentScope = nullptr;
  uast::asttags::ASTTag tag = uast::asttags::NUM_AST_TAGS;
  bool containsUseImport = false;
  ID id;
  DeclMap declared;

  Scope() { }

  bool operator==(const Scope& other) const {
    bool match =  parentScope == other.parentScope &&
                  tag == other.tag &&
                  containsUseImport == other.containsUseImport &&
                  id == other.id &&
                  declared.size() == other.declared.size();
    if (match) {
      // check also the contents of the maps
      for (const auto& pair : declared) {
        UniqueString key = pair.first;
        const OwnedIdsWithName& val = pair.second;
        // look up the same key in other
        auto search = other.declared.find(key);
        if (search == other.declared.end()) {
          return false;
        }
        // check that they values are the same
        const OwnedIdsWithName& otherVal = search->second;
        if (val != otherVal) {
          return false;
        }
      }
    }
    return match;
  }
  bool operator!=(const Scope& other) const {
    return !(*this == other);
  }
};

// This class supports both use and import
struct ImportedSymbols {
  ID symbolId;       // ID of the imported symbol, e.g. ID of a Module
  enum Kind {
    SYMBOL_ONLY,     // the named symbol itself only (one name in names)
    ALL_CONTENTS,    // (and names is empty)
    ONLY_CONTENTS,   // only the contents named in names
    CONTENTS_EXCEPT, // except the contents named in names (no renaming)
  };
  Kind kind = SYMBOL_ONLY;
  bool isPrivate = true;

  // the names/renames:
  //  pair.first is the name as declared
  //  pair.second is the name here
  std::vector<std::pair<UniqueString,UniqueString>> names;

  ImportedSymbols() { }
  ImportedSymbols(ID symbolId, Kind kind, bool isPrivate,
                  std::vector<std::pair<UniqueString,UniqueString>> names)
    : symbolId(symbolId), kind(kind), isPrivate(isPrivate),
      names(std::move(names))
  { }


  bool operator==(const ImportedSymbols& other) const {
    return symbolId == other.symbolId &&
           kind == other.kind &&
           names == other.names;
  }
  bool operator!=(const ImportedSymbols& other) const {
    return !(*this == other);
  }

  void swap(ImportedSymbols& other) {
    symbolId.swap(other.symbolId);
    Kind tmp = kind;
    kind = other.kind;
    other.kind = tmp;
    names.swap(other.names);
  }
};

// Stores the result of in-order resolution of use/import
// statements. This would not be separate from resolving variables
// if the language design was that symbols available due to use/import
// are only available after that statement (and in that case this analysis
// could fold into the logic about variable declarations).
struct ResolvedImportScope {
  const Scope* scope;
  std::vector<ImportedSymbols> imported;
  ResolvedImportScope(const Scope* scope)
    : scope(scope)
  { }
  bool operator==(const ResolvedImportScope& other) const {
    return scope == other.scope &&
           imported == other.imported;
  }
  bool operator!=(const ResolvedImportScope& other) const {
    return !(*this == other);
  }
};

/*
struct ContainedScopesAndScopedSymbols {
  std::unordered_map<ID, Scope> idToScope;
  std::unordered_map<ID, ID> scopeSymbolsToScopeIds;
};

using IdToScope = std::unordered_map<ID, Scope*>;
*/

/**
  An untyped function signature. This is really just the part of a function
  including the formals.
 */
struct UntypedFnSignature {
  UniqueString name;
  bool isMethod; // in that case, formals[0] is the receiver
  std::vector<const uast::Formal*> formals;
};

struct CallInfoActual {
  const types::Type* type;
  UniqueString byName;
};

struct CallInfo {
  bool isMethod; // in that case, actuals[0] is the receiver
  std::vector<CallInfoActual> actuals;
};

struct TypedFnSignatureFormal {
  const uast::Formal* formal;
  const types::Type* type;
  // TODO: param value
};

struct TypedFnSignature {
  UniqueString name;
  bool isMethod; // in that case, formals[0] is the receiver
  std::vector<TypedFnSignatureFormal> formals;
};

/*
struct ResolvedExpression {
  // the expr that is resolved
  const uast::Expression* expr = nullptr;
  // For simple cases, which named decl does it refer to?
  const uast::NamedDecl* decl = nullptr;
  // What is its type?
  const types::Type* type = nullptr;
  ResolutionResult() { }
  ResolutionResult(const uast::Expression* expr,
                   const uast::NamedDecl* decl,
                   const types::Type* type)
    : expr(expr), decl(decl), type(type) { }
};

struct MultiResolvedExpression : ResolvedExpression {
  // For a function call, it might refer to several Functions
  // and we might not know which return intent to choose yet.
  std::vector<const uast::Function*> candidates;

  // TODO:
  //  establishing types
  //  return-intent overloading
  //  generic instantiation
  //  establish concrete intents
  //  establish copy-init vs move
  MultiResolvedExpression() { }
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
*/
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
