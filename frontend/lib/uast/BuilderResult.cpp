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

#include "chpl/uast/BuilderResult.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/mark-functions.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Comment.h"
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


BuilderResult::BuilderResult(UniqueString filePath)
  : filePath_(filePath)
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

void BuilderResult::swap(BuilderResult& other) {
  filePath_.swap(other.filePath_);
  topLevelExpressions_.swap(other.topLevelExpressions_);
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
  changed |= defaultUpdate(keep.commentIdToLocation_, addin.commentIdToLocation_);

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

  for (const Location& loc : commentIdToLocation_) {
    loc.mark(context);
  }

  // update the filePathForModuleName query
  BuilderResult::updateFilePaths(context, *this);
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

#define DYNO_BUILDER_RESULT_START_STR std::string("DYNO_BUILDER_RESULT_START")
#define DYNO_BUILDER_RESULT_END_STR std::string("DYNO_BUILDER_RESULT_END")

void BuilderResult::serialize(std::ostream& os) const {
  Serializer ser(os);
  serialize(ser);
}

// BuilderResult serialization format (whitespace not significant):
// <start sentinel string, std::string>
// <file path, std::string>
// N: <number of top-level uAST expressions, uint32_t>
//   0..N-1: <top level expression and its children, AstNode>
// <id-to-parent-id, llvm::DenseMap<ID, ID>>
// M: <number of id-to-location entries, uint64_t>
// <file path for locations, std::string>
//   0..M-1:
//     <id as key, ID>
//     <first line, int>
//     <first column, int>
//     <last line, int>
//     <last column, int>
// <comment-id-to-location, std::vector<Location>>
// <end sentinel string, std::string>
void BuilderResult::serialize(Serializer& ser) const {
  ser.write(DYNO_BUILDER_RESULT_START_STR);
  ser.write(filePath_);
  const uint32_t numEntries = numTopLevelExpressions();
  ser.write(numEntries);

  for (auto ast : topLevelExpressions()) {
    ast->serialize(ser);
  }

  ser.write(idToParentId_);

  {
    // TODO: For config-vars set on the command line, the 'file path' is set to
    // something different. This leads to issues when comparing against a
    // deserialized BuilderResult.
    ser.write((uint64_t)idToLocation_.size());
    ser.write(filePath_);
    for (const auto& pair : idToLocation_) {
      ser.write(pair.first);
      ser.write(pair.second.firstLine());
      ser.write(pair.second.firstColumn());
      ser.write(pair.second.lastLine());
      ser.write(pair.second.lastColumn());
    }
  }

  ser.write(commentIdToLocation_);
  ser.write(DYNO_BUILDER_RESULT_END_STR);
}

static void assignIDsFromTree(llvm::DenseMap<ID, const AstNode*>& idToAst,
                              const AstNode* node) {
  if (node->isComment()) return;

  idToAst[node->id()] = node;
  for (auto child : node->children()) {
    assignIDsFromTree(idToAst, child);
  }
}

BuilderResult BuilderResult::deserialize(Deserializer& des) {
  BuilderResult ret;
  AstList alist;

  CHPL_ASSERT(DYNO_BUILDER_RESULT_START_STR == des.read<std::string>());

  auto path = des.read<UniqueString>(); // path

  const auto numEntries = des.read<uint32_t>();

  // TODO: for improved performance, try recomputing the IDs rather than
  // serializing and deserializing them. If we recompute these IDs then we
  // also don't need to store the 'idToParent' map.
  for (uint32_t i = 0; i < numEntries; i++) {
    alist.push_back(AstNode::deserialize(des));
  }

  auto idToParent = des.read<llvm::DenseMap<ID,ID>>();

  // TODO: For performance and reduced file-size, try variable-byte encoding.
  // TODO: For performance, could try not loading this data until it's needed.
  auto maplen = des.read<uint64_t>();
  llvm::DenseMap<ID,Location> idToLocation(maplen);
  auto pathstr = des.read<UniqueString>();
  for (uint64_t i = 0; i < maplen; i++) {
    auto curid = des.read<ID>();
    int fl = des.read<int>();
    int fc = des.read<int>();
    int ll = des.read<int>();
    int lc = des.read<int>();
    idToLocation.insert({curid, Location(pathstr, fl, fc, ll, lc)});
  }

  auto commentLocation = des.read<std::vector<Location>>();

  CHPL_ASSERT(DYNO_BUILDER_RESULT_END_STR == des.read<std::string>());

  // Build the 'idToAst' map with the IDs stored in uAST nodes.
  llvm::DenseMap<ID, const AstNode*> idToAst;
  for (auto& node : alist) {
    AstNode* ptr = node.get();
    assignIDsFromTree(idToAst, ptr);
  }

  // swap everything into the result
  std::swap(ret.filePath_, path);
  std::swap(ret.topLevelExpressions_, alist);
  std::swap(ret.idToAst_, idToAst);
  std::swap(ret.idToParentId_, idToParent);
  std::swap(ret.idToLocation_, idToLocation);
  std::swap(ret.commentIdToLocation_, commentLocation);

  BuilderResult::updateFilePaths(des.context(), ret);

  return ret;
}

bool BuilderResult::compare(const BuilderResult& other) const {
  if (idToParentId_ != other.idToParentId_) {
    return false;
  }
  if (idToLocation_ != other.idToLocation_) {
    return false;
  }
  if (commentIdToLocation_ != other.commentIdToLocation_) {
    return false;
  }

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

  return true;
}


} // namespace uast
} // namespace chpl
