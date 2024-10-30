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

#include "chpl/uast/BuilderResult.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/mark-functions.h"
#include "chpl/libraries/LibraryFile.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Module.h"
#include "chpl/util/filesystem.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

namespace chpl {
namespace uast {

BuilderResult::BuilderResult()
  : filePath_()
{
}


BuilderResult::BuilderResult(UniqueString filePath,
                             const libraries::LibraryFile* lib)
  : filePath_(filePath), libraryFile_(lib)
{
}

// Computes idToAst and idToParent maps by visiting all uAST nodes
static
void computeIdMaps(
    const AstNode* ast,
    const AstNode* parentAst,
    llvm::DenseMap<ID, const AstNode*>& idToAst,
    llvm::DenseMap<ID, ID>& idToParentId) {

  for (const AstNode* child : ast->children()) {
    computeIdMaps(child, ast, idToAst, idToParentId);
  }

  if (!ast->id().isEmpty()) {
    idToAst[ast->id()] = ast;

    if (parentAst != nullptr) {
      if (!parentAst->id().isEmpty()) {
        idToParentId[ast->id()] = parentAst->id();
      } else {
        CHPL_ASSERT(false && "parentAst does not have valid ID");
      }
    }
  }
}

bool BuilderResult::isSymbolTableSymbol(ID id) const {
  return libraryFileSymbols_.count(id) > 0;
}

void BuilderResult::swap(BuilderResult& other) {
  filePath_.swap(other.filePath_);
  topLevelExpressions_.swap(other.topLevelExpressions_);
  idToAst_.swap(other.idToAst_);
  idToParentId_.swap(other.idToParentId_);
  idToLocation_.swap(other.idToLocation_);

  // Swap additional location maps.
  #define LOCATION_MAP(ast__, location__) { \
    auto& m1 = CHPL_ID_LOC_MAP(ast__, location__); \
    auto& m2 = other.CHPL_ID_LOC_MAP(ast__, location__); \
    m1.swap(m2); \
  }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP

  commentIdToLocation_.swap(other.commentIdToLocation_);

  std::swap(libraryFile_, other.libraryFile_);
  libraryFileSymbols_.swap(other.libraryFileSymbols_);
}

bool BuilderResult::update(BuilderResult& keep, BuilderResult& addin) {
  bool changed = false;

  // update the filePath
  changed |= defaultUpdate(keep.filePath_, addin.filePath_);

  // update the ASTs
  changed |= updateAstList(keep.topLevelExpressions_,
                           addin.topLevelExpressions_);

  llvm::DenseMap<ID, const AstNode*> newIdToAst;
  llvm::DenseMap<ID, ID> newIdToParent;

  // recompute locationsVec by traversing the AST and using the maps
  for (const auto& ast : keep.topLevelExpressions_) {
    computeIdMaps(ast.get(), nullptr, newIdToAst, newIdToParent);
  }

  // now update the ID and Locations maps in keep
  changed |= defaultUpdate(keep.idToAst_, newIdToAst);
  changed |= defaultUpdate(keep.idToParentId_, newIdToParent);
  changed |= defaultUpdate(keep.idToLocation_, addin.idToLocation_);

  // Also update additional location maps.
  #define LOCATION_MAP(ast__, location__) { \
    auto& m1 = keep.CHPL_ID_LOC_MAP(ast__, location__); \
    auto& m2 = addin.CHPL_ID_LOC_MAP(ast__, location__); \
    changed |= defaultUpdate(m1, m2); \
  }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP

  changed |= defaultUpdate(keep.commentIdToLocation_,
                           addin.commentIdToLocation_);
  changed |= defaultUpdateBasic(keep.libraryFile_, addin.libraryFile_);
  changed |= defaultUpdate(keep.libraryFileSymbols_, addin.libraryFileSymbols_);

  return changed;
}

void BuilderResult::mark(Context* context) const {

  // mark the UniqueString file path
  filePath_.mark(context);

  // mark UniqueStrings in the ASTs
  markAstList(context, topLevelExpressions_);

  // NOTE: pair.first.mark(context) is redundant in each of these b/c any
  // ID (pair.first) will be marked by markAstList above

  // mark UniqueStrings in the Locations
  for (const auto& pair : idToLocation_) {
    //pair.first.mark(context); // redundant
    pair.second.mark(context);
  }

  for (const auto& pair : idToParentId_) {
    // pair.first.mark(context); // redundant
    pair.second.mark(context);
  }

  for (const auto& pair : idToAst_) {
    // pair.first.mark(context); // redundant
    context->markPointer(pair.second);
  }

  // Also mark locations in the additional location maps. No need to mark
  // IDs since they should already be marked as explained above.
  #define LOCATION_MAP(ast__, location__) { \
    auto& m = CHPL_ID_LOC_MAP(ast__, location__); \
    for (const auto& p : m) p.second.mark(context); \
  }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP

  for (const Location& loc : commentIdToLocation_) {
    loc.mark(context);
  }

  context->markPointer(libraryFile_);

  // libraryFileSymbols_ only has IDs that should have been marked above
  (void) libraryFileSymbols_;

  // update the filePathForModuleName query
  BuilderResult::updateFilePaths(context, *this);
}

void BuilderResult::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  ss << "BuilderResult(...)";
}

void BuilderResult::updateFilePaths(Context* context,
                                    const BuilderResult& keep) {
  UniqueString path = keep.filePath_;
  // Update the filePathForModuleName query
  for (auto & ast : keep.topLevelExpressions_) {
    if (const Module* mod = ast->toModule()) {
      context->setFilePathForModuleId(mod->id(), path);
    }
  }
}

