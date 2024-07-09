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

#ifndef CHPL_UAST_BUILDERRESULT_H
#define CHPL_UAST_BUILDERRESULT_H

#include "chpl/framework/UniqueString.h"
#include "chpl/framework/mark-functions.h"
#include "chpl/framework/update-functions.h"
#include "chpl/uast/AstNode.h"
#include "chpl/util/iteration.h"
#include "chpl/framework/stringify-functions.h"
#include "chpl/framework/global-strings.h"

#include <vector>
#include <unordered_map>
#include <utility>

#include "llvm/ADT/DenseMap.h"

// These macros make it easy for the Builder and BuilderResult types to
// manipulate their internal location maps without having to reconstruct
// their names each time with concatenation.

// Helper macro for concatenation.
#define CHPL_ID_LOC_MAP_INNER(ast__, location__) \
  id##ast__##To##location__##Location_

// Use to get the name of an additional <ID, Location> map.
#define CHPL_ID_LOC_MAP(ast__, location__) \
  CHPL_ID_LOC_MAP_INNER(ast__, location__)

// Helper macro for concatenation.
#define CHPL_AST_LOC_MAP_INNER(ast__, location__) \
  noted##ast__##To##location__##Location_

// Use to get the name of an additional <const AstNode*, Location> map.
#define CHPL_AST_LOC_MAP(ast__, location__) \
  CHPL_AST_LOC_MAP_INNER(ast__, location__)

namespace llvm {
  template<> struct DenseMapInfo<chpl::ID> {
    static bool isEqual(const chpl::ID& lhs, const chpl::ID& rhs) {
      return lhs == rhs;
    }

    static size_t getHashValue(const chpl::ID& id) {
      return chpl::hash(id);
    }

    static const chpl::ID getEmptyKey() {
      return chpl::ID(USTR("<empty>"));
    }

    static const chpl::ID getTombstoneKey() {
      return chpl::ID(USTR("<tombstone>"));
    }
  };
}


namespace chpl {

class Context;
class Location;

namespace libraries {
  class LibraryFile;
}

namespace uast {
  class BuilderResult;
}

namespace parsing {
  // forward declare for use in friend declaration
  const uast::BuilderResult& parseFile(Context* context, UniqueString path);
}

namespace uast {


/**
  This type records the result of building some AST.
 */
class BuilderResult final {
  friend class Builder;

 public:
  // enum defining integer indices for location map names
  // (for use with library files, primarily)
  enum struct LocationMapTag {
    BaseMap = 0,
    #define LOCATION_MAP(ast__, location__) \
      location__,
    #include "chpl/uast/all-location-maps.h"
    #undef LOCATION_MAP
  };

 private:
  UniqueString filePath_;
  AstList topLevelExpressions_;

  // Given an ID, what is the AstNode?
  llvm::DenseMap<ID, const AstNode*> idToAst_;

  // Given an ID, what is the parent ID?
  llvm::DenseMap<ID, ID> idToParentId_;

  // Goes from ID to Location, applies to all AST nodes except Comment
  llvm::DenseMap<ID, Location> idToLocation_;

  // Expand maps for locations of things that are not captured by AST.
  // The key is the ID of the relevant AST, and value is the location
  // of interest.
  #define LOCATION_MAP(ast__, location__) \
    llvm::DenseMap<ID, Location> CHPL_ID_LOC_MAP(ast__, location__);
  #include "all-location-maps.h"
  #undef LOCATION_MAP

  // Goes from Comment ID to Location (for comments, specifically)
  std::vector<Location> commentIdToLocation_;

  const libraries::LibraryFile* libraryFile_ = nullptr;
  // maps from ID to module number and symbol table entry number
  // but only for modules & symbols stored in the symbol table
  llvm::DenseMap<ID, std::pair<int,int>> libraryFileSymbols_;

  // For use with library files.
  // Returns the module index & symbol index for the symbol
  // containing the passed ID.
  // Returns 'true' if something was found.
  bool findContainingSymbol(ID id,
                            int& foundModuleIdx,
                            ID& foundSymbolId,
                            int& foundSymbolIdx) const;

  Location computeLocationFromLibraryFile(Context* context,
                                          ID id,
                                          UniqueString path,
                                          LocationMapTag tag) const;

 public:
  /** Construct an empty BuilderResult */
  BuilderResult();
  /** Construct a BuilderResult that records a particular file path,
      and optionally refers to a LibraryFile. */
  BuilderResult(UniqueString filePath,
                const libraries::LibraryFile* lib = nullptr);

  /** Return the file path this result refers to */
  UniqueString filePath() const {
    return filePath_;
  }

  /** return the number of top-level expressions */
  int numTopLevelExpressions() const {
    return topLevelExpressions_.size();
  }

  /** return the i'th top-level expression */
  const AstNode* topLevelExpression(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < topLevelExpressions_.size());
    return topLevelExpressions_[i].get();
  }

  /** iterate over the parsed top-level expressions */
  AstListIteratorPair<AstNode> topLevelExpressions() const {
    return AstListIteratorPair<AstNode>(topLevelExpressions_.begin(),
                                        topLevelExpressions_.end());
  }

  /** If the top-level expressions contain only a single Module,
     return it. Otherwise, return nullptr. */
  const Module* singleModule() const {
    if (topLevelExpressions_.size() == 1) {
      return topLevelExpressions_[0]->toModule();
    }

    return nullptr;
  }

  /** Find the AstNode* corresponding to a particular ID, or return
      nullptr if there is not one in this result. */
  const AstNode* idToAst(ID id) const;
  /** Find the Location for a particular ID.
      Returns a location just to path if none is found. */
  Location idToLocation(Context* context, ID id, UniqueString path) const;

  /** Find the Location for a particular comment.
      The Comment must have been from this BuilderResult, but this is not
      checked.
      An empty Location will be returned if the Comment couldn't be found
  */
  Location commentToLocation(const Comment *comment) const;
  /** Find the ID for a parent given an ID.
      Returns the empty ID if none is found */
  ID idToParentId(ID id) const;

  /** Find an additional location given ID input. Returns an empty location
      pointing to 'path' if none was found. */
  #define LOCATION_MAP(ast__, location__) \
    Location idTo##location__##Location(Context* context, ID id, UniqueString path) const;
  #include "all-location-maps.h"
  #undef LOCATION_MAP

  /** Returns 'true' if this BuilderResult is using a LibraryFile and
      the passed ID represents a symbol table symbol in the LibraryFile */
  bool isSymbolTableSymbol(ID id) const;

  BuilderResult(BuilderResult&&) = default; // move-constructable
  BuilderResult(const BuilderResult&) = delete; // not copy-constructable
  BuilderResult& operator=(const BuilderResult&) = delete; // not assignable

  void swap(BuilderResult& other);

  static bool update(BuilderResult& keep, BuilderResult& addin);
  void mark(Context* context) const;
  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  // these two should only be called by the parser
  static void updateFilePaths(Context* context, const BuilderResult& keep);
  bool equals(const BuilderResult& other) const;
};


} // end namespace uast



} // end namespace chpl


#endif
