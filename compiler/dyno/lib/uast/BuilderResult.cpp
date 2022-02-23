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

#include "chpl/uast/BuilderResult.h"

#include "chpl/queries/Context.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Comment.h"

#include <cstring>
#include <string>

namespace chpl {
namespace uast {

BuilderResult::BuilderResult()
  : filePath_()
{
}


BuilderResult::BuilderResult(UniqueString filePath)
  : filePath_(filePath)
{
}

// Computes idToAst and idToParent maps by visiting all uAST nodes
static
void computeIdMaps(
    const ASTNode* ast,
    const ASTNode* parentAst,
    std::unordered_map<ID, const ASTNode*>& idToAst,
    std::unordered_map<ID, ID>& idToParentId) {

  for (const ASTNode* child : ast->children()) {
    computeIdMaps(child, ast, idToAst, idToParentId);
  }

  if (!ast->id().isEmpty()) {
    idToAst[ast->id()] = ast;

    if (parentAst != nullptr) {
      if (!parentAst->id().isEmpty()) {
        idToParentId[ast->id()] = parentAst->id();
      } else {
        assert(false && "parentAst does not have valid ID");
      }
    }
  }
}

void BuilderResult::swap(BuilderResult& other) {
  filePath_.swap(other.filePath_);
  topLevelExpressions_.swap(other.topLevelExpressions_);
  errors_.swap(other.errors_);
  idToAst_.swap(other.idToAst_);
  idToLocation_.swap(other.idToLocation_);
  commentIdToLocation_.swap(other.commentIdToLocation_);
  idToParentId_.swap(other.idToParentId_);
  idToAst_.swap(other.idToAst_);
}

bool BuilderResult::update(BuilderResult& keep, BuilderResult& addin) {
  bool changed = false;

  // update the filePath
  changed |= defaultUpdate(keep.filePath_, addin.filePath_);

  // update the errors
  changed |= defaultUpdate(keep.errors_, addin.errors_);

  // update the ASTs
  changed |= updateASTList(keep.topLevelExpressions_,
                           addin.topLevelExpressions_);

  std::unordered_map<ID, const ASTNode*> newIdToAst;
  std::unordered_map<ID, ID> newIdToParent;

  // recompute locationsVec by traversing the AST and using the maps
  for (const auto& ast : keep.topLevelExpressions_) {
    computeIdMaps(ast.get(), nullptr, newIdToAst, newIdToParent);
  }

  // now update the ID and Locations maps in keep
  changed |= defaultUpdate(keep.idToAst_, newIdToAst);
  changed |= defaultUpdate(keep.idToParentId_, newIdToParent);
  changed |= defaultUpdate(keep.idToLocation_, addin.idToLocation_);
  changed |= defaultUpdate(keep.commentIdToLocation_, addin.commentIdToLocation_);

  return changed;
}

void BuilderResult::mark(Context* context) const {

  // mark the UniqueString file path
  filePath_.mark(context);

  // mark UniqueStrings in the ASTs
  markASTList(context, topLevelExpressions_);

  // NOTE: pair.first.mark(context) is redundant in each of these b/c any
  // ID (pair.first) will be marked by markASTList above

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

  for (const Location& loc : commentIdToLocation_) {
    loc.mark(context);
  }

  for (const auto& em : errors_) {
    em.mark(context);
  }

  // update the filePathForModuleName query
  BuilderResult::updateFilePaths(context, *this);
}

static void updateFilePathsForModulesRecursively(Context* context,
                                                 const ASTNode* ast,
                                                 UniqueString path) {
  if (const Module* mod = ast->toModule()) {
    context->setFilePathForModuleID(mod->id(), path);
  }

  for (const ASTNode* child : ast->children()) {
    updateFilePathsForModulesRecursively(context, child, path);
  }
}

void BuilderResult::updateFilePaths(Context* context,
                                    const BuilderResult& keep) {
  UniqueString path = keep.filePath_;
  // Update the filePathForModuleName query
  for (auto & expr : keep.topLevelExpressions_) {
    updateFilePathsForModulesRecursively(context, expr.get(), path);
  }
}

const ASTNode* BuilderResult::idToAst(ID id) const {
  const ASTNode* ast = nullptr;
  auto search = idToAst_.find(id);
  if (search != idToAst_.end()) {
    ast = search->second;
  }
  return ast;
}

Location BuilderResult::idToLocation(ID id, UniqueString path) const {
  // Look in astToLocation
  auto search = idToLocation_.find(id);
  if (search != idToLocation_.end()) {
    return search->second;
  }
  return Location(path);
}

Location BuilderResult::commentToLocation(const Comment *c) const {
  int idx = c->commentId().index();
  assert(idx >= 0 && "Cant lookup comment that has -1 id");
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


} // namespace uast
} // namespace chpl
