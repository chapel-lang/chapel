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

#ifndef CHPL_UAST_FOR_H
#define CHPL_UAST_FOR_H

#include "chpl/queries/Location.h"
#include "chpl/uast/IndexableLoop.h"

namespace chpl {
namespace uast {


/**
  This class represents a for loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      for i in myRange {
        var x;
      }

  \endrst

 */
class For final : public IndexableLoop {
 private:
  For(ASTList children,  int8_t indexVariableDeclChildNum,
      int8_t iterandChildNum,
      bool usesDo,
      bool isExpressionLevel,
      bool isParam)
        : IndexableLoop(asttags::For, std::move(children),
                        indexVariableDeclChildNum,
                        iterandChildNum,
                        usesDo),
      isExpressionLevel_(isExpressionLevel),
      isParam_(isParam) {

    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

  bool isExpressionLevel_;
  bool isParam_;

 public:

  /**
    Create and return a for loop. 
  */
  static owned<For> build(Builder* builder, Location loc,
                          owned<Decl> indexVariableDecl,
                          owned<Expression> iterand,
                          ASTList stmts,
                          bool usesDo,
                          bool isExpressionLevel,
                          bool isParam);

  /**
    Create and return a for loop without an index variable.
  */
  static owned<For> build(Builder* builder, Location loc,
                          owned<Expression> iterand,
                          ASTList stmts,
                          bool usesDo);

  /**
    Returns true if this for loop appears at the expression level. 
  */
  bool isExpressionLevel() const {
    return isExpressionLevel_;
  }

  /**
    Returns true if this for loop is param.
  */
  bool isParam() const {
    return isParam_;
  }


};


} // end namespace uast
} // end namespace chpl

#endif