const AstNode* BuilderResult::idToAst(ID id) const {
  const AstNode* ast = nullptr;
  auto search = idToAst_.find(id);
  if (search != idToAst_.end()) {
    ast = search->second;
  }
  return ast;
}

bool BuilderResult::findContainingSymbol(ID id,
                                         int& foundModuleIdx,
                                         ID& foundSymbolId,
                                         int& foundSymbolIdx) const {
  ID cur = id;
  while (!cur.isEmpty()) {
    // check to see if the ID corresponds to a symbol table symbol
    {
      auto search1 = libraryFileSymbols_.find(cur);
      if (search1 != libraryFileSymbols_.end()) {
        // return the symbol ID symbol information that we found
        foundSymbolId = cur;
        foundModuleIdx = search1->second.first;
        foundSymbolIdx = search1->second.second;
        return true;
      }
    }

    // If not, find the parent ID and return failure if no parent ID is here
    {
      auto search2 = idToParentId_.find(cur);
      if (search2 != idToParentId_.end()) {
        cur = search2->second;
      } else {
        return false;
      }
    }
  }

  return false;
}

Location BuilderResult::computeLocationFromLibraryFile(Context* context,
                                                       ID id,
                                                       UniqueString path,
                                                       LocationMapTag tag) const
{
  CHPL_ASSERT(libraryFile_ != nullptr);
  if (libraryFile_ == nullptr) {
    return Location(path);
  }

  int inModuleIdx = -1;
  ID inSymbolId;
  int inSymbolIdx = -1;
  bool found = findContainingSymbol(id, inModuleIdx, inSymbolId, inSymbolIdx);

  // return early if 'id' does not seem to be contained in here
  if (!found || inSymbolIdx < 0 || inModuleIdx < 0 || inSymbolId.isEmpty()) {
    return Location(path);
  }

  // look up the AstNode* for the containing symbol
  const AstNode* symbolAst = idToAst(inSymbolId);
  if (symbolAst == nullptr) {
    return Location(path);
  }

  // look up the AstNode* for the requested id
  const AstNode* requestedAst = idToAst(id);
  if (requestedAst == nullptr) {
    return Location(path);
  }

  // read the appropriate Locations from the library file and get
  // the appropriate Location from it
  const auto& maps = libraryFile_->loadLocations(context, inModuleIdx,
                                                 inSymbolIdx, symbolAst);

  const auto* map = maps.getLocationMap((int)tag);
  if (map != nullptr) {
    auto search = map->find(requestedAst);
    if (search != map->end()) {
      return search->second;
    }
  }

  return Location(path);
}

Location BuilderResult::idToLocation(Context* context,
                                     ID id,
                                     UniqueString path) const {
  if (libraryFile_) {
    return computeLocationFromLibraryFile(context, id, path,
                                          LocationMapTag::BaseMap);
  }

  // Look in astToLocation
  auto search = idToLocation_.find(id);
  if (search != idToLocation_.end()) {
    return search->second;
  }
  return Location(path);
}

Location BuilderResult::commentToLocation(const Comment *c) const {
  if (libraryFile_) {
    // library files don't store comments
    CHPL_ASSERT(false && "should not be reachable");
  }

  int idx = c->commentId().index();
  CHPL_ASSERT(idx >= 0 && "Cant lookup comment that has -1 id");
  if (idx < 0 || (size_t)idx >= commentIdToLocation_.size()) {
    return Location();
  }
  return commentIdToLocation_[idx];
}

ID BuilderResult::idToParentId(ID id) const {
  auto search = idToParentId_.find(id);
  if (search != idToParentId_.end()) {
    return search->second;
  }
  return ID();
}


// Add getters for additional locations that go from AST or ID to location.
#define LOCATION_MAP(ast__, location__) \
  Location BuilderResult:: \
  idTo##location__##Location(Context* context, ID id, UniqueString path) const { \
    if (!id) return Location(path); \
    if (libraryFile_ != nullptr) { \
      return computeLocationFromLibraryFile(context, id, path, \
                                            LocationMapTag::location__); \
    } \
    auto& m = CHPL_ID_LOC_MAP(ast__, location__); \
    auto it = m.find(id); \
    return (it != m.end()) ? it->second : Location(path); \
  }
#include "chpl/uast/all-location-maps.h"
#undef LOCATION_MAP

bool BuilderResult::equals(const BuilderResult& other) const {
  if (idToParentId_ != other.idToParentId_) {
    return false;
  }
  if (idToLocation_ != other.idToLocation_) {
    return false;
  }
  if (commentIdToLocation_ != other.commentIdToLocation_) {
    return false;
  }

  if ((libraryFile_ != nullptr) != (other.libraryFile_ != nullptr)) {
    return false;
  }

  if (libraryFile_ && other.libraryFile_) {
    // check if the hashes are the same
    if (*libraryFile_ != *other.libraryFile_) {
      return false;
    }
  }

  /* should be unnecessary to check libraryFileSymbols_
     because if the library file changed,
     the above check would return false. */
  CHPL_ASSERT(libraryFileSymbols_ == other.libraryFileSymbols_);

  auto& alist = other.topLevelExpressions_;
  const int n = numTopLevelExpressions();
  if (alist.size() != (size_t)n) {
    return false;
  }

  for (int i = 0; i < n; i++) {
    if (alist[i] == nullptr) {
      return false;
    }
    if (alist[i]->completeMatch(topLevelExpression(i)) == false) {
      return false;
    }
  }

  // Check that all the additional location maps match.
  #define LOCATION_MAP(ast__, location__) { \
      auto& m1 = CHPL_ID_LOC_MAP(ast__, location__); \
      auto& m2 = other.CHPL_ID_LOC_MAP(ast__, location__); \
      if (m1 != m2) return false; \
    }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP

  return true;
}


} // namespace uast
} // namespace chpl
