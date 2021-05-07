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

#ifndef CHPL_UAST_LOCAL_H
#define CHPL_UAST_LOCAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Loop.h"

namespace chpl {
namespace uast {


/**
  This class represents a for loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      for i in foo {
        var x;
      }

  \endrst

 */
class For final : public Loop {
 private:
  For(ASTList children, int8_t indexVarChildNum, int8_t iterandChildNum,
      bool usesDo,
      bool expressionLevel,
      bool param);
  bool contentsMatchInner(const ASTNode* other) const;
  void markUniqueStringsInner(Context* context) const;
  int8_t indexVarChildNum_;
  int8_t iterandChildNum_;
  bool expressionLevel_;
  bool param_;

 public:

  /**
    Create and return a for loop. 
  */
  static owned<For> build(Builder* builder, Location loc,
                          owned<Expression> indexVar,
                          owned<Expression> iterand,
                          ASTList stmts,
                          bool usesDo,
                          bool expressionLevel,
                          bool param);

  /**
    Create and return a for loop without an index variable. This variation
    cannot be param and cannot appear at the expression level.
  */
  static owned<For> build(Builder* builder, Location loc,
                          owned<Expression> iterand,
                          ASTList stmts,
                          bool usesDo);

  /**
    Returns the index variable of this for loop.
  */
  const Expression* indexVar() const {
    return indexVarChildNum_ < 0 ? nullptr
      : (const Expression*)child(indexVarChildNum_);
  }

  /**
    Returns the iterand of this for loop.
  */
  const Expression* iterand() const {
    return iterandChildNum_ < 0 ? nullptr
      : (const Expression*)child(iterandChildNum_);
  }

  /**
    Returns true if this for loop appears at the expression level. 
  */
  bool expressionLevel() const {
    return expressionLevel_;
  }

  /**
    Returns true if this for loop is param.
  */
  bool param() const {
    return param_;
  }


};


} // end namespace uast
} // end namespace chpl

#endif
