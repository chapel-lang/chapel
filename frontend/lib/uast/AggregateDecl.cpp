/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/AggregateDecl.h"

#include "chpl/uast/Builder.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"

namespace chpl {
namespace uast {


bool AggregateDecl::validAggregateChildren(AstListIteratorPair<AstNode> it) {
  for (auto elt: it) {
    if (elt->isComment() || elt->isErroneousExpression() ||
        elt->isEmptyStmt()) {
      // OK
    } else if (elt->isDecl()) {
      if (elt->isVariable() || elt->isFunction() || elt->isTupleDecl() ||
          elt->isMultiDecl() ||
          elt->isAggregateDecl() ||
          elt->isForwardingDecl() ||
          elt->isTypeDecl()) {
        // OK
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

std::string AggregateDecl::aggregateDeclDumpChildLabelInner(int i) const {
  if (i >= inheritExprChildNum_ && i  < inheritExprChildNum_ + numInheritExprs_) {
    return "inherit-expr";
  }

  return "";
}

AggregateDecl::~AggregateDecl() {
}

const AstNode* AggregateDecl::getUnwrappedInheritExpr(const AstNode* ast,
                                                      bool& markedGeneric) {
  // This doesn't do a deep check that e.g., the Dot expression is well-formed.
  // This is expected to be done once during post-parse checks.

  if (ast != nullptr) {
    if (ast->isIdentifier() || ast->isDot()) {
      // inheriting from e.g. Parent or M.Parent is OK
      markedGeneric = false;
      return ast;
    } else if (auto call = ast->toFnCall()) {
      const AstNode* calledExpr = call->calledExpression();
      if (calledExpr != nullptr &&
          (calledExpr->isIdentifier() || calledExpr->isDot()) &&
          call->numActuals() == 1) {
        if (const AstNode* actual = call->actual(0)) {
          if (auto id = actual->toIdentifier()) {
            if (id->name() == USTR("?")) {
              // inheriting from e.g. Parent(?) is OK
              markedGeneric = true;
              return calledExpr;
            }
          }
        }
      }
    }
  }

  markedGeneric = false;
  return nullptr;
}


} // namespace uast
} // namespace chpl
