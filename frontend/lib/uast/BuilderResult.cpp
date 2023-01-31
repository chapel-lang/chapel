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


// <7F>HPECHPL
const uint64_t magic = 0x4C5048434550487F;
const uint32_t version = 0x00000001;

std::string BuilderResult::serialize(const char* dn) const {
  std::string dirName = dn;
  chpl::makeDir(dirName, true);
  std::string baseName;
  {
    auto copy = this->filePath().str();
    const char* fname = copy.c_str();
    const char* right = strrchr(fname, '/');
    if (right == NULL) {
      baseName = fname;
    } else {
      baseName = right + 1;
    }
  }
  auto fileName = dirName + "/" + baseName + ".ast.bin";
  std::ofstream myFile;
  myFile.open(fileName, std::ios::out | std::ios::trunc | std::ios::binary);

  serialize(myFile);

  return fileName;
}

void BuilderResult::serialize(std::ostream& os) const {
  Serializer ser(os);
  ser.write(magic);
  ser.write(version);
  const uint32_t numEntries = numTopLevelExpressions();
  ser.write(numEntries);

  for (auto ast : topLevelExpressions()) {
    ast->serialize(ser);
  }
}

// TODO: handle Locations
AstList BuilderResult::deserialize(Context* context, std::string& sfname) {
  std::ifstream myFile;
  myFile.open(sfname, std::ios::in | std::ios::binary);

  return deserialize(context, myFile);
}

AstList BuilderResult::deserialize(Context* context, std::istream& is) {
  AstList ret;
  Deserializer des(context, is);

  auto m = des.read<uint64_t>();
  auto v = des.read<uint32_t>();
  (void)m; // silence unused variable warnings
  (void)v; // silence unused variable warnings
  assert(m == magic);
  assert(v == version);

  const auto numEntries = des.read<uint32_t>();

  for (uint32_t i = 0; i < numEntries; i++) {
    ret.push_back(AstNode::deserialize(des));
  }

  is.peek();
  assert(is.eof());

  return ret;
}

bool BuilderResult::compare(const AstList& other) const {
  const int n = numTopLevelExpressions();
  if (other.size() != (size_t)n) {
    return false;
  }
  for (int i = 0; i < n; i++) {
    if (other[i] == nullptr) {
      return false;
    }
    if (other[i]->completeMatch(topLevelExpression(i)) == false) {
      return false;
    }
  }
  return true;
}


} // namespace uast
} // namespace chpl
