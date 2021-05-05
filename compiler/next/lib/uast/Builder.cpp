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

#include "chpl/uast/Builder.h"

#include "chpl/queries/Context.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/uast/Exp.h"
#include "chpl/uast/ModuleDecl.h"

#include <cstring>
#include <string>

namespace chpl {
namespace uast {


Builder::Builder(Context* context,
                 UniqueString filepath,
                 UniqueString inferredModuleName)
  : context_(context),
    filepath_(filepath),
    inferredModuleName_(inferredModuleName),
    topLevelExps_(), errors_(), locations_() {
}

static std::string filenameToModulename(const char* filename) {
  const char* moduleName = filename;
  const char* firstSlash = strrchr(moduleName, '/');

  if (firstSlash) {
    moduleName = firstSlash + 1;
  }

  const char* firstPeriod = strrchr(moduleName, '.');
  if (firstPeriod) {
    return std::string(moduleName, firstPeriod-moduleName);
  } else {
    return std::string(moduleName);
  }
}

owned<Builder> Builder::build(Context* context, const char* filepath) {
  // compute the basename of filename to get the inferred module name
  std::string modname = filenameToModulename(filepath);
  auto uniqueModname = UniqueString::build(context, modname);
  auto uniqueFilename = UniqueString::build(context, filepath);
  auto b = new Builder(context, uniqueFilename, uniqueModname);
  return toOwned(b);
}

void Builder::addToplevelExp(owned<Exp> e) {
  this->topLevelExps_.push_back(std::move(e));
}

void Builder::addError(ErrorMessage e) {
  this->errors_.push_back(std::move(e));
}

void Builder::noteLocation(ASTBase* ast, Location loc) {
  this->locations_.push_back(std::make_pair(ast, loc));
}

Builder::Result Builder::result() {
  UniqueString inferredName = this->createImplicitModuleIfNeeded();
  this->assignIDs(inferredName);

  // Performance: We could consider copying all of these AST
  // nodes to a newly allocated buffer big enough to hold them
  // all contiguously. The reason to do so would be to ensure
  // that a postorder traversal of the AST has good data locality
  // (i.e. good cache behavior).

  Builder::Result ret;
  ret.filePath = filepath_;
  ret.topLevelExps.swap(topLevelExps_);
  ret.errors.swap(errors_);
  ret.locations.swap(locations_);
  return std::move(ret);
}

// Returns the name of the implicit module, or "" if there is none
// If the implicit module is needed, moves the statements in to it.
UniqueString Builder::createImplicitModuleIfNeeded() {
  bool containsOnlyModules = true;
  bool containsAnyModules = false;
  for (auto const& ownedExp: topLevelExps_) {
    if (ownedExp->isModuleDecl()) {
      containsAnyModules = true;
    } else {
      containsOnlyModules = false;
    }
  }
  if (containsAnyModules && containsOnlyModules) {
    UniqueString empty;
    return empty;
  } else {
    // create a new module containing all of the statements
    ASTList stmts;
    stmts.swap(topLevelExps_);
    auto implicitModule = ModuleDecl::build(this, Location(filepath_),
                                            inferredModuleName_,
                                            Sym::VISIBILITY_DEFAULT,
                                            Module::IMPLICIT,
                                            std::move(stmts));
    topLevelExps_.push_back(std::move(implicitModule));
    // return the name of the module
    return inferredModuleName_;
  }
}

void Builder::assignIDs(UniqueString inferredModule) {
  pathVecT pathVec;
  declaredHereT duplicates;
  int i = 0;

  for (auto const& ownedExp: topLevelExps_) {
    if (ModuleDecl* moduleDecl = ownedExp->toModuleDecl()) {
      UniqueString emptyString;
      doAssignIDs(moduleDecl, emptyString, i, pathVec, duplicates);
    } else {
      assert(false && "topLevelExprs should only be module decls");
    }
  }
}

void Builder::doAssignIDs(ASTBase* ast, UniqueString symbolPath, int& i,
                          pathVecT& pathVec, declaredHereT& duplicates) {
  // It is appealing not to consider comments when computing AST ids,
  // but if that happens then we can't figure out source line numbers
  // for comments, which would be an issue in some documentation use cases.
  //
  // However we could make this be a flag on the Context - whether or not
  // comments can be ignored entirely.

  int firstChildID = i;

  Decl* decl = ast->toDecl();

  if (decl == nullptr) {
    // visit the children now to get integer part of ids in postorder
    for (auto & child : ast->children_) {
      ASTBase* ptr = child.get();
      this->doAssignIDs(ptr, symbolPath, i, pathVec, duplicates);
    }
  }
  int afterChildID = i;
  int myID = afterChildID;
  i++; // count the ID for the node we are currently visiting
  int numContainedIDs = afterChildID - firstChildID;
  ast->setID(ID(symbolPath, myID, numContainedIDs));

  // for decls, adjust the symbolPath and then visit the defined symbol
  if (decl != nullptr) {
    assert(decl->numChildren() == 1);
    assert(decl->child(0)->isSym());
    Sym* sym = (Sym*) decl->child(0);
    UniqueString name = sym->name();
    int repeat = 0;

    auto search = duplicates.find(name);
    if (search != duplicates.end()) {
      // it's already there, so increment the repeat counter
      repeat = search->second;
      repeat++;
      search->second = repeat;
    } else {
      duplicates.insert(search, std::make_pair(name, 0));
    }

    // push the path component
    pathVec.push_back(std::make_pair(name, repeat));

    std::string pathStr;
    bool first = true;
    for (const auto& p : pathVec) {
      UniqueString name = p.first;
      int repeat = p.second;
      if (first == false)
        pathStr += ".";
      first = false;
      pathStr += name.c_str();
      if (repeat != 0) {
        pathStr += "#";
        pathStr += std::to_string(repeat);
      }
    }
    auto symbolPath = UniqueString::build(this->context(), pathStr);
    // get a fresh postorder traversal counter and duplicates map
    int freshId = 0;
    declaredHereT freshMap;
    doAssignIDs(sym, symbolPath, freshId, pathVec, freshMap);
    // pop the path component we just added
    pathVec.pop_back();
  }
}

Builder::Result::Result()
  : filePath(), topLevelExps(), errors(), locations()
{
}

bool Builder::Result::update(Result& keep, Result& addin) {
  bool changed = false;

  // update the filePath
  changed |= defaultUpdate(keep.filePath, addin.filePath);

  // update the errors and locations
  changed |= defaultUpdate(keep.errors, addin.errors);
  changed |= defaultUpdate(keep.locations, addin.locations);

  // update the ASTs
  changed |= updateASTList(keep.topLevelExps, addin.topLevelExps);

  return changed;
}

void Builder::Result::mark(Context* context, const Result& keep) {

  // mark the UniqueString file path
  keep.filePath.mark(context);

  // mark UniqueStrings in the locations
  for (const auto& pair : keep.locations) {
    pair.second.markUniqueStrings(context);
  }

  // mark UniqueStrings in the ASTs
  markASTList(context, keep.topLevelExps);

  // update the filePathForModuleName query
  Builder::Result::updateFilePaths(context, keep);
}

void Builder::Result::updateFilePaths(Context* context, const Result& keep) {
  UniqueString path = keep.filePath;
  // Update the filePathForModuleName query
  for (auto & topLevelExp : keep.topLevelExps) {
    if (ModuleDecl* moduleDecl = topLevelExp->toModuleDecl()) {
      UniqueString moduleName = moduleDecl->name();
      context->setFilePathForModuleName(moduleName, path);
    } else {
      assert(false && "topLevelExprs should only be module decls");
    }
  }
}


} // namespace uast
} // namespace chpl
