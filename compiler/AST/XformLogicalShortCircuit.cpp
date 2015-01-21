/*
 * Copyright 2004-2015 Cray Inc.
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

/************************************ | *************************************
*                                                                           *
* Please see header file for description.                                   *
*                                                                           *
************************************* | ************************************/

#include "XformLogicalShortCircuit.h"

#include "build.h"
#include "expr.h"
#include "stmt.h"

XformLogicalShortCircuit::XformLogicalShortCircuit(Expr* insertionPoint)
{
  mInsertionPoint = insertionPoint;
}

XformLogicalShortCircuit::~XformLogicalShortCircuit()
{

}

void XformLogicalShortCircuit::exitCallExpr(CallExpr* call)
{
  if (call->primitive == 0)
  {
    if (UnresolvedSymExpr* expr = toUnresolvedSymExpr(call->baseExpr))
    {
      if (strcmp(expr->unresolved, "&&") == 0)
      {
        SET_LINENO(call);

        Expr*      left  = call->get(1);
        Expr*      right = call->get(2);

        left->remove();
        right->remove();

        VarSymbol* lvar  = newTemp();
        VarSymbol* eMsg  = new_StringSymbol("cannot promote short-circuiting && operator");
        FnSymbol*  ifFn  = buildIfExpr(new CallExpr("isTrue", lvar),
                                       new CallExpr("isTrue", right),
                                       new SymExpr(gFalse));
        DefExpr*   fnDef = new DefExpr(ifFn);

        lvar->addFlag(FLAG_MAYBE_PARAM);

        ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar),
                                        new CallExpr("compilerError", eMsg)));
        ifFn->insertAtHead(new CallExpr(PRIM_MOVE, lvar, left));
        ifFn->insertAtHead(new DefExpr(lvar));

        call->baseExpr->replace(new UnresolvedSymExpr(ifFn->name));

        mInsertionPoint->insertBefore(fnDef);
      }

      if (strcmp(expr->unresolved, "||") == 0)
      {
        SET_LINENO(call);

        Expr*      left  = call->get(1);
        Expr*      right = call->get(2);

        left->remove();
        right->remove();

        VarSymbol* lvar  = newTemp();
        VarSymbol* eMsg  = new_StringSymbol("cannot promote short-circuiting || operator");
        FnSymbol*  ifFn  = buildIfExpr(new CallExpr("isTrue", lvar),
                                       new SymExpr(gTrue),
                                       new CallExpr("isTrue", right));
        DefExpr*   fnDef = new DefExpr(ifFn);

        lvar->addFlag(FLAG_MAYBE_PARAM);

        ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar),
                                        new CallExpr("compilerError", eMsg)));
        ifFn->insertAtHead(new CallExpr(PRIM_MOVE, lvar, left));
        ifFn->insertAtHead(new DefExpr(lvar));

        call->baseExpr->replace(new UnresolvedSymExpr(ifFn->name));

        mInsertionPoint->insertBefore(fnDef);
      }
    }
  }
}
