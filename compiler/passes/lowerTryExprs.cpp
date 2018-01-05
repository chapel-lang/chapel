/*
 * Copyright 2004-2018 Cray Inc.
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

#include "lowerTryExprs.h"

#include "AstVisitorTraverse.h"
#include "stmt.h"
#include "symbol.h"

#include <stack>

namespace {

class NormalizeTryExprsVisitor : public AstVisitorTraverse {

public:
  NormalizeTryExprsVisitor();

  virtual bool enterCallExpr  (CallExpr*   call);
  virtual void exitCallExpr   (CallExpr*   call);

private:

  std::stack<TryTag> stack;
};

NormalizeTryExprsVisitor::NormalizeTryExprsVisitor()
  : stack()
{
}

bool NormalizeTryExprsVisitor::enterCallExpr(CallExpr* call) {

  bool isTry = call->isPrimitive(PRIM_TRY_EXPR);
  bool isTryBang = call->isPrimitive(PRIM_TRYBANG_EXPR);

  TryTag tag = TRY_TAG_NONE;
  TryTag parentTag = TRY_TAG_NONE;

  if (!stack.empty()) {
    // Gather parent try/tryBang
    parentTag = stack.top();
  }

  if (isTryBang || parentTag == TRY_TAG_IN_TRYBANG) {
    // try! on this expr or a parent always makes it try!
    tag = TRY_TAG_IN_TRYBANG;
  } else if (isTry || parentTag == TRY_TAG_IN_TRY) {
    // if we're in a try, or parent is in a try (and not a try!)
    tag = TRY_TAG_IN_TRY;
  }

  stack.push(tag);

  call->tryTag = tag;

  return true;
}

void NormalizeTryExprsVisitor::exitCallExpr(CallExpr* call) {
  stack.pop();

  bool isTry = call->isPrimitive(PRIM_TRY_EXPR);
  bool isTryBang = call->isPrimitive(PRIM_TRYBANG_EXPR);

  if (isTry || isTryBang) {
    Expr* sub = call->get(1);
    INT_ASSERT(sub);
    sub->remove();
    call->replace(sub);
  }

}


} /* end anon namespace */

void lowerTryExprs(BaseAST* ast)
{
  NormalizeTryExprsVisitor n;
  ast->accept(&n);
}
