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
#include "chpl/uast/Expression.h"
#include "chpl/uast/Module.h"

#include <cstring>
#include <string>

namespace chpl {
namespace uast {


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
  auto uniqueFilename = UniqueString::build(context, filepath);
  auto b = new Builder(context, uniqueFilename);
  return toOwned(b);
}

void Builder::addToplevelExpression(owned<Expression> e) {
  this->topLevelExpressions_.push_back(std::move(e));
}

void Builder::addError(ErrorMessage e) {
  this->errors_.push_back(std::move(e));
}

void Builder::noteLocation(ASTNode* ast, Location loc) {
  notedLocations_[ast] = loc;
}

BuilderResult Builder::result() {
  this->createImplicitModuleIfNeeded();
  this->assignIDs();

  // Performance: We could consider copying all of these AST
  // nodes to a newly allocated buffer big enough to hold them
  // all contiguously. The reason to do so would be to ensure
  // that a postorder traversal of the AST has good data locality
  // (i.e. good cache behavior).

  BuilderResult ret(filepath_);
  ret.topLevelExpressions_.swap(topLevelExpressions_);
  ret.errors_.swap(errors_);
  ret.idToAst_.swap(idToAst_);
  ret.astToLocation_.swap(astToLocation_);

  return ret;
}

bool Builder::astTagIndicatesNewIdScope(asttags::ASTTag tag) {
  return asttags::isNamedDecl(tag) &&
        (asttags::isFunction(tag) ||
         asttags::isModule(tag) ||
         asttags::isTypeDecl(tag));
}

// If the implicit module is needed, moves the statements in to it.
void Builder::createImplicitModuleIfNeeded() {
  bool containsOnlyModules = true;
  bool containsAnyModules = false;
  for (auto const& ownedExpression: topLevelExpressions_) {
    if (ownedExpression->isComment()) {
      // ignore comments for this analysis
    } else if (ownedExpression->isModule()) {
      containsAnyModules = true;
    } else {
      containsOnlyModules = false;
    }
  }
  if (containsAnyModules && containsOnlyModules) {
    // no inferred module is needed.
    return;
  } else {
    // compute the basename of filename to get the inferred module name
    std::string modname = filenameToModulename(filepath_.c_str());
    auto inferredModuleName = UniqueString::build(context_, modname);
    // create a new module containing all of the statements
    ASTList stmts;
    stmts.swap(topLevelExpressions_);
    auto implicitModule = Module::build(this, Location(filepath_),
                                        inferredModuleName,
                                        Decl::DEFAULT_VISIBILITY,
                                        Module::IMPLICIT,
                                        std::move(stmts));
    topLevelExpressions_.push_back(std::move(implicitModule));
  }
}

void Builder::assignIDs() {
  pathVecT pathVec;
  declaredHereT duplicates;
  int i = 0;

  for (auto const& ownedExpression: topLevelExpressions_) {
    ASTNode* ast = ownedExpression.get();
    if (ast->isModule() || ast->isComment()) {
      UniqueString emptyString;
      doAssignIDs(ast, emptyString, i, pathVec, duplicates);
    } else {
      assert(false && "topLevelExpressions should only be module decls or comments");
    }
  }
}

/* A note about ID assignment

  This ID assigment tries to balance several competing goals:
   * would like postorder Ids to be available to make it easy to store e.g.
     resolution results for a function in a vector
   * would like incremental recompilation to minimize recomputation if code is
     added -- in particular this means that for say a function we don't want
     that function's ID to include the postOrderId in the parent scope

  The ID assignment uses the strategy of having functions, type decls, and
  modules create a new ID scope (with a new postOrderId counter). These uAST
  nodes have an ID based upon the path to that symbol and have a postOrderId
  that is just after the last element contained within.

  When printing IDs we use the notation of putting the symbolPath
  part first and then '@' and then the postOrderId. We leave out the
  @ and postOrderId when the postOrderId is -1. That is the case
  with AST nodes that create a new scope (Modules, Functions, Type decls).

  For example:

  M         module M {
  M.Inner     module Inner {
  M.Inner@0     a;
  M.Inner@1     b;
  M.Inner@2     c;
              }
  M@0         x;
            }

  Comments are not included in ID assignment.
  That means that comments don't have IDs and as a result it's not
  possible to go from a Comment to the file. We think this is acceptable
  because a documentation tool processing Comments can work with the
  parse result and make its own tables of these things.
 */
void Builder::doAssignIDs(ASTNode* ast, UniqueString symbolPath, int& i,
                          pathVecT& pathVec, declaredHereT& duplicates) {

  // update locations_ for the visited ast
  auto search = notedLocations_.find(ast);
  if (search != notedLocations_.end()) {
    assert(!search->second.isEmpty());
    astToLocation_[search->first] = search->second;
  } else {
    assert(false && "Location for all ast should be set by noteLocation");
  }

  if (ast->isComment()) {
    // comments don't have IDs
    return;
  }

  int firstChildID = i;

  bool newScope = Builder::astTagIndicatesNewIdScope(ast->tag());

  if (newScope) {
    // for scoping constructs, adjust the symbolPath and
    // then visit the defined symbol
    UniqueString name = ast->toNamedDecl()->name();
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

    // compute the string representing the path
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
    auto newSymbolPath = UniqueString::build(this->context(), pathStr);

    // get a fresh postorder traversal counter and duplicates map
    int freshId = 0;
    declaredHereT freshMap;
    for (auto & child : ast->children_) {
      ASTNode* ptr = child.get();
      this->doAssignIDs(ptr, newSymbolPath, freshId, pathVec, freshMap);
    }

    int numContainedIds = freshId;
    ast->setID(ID(newSymbolPath, -1, numContainedIds));

    // Note: when creating a new symbol (e.g. fn), we're not incrementing i.
    // The new symbol ID has the updated path (e.g. function name)
    // and other IDs in the parent scope don't consider the position
    // of this function.

    // pop the path component we just added
    pathVec.pop_back();

  } else {
    // not a new scope

    // visit the children now to get integer part of ids in postorder
    for (auto & child : ast->children_) {
      ASTNode* ptr = child.get();
      this->doAssignIDs(ptr, symbolPath, i, pathVec, duplicates);
    }

    int afterChildID = i;
    int myID = afterChildID;
    i++; // count the ID for the node we are currently visiting
    int numContainedIDs = afterChildID - firstChildID;
    ast->setID(ID(symbolPath, myID, numContainedIDs));
  }

  // update idToAst_ for the visited AST node
  idToAst_[ast->id()] = ast;
}

ASTList Builder::flattenTopLevelBlocks(ASTList lst) {
  ASTList ret;

  for (auto& ast : lst) {
    if (ast->isBlock()) {
      for (auto& child : takeChildren(std::move(ast))) {
        ret.push_back(std::move(child));
      }
    } else {
      ret.push_back(std::move(ast));
    }
  }

  lst.clear();

  return ret;
}


} // namespace uast
} // namespace chpl
