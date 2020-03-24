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

#include "WhileStmt.h"

#include "astutil.h"
#include "expr.h"
#include "stlUtil.h"

WhileStmt::WhileStmt(Expr* condExpr, BlockStmt* body) :
  LoopStmt(body)
{
  mCondExpr = condExpr;
}

WhileStmt::WhileStmt(VarSymbol* var, BlockStmt* body) :
  LoopStmt(body)
{
  mCondExpr = (var != 0) ? new SymExpr(var) : 0;
}

WhileStmt::~WhileStmt()
{

}

void WhileStmt::copyShare(const WhileStmt& ref,
                          SymbolMap*       mapRef,
                          bool             internal)
{
  SymbolMap  localMap;
  SymbolMap* map       = (mapRef != 0) ? mapRef : &localMap;
  Expr*      condExpr  = ref.condExprGet();

  astloc            = ref.astloc;
  blockTag          = ref.blockTag;

  mBreakLabel       = ref.mBreakLabel;
  mContinueLabel    = ref.mContinueLabel;
  mOrderIndependent = ref.mOrderIndependent;

  if (condExpr != 0)
    mCondExpr = condExpr->copy(map, true);

  for_alist(expr, ref.body)
    insertAtTail(expr->copy(map, true));

  if (internal == false)
    update_symbols(this, map);
}

void WhileStmt::verify()
{
  BlockStmt::verify();

  if (condExprGet()             == 0)
    INT_FATAL(this, "WhileStmt::verify. condExpr  is NULL");

  if (BlockStmt::blockInfoGet() != 0)
    INT_FATAL(this, "WhileStmt::verify. blockInfo is not NULL");

  if (useList                   != 0)
    INT_FATAL(this, "WhileStmt::verify. useList   is not NULL");

  if (byrefVars                 != 0)
    INT_FATAL(this, "WhileStmt::verify. byrefVars is not NULL");
}

bool WhileStmt::isWhileStmt() const
{
  return true;
}

Expr* WhileStmt::condExprGet() const
{
  return mCondExpr;
}

// Much of the compiler expects the condExpr to be a
// SymExpr that references a tmpVariable for the loop.
SymExpr* WhileStmt::condExprForTmpVariableGet() const
{
  SymExpr* retval = toSymExpr(mCondExpr);

  INT_ASSERT(retval != 0);

  return retval;
}

void WhileStmt::replaceChild(Expr* oldAst, Expr* newAst)
{
  if (oldAst == mCondExpr)
    mCondExpr = newAst;
  else
    BlockStmt::replaceChild(oldAst, newAst);
}

CallExpr* WhileStmt::blockInfoGet() const
{
  printf("Migration: WhileStmt %12d Unexpected call to blockInfoGet()\n", id);

  return 0;
}

CallExpr* WhileStmt::blockInfoSet(CallExpr* expr)
{
  printf("Migration: WhileStmt %12d Unexpected call to blockInfoSet()\n", id);

  return 0;
}

