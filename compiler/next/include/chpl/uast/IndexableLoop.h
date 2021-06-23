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

#ifndef CHPL_UAST_INDEXABLELOOP_H
#define CHPL_UAST_INDEXABLELOOP_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Loop.h"
#include "chpl/uast/Variable.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents an indexable loop.
 */
class IndexableLoop : public Loop {
 protected:
  IndexableLoop(ASTTag tag, ASTList children,
                int8_t indexChildNum,
                int8_t iterandChildNum,
                int8_t withClauseChildNum,
                BlockStyle blockStyle,
                int loopBodyChildNum,
                int numLoopBodyStmts,
                bool isExpressionLevel)
    : Loop(tag, std::move(children), blockStyle,
           loopBodyChildNum,
           numLoopBodyStmts),
      indexChildNum_(indexChildNum),
      iterandChildNum_(iterandChildNum),
      withClauseChildNum_(withClauseChildNum),
      isExpressionLevel_(isExpressionLevel) {

    assert(isExpressionASTList(children_));
    assert(iterandChildNum >= 0);
  }

  bool indexableLoopContentsMatchInner(const IndexableLoop* other) const;

  void indexableLoopMarkUniqueStringsInner(Context* context) const {
    loopMarkUniqueStringsInner(context);
  }

  int8_t indexChildNum_;
  int8_t iterandChildNum_;
  int8_t withClauseChildNum_;
  bool isExpressionLevel_;

 public:
  virtual ~IndexableLoop() override = 0; // this is an abstract base class

  /**
    Returns the index declaration of this indexable loop, or nullptr if
    there is none.
  */
  const Decl* index() const {
    if (indexChildNum_ < 0) return nullptr;
    const ASTNode* ast = child(indexChildNum_);
    assert(ast->isDecl());
    return (const Decl*) ast;
  }

  /**
    Returns the iterand of this indexable loop.
  */
  const Expression* iterand() const {
    if (iterandChildNum_ < 0) return nullptr;
    const ASTNode* ast = child(iterandChildNum_); 
    assert(ast->isExpression());
    return (const Expression*) ast;
  }

  /**
    Returns the with clause of this indexable loop, or nullptr if there
    is none.
  */
  const WithClause* withClause() const {
    if (withClauseChildNum_ < 0) return nullptr;
    auto ret = child(withClauseChildNum_);
    assert(ret->isWithClause());
    return (const WithClause*)ret;
  }

  /**
    Returns true if this indexable loop appears at the expression level.
  */
  bool isExpressionLevel() const {
    return isExpressionLevel_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
