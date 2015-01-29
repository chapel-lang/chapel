/************************************ | *************************************
*                                                                           *
* Please see header file for description.                                   *
*                                                                           *
************************************* | ************************************/

#include "TransformLogicalShortCircuit.h"

#include "build.h"
#include "expr.h"
#include "stmt.h"

TransformLogicalShortCircuit::TransformLogicalShortCircuit(Expr* insertionPoint)
{
  mInsertionPoint = insertionPoint;
}

TransformLogicalShortCircuit::~TransformLogicalShortCircuit()
{

}

void TransformLogicalShortCircuit::exitCallExpr(CallExpr* call)
{
  if (call->primitive == 0)
  {
    if (UnresolvedSymExpr* expr = toUnresolvedSymExpr(call->baseExpr))
    {
      bool isLogicalAnd = strcmp(expr->unresolved, "&&") == 0;
      bool isLogicalOr  = strcmp(expr->unresolved, "||") == 0;

      if (isLogicalAnd || isLogicalOr)
      {
        SET_LINENO(call);

        if (call->getStmtExpr() == mInsertionPoint)
        {
          Expr*      left  = call->get(1);
          Expr*      right = call->get(2);
          VarSymbol* lvar  = newTemp();

          VarSymbol* eMsg  = NULL;
          FnSymbol*  ifFn  = NULL;

          left->remove();
          right->remove();

          lvar->addFlag(FLAG_MAYBE_PARAM);

          if (isLogicalAnd)
          {
            eMsg = new_StringSymbol("cannot promote short-circuiting && operator");
            ifFn = buildIfExpr(new CallExpr("isTrue", lvar),
                               new CallExpr("isTrue", right),
                               new SymExpr(gFalse));
          }
          else
          {
            eMsg = new_StringSymbol("cannot promote short-circuiting || operator");
            ifFn = buildIfExpr(new CallExpr("isTrue", lvar),
                               new SymExpr(gTrue),
                               new CallExpr("isTrue", right));
          }

          ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar),
                                          new CallExpr("compilerError", eMsg)));
          ifFn->insertAtHead(new CallExpr(PRIM_MOVE, lvar, left));
          ifFn->insertAtHead(new DefExpr(lvar));

          call->baseExpr->replace(new UnresolvedSymExpr(ifFn->name));

          mInsertionPoint->insertBefore(new DefExpr(ifFn));
        }
      }
    }
  }
}