bool WhileStmt::deadBlockCleanup()
{
  bool retval = false;

  if (condExprGet() == 0)
  {
    remove();
    retval = true;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Additional Validation                                                       *
*                                                                             *
************************************** | *************************************/

// This routine looks for loops in which the condition variable is *not*
// updated within the body of the loop, and issues a warning for places
// in the code where that occurs.
void WhileStmt::checkConstLoops()
{
  SymExpr* tmpVar  = condExprForTmpVariableGet();

  // Get the loop condition variable.
  if (VarSymbol* condSym = toVarSymbol(tmpVar->symbol()))
  {
    // Look for definitions of the loop condition variable
    // within the body of the loop.
    if (SymExpr* condDef = getWhileCondDef(condSym))
    {
      // Get the call expression that updates the condition variable.
      if (CallExpr* outerCall = toCallExpr(condDef->parentExpr))
      {
        // Assume the outer call is a move expression and that its LHS is
        // the (SymExpr that contains the) loop condition variable.
        if (outerCall->get(1) == condDef)
        {
          if (outerCall->isPrimitive(PRIM_MOVE))
          {
            // Expect the update to be the result of a call to _cond_test.
            if (CallExpr* innerCall = toCallExpr(outerCall->get(2)))
            {
              FnSymbol* fn = innerCall->resolvedFunction();

              if (innerCall->numActuals()        == 1 &&
                  strcmp(fn->name, "_cond_test") == 0)
              {
                checkWhileLoopCondition(innerCall->get(1));
              }
              else
              {
                INT_FATAL(innerCall,
                          "Expected the update of a loop conditional "
                          "to be piped through _cond_test().");
              }
            }

            // The RHS of the move can also be a SymExpr as the result of param
            // folding ...
            else if (SymExpr* moveSrc = toSymExpr(outerCall->get(2)))
            {
              // ... in which case, the literal should be 'true' or 'false'.
              if (moveSrc->symbol() == gTrue)
              {
                // while true do ... ;  -- probably OK.
                // User said to loop forever ... .
              }

              else if (moveSrc->symbol() == gFalse)
              {
                // while false do ...; -- probably nothing to worry about
                // We probably don't get here unless fRemoveUnreachableBlocks
                // is false.
              }

              else
              {
                INT_FATAL(moveSrc,
                          "Expected const loop condition variable to be "
                          "true or false.");
              }
            }

            else
            {
              // The RHS was neither a CallExpr nor a SymExpr.
              INT_FATAL(outerCall,
                        "Invalid RHS in a loop condition variable update "
                        "expression.");
            }
          }

          else
          {
            INT_FATAL(outerCall,
                      "Expected a loop condition variable update to "
                      "be a MOVE.");
          }
        }
        else
        {
          // Note that this being true depends on the compiler inserting a temp
          // that is the result of applying _cond_test to a more-general loop
          // conditional expression.
          // Copy propagation could potentially make this false again....
          INT_FATAL(condDef,
                    "Expected loop condition variable to be only "
                    "updated (not read).");
        }
      }

      else
      {
        INT_FATAL(condDef,
                  "The update of a loop condition variable could not "
                  "be converted to a call.");
      }
    }
    else
    {
      // There was no update of the loop condition variable in the
      // body of the loop.
      // It could be an infinite loop, or it could have a
      // 'break' or 'return' in it.
    }
  }

  else
  {
    INT_FATAL(tmpVar,
              "The loop condition variable could not be converted "
              "to a VarSymbol.");
  }
}

// Find a definition of the condition variable in the body of the loop.
// Returns null if no such expression is found.
SymExpr* WhileStmt::getWhileCondDef(VarSymbol* condSym)
{
  std::vector<SymExpr*> symExprs;
  SymExpr*              condDef = NULL;

  collectSymExprsFor(this, condSym, symExprs);

  for_vector(SymExpr, se, symExprs)
  {
      if (se == mCondExpr)
      {
        // The reference is the condition expression - not interesting.
      }

      else if (condDef)
      {
        // There are >1 references to condSym. Let us notify ourselves
        // so we can adjust the code to handle this case as well.
        // If desired, disable this assert - the only outcome of that may be
        // that the warning will not be issued in some cases.
        INT_ASSERT(false);
      }

      else
      {
        // This is what we are looking for.
        condDef = se;
      }
  }

  return condDef;
}

void WhileStmt::checkWhileLoopCondition(Expr* condExp)
{
  if (SymExpr* condSE = toSymExpr(condExp))
  {
    Symbol* condSym = condSE->symbol();

    if (condSym->isConstant() == true && symDeclaredInBlock(condSym) == false)
      checkConstWhileLoop();
  }
}

bool WhileStmt::symDeclaredInBlock(Symbol* condSym)
{
  Expr* parent = condSym->defPoint->parentExpr;
  bool  retval = false;

  while (parent && retval == false)
  {
    if (parent == this)
      retval = true;
    else
      parent = parent->parentExpr;
  }

  return retval;
}

void WhileStmt::checkConstWhileLoop()
{
  if (loopBodyHasExits() == false)
    USR_WARN(this, "A while loop with a constant condition");
}

bool WhileStmt::loopBodyHasExits()
{
  std::vector<Expr*> exprs;

  collectExprs(this, exprs);

  for_vector(Expr, node, exprs)
  {
    if (CallExpr* call = toCallExpr(node))
    {
      if (call->isPrimitive(PRIM_YIELD) || call->isPrimitive(PRIM_RETURN))
        return true;
    }

    else if (GotoStmt* gs = toGotoStmt(node))
    {
      if (gs->gotoTag == GOTO_RETURN || gs->gotoTag == GOTO_BREAK)
        return true;
    }
  }

  return false;
}
