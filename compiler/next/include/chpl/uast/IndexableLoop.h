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
                int8_t indexVariableChildNum,
                int8_t iterandChildNum,
                int8_t loopBodyChildNum,
                bool usesDo)
    : Loop(tag, std::move(children), loopBodyChildNum, usesDo),
      indexVariableChildNum_(indexVariableChildNum),
      iterandChildNum_(iterandChildNum) {

    assert(isExpressionASTList(children_));
    assert(iterandChildNum >= 0);
  }

  bool indexableLoopContentsMatchInner(const IndexableLoop* other) const {
    return this->loopContentsMatchInner((const Loop*)other);
  }

  void indexableLoopMarkUniqueStringsInner(Context* context) const {
    this->loopMarkUniqueStringsInner(context);
  }

  int8_t indexVariableChildNum_;
  int8_t iterandChildNum_;

 public:
  virtual ~IndexableLoop() override = 0; // this is an abstract base class

  /**
    Returns the index variable declaration of this indexable loop. If there
    is no index variable this method returns nullptr.
  */
  const Decl* indexVariable() const {
    if (indexVariableChildNum_ < 0) return nullptr;
    const ASTNode* ast = child(indexVariableChildNum_);
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

};


} // end namespace uast
} // end namespace chpl

#endif
