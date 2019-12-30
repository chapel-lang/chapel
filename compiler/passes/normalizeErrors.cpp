/*
 * Copyright 2004-2020 Cray Inc.
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

#include "errorHandling.h"

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

  if (call->tryTag != TRY_TAG_NONE) {
    // if this expr is already marked with try or try! leave it alone
    tag = call->tryTag;
  } else if (isTryBang || parentTag == TRY_TAG_IN_TRYBANG) {
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

class NormalizeThrowsVisitor : public AstVisitorTraverse {

public:
  NormalizeThrowsVisitor();

  virtual bool enterCallExpr   (CallExpr*   call);
};

NormalizeThrowsVisitor::NormalizeThrowsVisitor()
{
}

bool NormalizeThrowsVisitor::enterCallExpr(CallExpr* call) {

  if (call->isPrimitive(PRIM_THROW)) {
    SET_LINENO(call);

    INT_ASSERT(call->numActuals() == 1);
    CallExpr* callFixError = new CallExpr("chpl_fix_thrown_error",
                                          call->get(1)->remove());
    call->insertAtTail(callFixError);
  }

  return true;
}

} /* end anon namespace */

static
void lowerTryExprs(BaseAST* ast)
{
  NormalizeTryExprsVisitor n;
  ast->accept(&n);
}

static
void normalizeThrows(BaseAST* ast) {
  NormalizeThrowsVisitor n;
  ast->accept(&n);
}

void normalizeErrorHandling(BaseAST* ast)
{
  lowerTryExprs(ast);
  normalizeThrows(ast);
}
