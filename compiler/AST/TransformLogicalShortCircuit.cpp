/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "TransformLogicalShortCircuit.h"

#include "build.h"
#include "expr.h"
#include "IfExpr.h"
#include "stmt.h"

TransformLogicalShortCircuit::TransformLogicalShortCircuit()
{
}

TransformLogicalShortCircuit::~TransformLogicalShortCircuit()
{

}

bool TransformLogicalShortCircuit::enterCallExpr(CallExpr* call)
{
  // Lowering of LoopExprs will handle short-circuits itself
  if (call->primitive == 0 && isLoopExpr(call->parentExpr) == false)
  {
    if (UnresolvedSymExpr* expr = toUnresolvedSymExpr(call->baseExpr))
    {
      bool isLogicalAnd = strcmp(expr->unresolved, "&&") == 0;
      bool isLogicalOr  = strcmp(expr->unresolved, "||") == 0;

      if (isLogicalAnd || isLogicalOr)
      {
        SET_LINENO(call);

        Expr*      left  = call->get(1);
        Expr*      right = call->get(2);
        VarSymbol* lvar  = newTemp();

        VarSymbol* eMsg  = NULL;
        IfExpr*    ife   = NULL;

        left->remove();
        right->remove();

        lvar->addFlag(FLAG_MAYBE_PARAM);

        if (isLogicalAnd)
        {
          eMsg = new_StringSymbol("cannot promote short-circuiting && operator");
          ife = new IfExpr(new CallExpr("isTrue", lvar),
                           new CallExpr("isTrue", right),
                           new SymExpr(gFalse));
        }
        else
        {
          eMsg = new_StringSymbol("cannot promote short-circuiting || operator");
          ife = new IfExpr(new CallExpr("isTrue", lvar),
                           new SymExpr(gTrue),
                           new CallExpr("isTrue", right));
        }

        //
        // By handling conditionals in pre-order, we do not need to store an
        // insertion point. The top-level conditional will be inserted before
        // the original statement, and any nested conditionals will be stored
        // within the IfExprs blocks which are still before the original
        // statement.
        //
        Expr* stmt = call->getStmtExpr();
        stmt->insertBefore(new DefExpr(lvar));
        stmt->insertBefore(new CallExpr(PRIM_MOVE, lvar, left));
        stmt->insertBefore(new CondStmt(new CallExpr("_cond_invalid", lvar),
                                        new CallExpr("compilerError", eMsg)));

        call->replace(ife);

        left->accept(this);
        ife->accept(this);
      }
    }
  }
  return true;
}
