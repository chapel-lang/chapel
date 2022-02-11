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

#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/mark-functions.h"
#include "chpl/queries/update-functions.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/BuilderResult.h"

#include <vector>
#include <unordered_map>
#include <utility>

namespace chpl {

class Context;
class Location;

namespace uast {

class Expression;

/**
  This class helps to build AST. It should only build AST
  from one file at a time.
 */
class Builder final {
 private:
  using pathVecT = std::vector<std::pair<UniqueString,int>>;
  using declaredHereT = std::unordered_map<UniqueString,int>;

  Context* context_;
  UniqueString filepath_;
  ASTList topLevelExpressions_;
  std::vector<ErrorMessage> errors_;

  // note: notedLocations_ might have keys pointing to deleted uAST
  // nodes in the event one is created temporarily during parsing.
  // These are removed in the astToLocation_ map.
  std::unordered_map<const ASTNode*, Location> notedLocations_;

  // the following maps are computed during assignIDs
  std::unordered_map<ID, Location> idToLocation_;
  std::vector<Location> commentToLocation_;
  std::unordered_map<ID, const ASTNode*> idToAst_;
  std::unordered_map<ID, ID> idToParent_;

  Builder(Context* context, UniqueString filepath)
    : context_(context), filepath_(filepath)
  {
  }

  void createImplicitModuleIfNeeded();
  void assignIDs();
  void doAssignIDs(ASTNode* ast, UniqueString symbolPath, int& i,
                   int& commentIndex, pathVecT& pathVec,
                   declaredHereT& duplicates);

 public:
  static owned<Builder> build(Context* context, const char* filepath);

  Context* context() const { return context_; }

  /**
    Save a toplevel expression in to the builder.
    This is called by the parser.
   */
  void addToplevelExpression(owned<Expression> e);

  /**
    Save an error.
   */
  void addError(ErrorMessage);

  /**
    Record the location of an AST element.
   */
  void noteLocation(ASTNode* ast, Location loc);

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
  static bool astTagIndicatesNewIdScope(asttags::ASTTag tag);

  // build methods are actually type methods on the individual AST
  // elements. This prevents the Builder API from growing unreasonably large.

  /// \cond DO_NOT_DOCUMENT

  // Use this in the parser to get a mutable view of a node's children so
  // that the node can be modified in place. Later we can also add a method
  // such as 'swapChildren' if we need it.
  ASTList& mutableRefToChildren(ASTNode* ast) {
    return ast->children_;
  }

  // Use this to take the children of an AST node. The AST node is marked
  // as owned because it is consumed.
  ASTList takeChildren(owned<ASTNode> ast) {
    auto ret = std::move(ast->children_);
    assert(ast->children_.size() == 0);
    return ret;
  }

  // Use this to flatten top level blocks within an ASTList.
  ASTList flattenTopLevelBlocks(ASTList lst);

  /// \endcond
};

} // end namespace uast
} // end namespace chpl


#endif
