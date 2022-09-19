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

#ifndef CHPL_UAST_BUILDER_H
#define CHPL_UAST_BUILDER_H

#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/framework/mark-functions.h"
#include "chpl/framework/update-functions.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/uast/Variable.h"

#include <vector>
#include <unordered_map>
#include <utility>

namespace chpl {

class Context;
class Location;

namespace uast {

class AstNode;

/**
  This class helps to build AST. It should only build AST
  from one file at a time.
 */
class Builder final {
 private:
  // stores symbol path and repeat number (>0 for syms with same name)
  using pathVecT = std::vector<std::pair<UniqueString,int>>;
  // maps the name to the repeat number
  using declaredHereT = std::unordered_map<UniqueString,int>;

  Context* context_;
  UniqueString filepath_;
  UniqueString startingSymbolPath_;
  AstList topLevelExpressions_;
  std::vector<ErrorMessage> errors_;

  // note: notedLocations_ might have keys pointing to deleted uAST
  // nodes in the event one is created temporarily during parsing.
  // These are removed in the astToLocation_ map.
  std::unordered_map<const AstNode*, Location> notedLocations_;

  // the following maps are computed during assignIDs
  llvm::DenseMap<ID, Location> idToLocation_;
  std::vector<Location> commentToLocation_;
  llvm::DenseMap<ID, const AstNode*> idToAst_;
  llvm::DenseMap<ID, ID> idToParent_;

  Builder(Context* context, UniqueString filepath,
          UniqueString startingSymbolPath)
    : context_(context), filepath_(filepath),
      startingSymbolPath_(startingSymbolPath)
  {
  }

  void createImplicitModuleIfNeeded();
  void assignIDs();
  void doAssignIDs(AstNode* ast, UniqueString symbolPath, int& i,
                   int& commentIndex, pathVecT& pathVec,
                   declaredHereT& duplicates);
  void postParseChecks();

 public:
  /** Construct a Builder for parsing a top-level module */
  static owned<Builder> createForTopLevelModule(Context* context,
                                                const char* filepath);

  /** Construct a Builder for parsing an included module.
      'parentSymbolPath' is the symbol path component of the ID
      of the module containing the 'module include' statement.
   */
  static owned<Builder> createForIncludedModule(Context* context,
                                                const char* filepath,
                                                UniqueString parentSymbolPath);

  Context* context() const { return context_; }

  /**
    Save a toplevel expression in to the builder.
    This is called by the parser.
   */
  void addToplevelExpression(owned<AstNode> e);

  /**
    Save an error.
   */
  void addError(ErrorMessage);

  /**
    Record the location of an AST element.
   */
  void noteLocation(AstNode* ast, Location loc);

  /**
    Assign IDs to all of the AST elements added as toplevel expressions
    to this builder and return the result. This function clears
    these elements from the builder and it becomes empty.
   */
  BuilderResult result();

  /**
    Certain uAST nodes, - Function, Module, Class, Record, Union, Enum -
    all create a new ID scope. This function returns `true` for AST tags
    with this property.
   */
  static bool astTagIndicatesNewIdScope(asttags::AstTag tag);

  /**
    Compute the module name based on a file name.
   */
  static std::string filenameToModulename(const char* filename);

  // build methods are actually type methods on the individual AST
  // elements. This prevents the Builder API from growing unreasonably large.

  /// \cond DO_NOT_DOCUMENT

  // Use this in the parser to get a mutable view of a node's children so
  // that the node can be modified in place. Later we can also add a method
  // such as 'swapChildren' if we need it.
  AstList& mutableRefToChildren(AstNode* ast) {
    return ast->children_;
  }

  void addOrReplaceInitExpr(Variable* var, owned<AstNode> ie) {
    var->setInitExprForConfig(std::move(ie));
  }


  // Use this to take the children of an AST node. The AST node is marked
  // as owned because it is consumed.
  AstList takeChildren(owned<AstNode> ast) {
    auto ret = std::move(ast->children_);
    assert(ast->children_.size() == 0);
    return ret;
  }

  // Use this to flatten top level blocks within an AstList.
  AstList flattenTopLevelBlocks(AstList lst);

  // Use this to get a temporary location while parsing.
  Location getLocation(const AstNode* ast);

  // check for the existence of new config values (from the command line) for this var
  void lookupConfigSettingsForVar(Variable* var, pathVecT& pathVec, std::string& name, std::string& value);

  // update the initExpr of a config with values passed from the command line
  AstNode* updateConfig(Variable* var, std::string configName, std::string configVal);

  // recursively note the location of a nodes children as the location of the parent
  // used when updating a config with a new initExpr
  void noteChildrenLocations(AstNode* ast, Location loc);

  // used to check if a config assignment was used in a previous assignment
  void checkConfigPreviouslyUsed(const Variable* var, std::string& configNameUsed);

  // build a dummy input string and parse it, extracting the initExpr and returning it
  owned <AstNode> parseDummyNodeForInitExpr(Variable* var, std::string value);

  // check that all the configs passed from the command line were consumed
  static bool checkAllConfigVarsAssigned(Context* context);

  /// \endcond

};

} // end namespace uast
} // end namespace chpl


#endif
