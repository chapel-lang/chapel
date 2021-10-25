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

#include "chpl/uast/TupleDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool TupleDecl::assertAcceptableTupleDecl() {
  asttags::ASTTag firstNonTupleTag = asttags::START_Expression;
  int i = 0;

  for (const auto& elt: children_) {
    if (i == typeExpressionChildNum_) {
      if (!elt->isExpression()) {
        assert(false && "type expression child is not expression"); 
        return false;
      }
    } else if (i == initExpressionChildNum_) {
      if (!elt->isExpression()) {
        assert(false && "init expression child is not expression"); 
        return false;
      }
    } else if (!elt->isTupleDecl()) {
      if (elt->isFormal() || elt->isVariable()) {
        if (firstNonTupleTag == asttags::START_Expression) {
          firstNonTupleTag = elt->tag();
        }

        if (elt->tag() != firstNonTupleTag) {
          assert(0 == "cannot mix formal and variable components");
          return false;
        }
      } else {
        assert(0 == "variable, formal, or tuple decl components only");
        return false;
      }
    } else {
      assert(elt->isTupleDecl());
    }
    i++;
  }
  return true;
}

owned<TupleDecl> TupleDecl::build(Builder* builder, Location loc,
                                  Decl::Visibility vis,
                                  Decl::Linkage linkage,
                                  TupleDecl::IntentOrKind intentOrKind,
                                  ASTList elements,
                                  owned<Expression> typeExpression,
                                  owned<Expression> initExpression) {
  ASTList list;
  int numElements = 0;
  int typeExpressionChildNum = -1;
  int initExpressionChildNum = -1;

  // start out with the child declarations
  list.swap(elements);
  numElements = list.size();

  // then add the typeExpression, if any
  if (typeExpression.get() != nullptr) {
    typeExpressionChildNum = list.size();
    list.push_back(std::move(typeExpression));
  }

  // and then the initExpression
  if (initExpression.get() != nullptr) {
    initExpressionChildNum = list.size();
    list.push_back(std::move(initExpression));
  }

  TupleDecl* ret = new TupleDecl(std::move(list), vis, linkage,
                                 intentOrKind,
                                 numElements,
                                 typeExpressionChildNum,
                                 initExpressionChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
