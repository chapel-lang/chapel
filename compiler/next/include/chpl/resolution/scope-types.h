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
// It stores a normalized form of the symbols made available
// by a use/import clause.
struct VisibilitySymbols {
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

  VisibilitySymbols() { }
  VisibilitySymbols(ID symbolId, Kind kind, bool isPrivate,
                    std::vector<std::pair<UniqueString,UniqueString>> names)
    : symbolId(symbolId), kind(kind), isPrivate(isPrivate),
      names(std::move(names))
  { }


  bool operator==(const VisibilitySymbols& other) const {
    return symbolId == other.symbolId &&
           kind == other.kind &&
           names == other.names;
  }
  bool operator!=(const VisibilitySymbols& other) const {
    return !(*this == other);
  }

  void swap(VisibilitySymbols& other) {
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
struct ResolvedVisibilityScope {
  const Scope* scope;
  std::vector<VisibilitySymbols> visibilityClauses;
  ResolvedVisibilityScope(const Scope* scope)
    : scope(scope)
  { }
  bool operator==(const ResolvedVisibilityScope& other) const {
    return scope == other.scope &&
           visibilityClauses == other.visibilityClauses;
  }
  bool operator!=(const ResolvedVisibilityScope& other) const {
    return !(*this == other);
  }
};


} // end namespace resolution
} // end namespace chpl
#endif
