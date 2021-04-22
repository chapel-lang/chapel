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

#include "chpl/AST/Variable.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


Variable::Variable(ASTList children,
                   UniqueString name, Symbol::Visibility vis,
                   Variable::Tag tag,
                   int8_t typeExprChildNum, int8_t initExprChildNum)
  : Symbol(asttags::Variable, std::move(children), name, vis),
    tag_(tag),
    typeExprChildNum(typeExprChildNum), initExprChildNum(initExprChildNum) {

  assert(-1 <= typeExprChildNum && typeExprChildNum <= 1);
  assert(-1 <= initExprChildNum && initExprChildNum <= 1);
  assert(numChildren() <= 2);
#ifndef NDEBUG
  // check that all children are exprs (and not, say, Symbols)
  for (int i = 0; i < this->numChildren(); i++) {
    assert(child(i)->isExpr());
  }
#endif
}

bool Variable::contentsMatchInner(const BaseAST* other) const {
  const Variable* lhs = this;
  const Variable* rhs = (const Variable*) other;
  return lhs->symbolContentsMatchInner(rhs) &&
         lhs->tag_ == rhs->tag_ &&
         lhs->typeExprChildNum == rhs->typeExprChildNum &&
         lhs->initExprChildNum == rhs->initExprChildNum;
}



} // namespace ast
} // namespace chpl
