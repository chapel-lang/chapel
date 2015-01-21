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

  astloc         = ref.astloc;
  blockTag       = ref.blockTag;

  mBreakLabel    = ref.mBreakLabel;
  mContinueLabel = ref.mContinueLabel;

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

  if (modUses                   != 0)
    INT_FATAL(this, "WhileStmt::verify. modUses   is not NULL");

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

/************************************ | *************************************
*                                                                           *
* Additional Validation                                                     *
*                                                                           *
************************************* | ************************************/

void WhileStmt::checkConstLoops()
{
  SymExpr* tmpVar  = condExprForTmpVariableGet();
  bool     foundit = false;

  if (VarSymbol* condSym = toVarSymbol(tmpVar->var))
  {
    if (SymExpr* condDef = getWhileCondDef(condSym))
    {
      // Parse the move expr.
      if (CallExpr* outerCall = toCallExpr(condDef->parentExpr))
      {
        if (outerCall->get(1) == condDef)
        {
          if (outerCall->isPrimitive(PRIM_MOVE))
          {
            if (CallExpr* innerCall = toCallExpr(outerCall->get(2)))
            {
              if (innerCall->numActuals()          == 1 &&
                  innerCall->isNamed("_cond_test") == true)
              {
                foundit = true;
                checkWhileLoopCondition(innerCall->get(1));
              }
            }

            else if (SymExpr* moveSrc = toSymExpr(outerCall->get(2)))
            {
              // Sometimes _cond_test resolves to a param version, so
              // we get either true or false.
              if (moveSrc->var == gTrue)
              {
                foundit = true;
              }

              else if (moveSrc->var == gFalse)
              {
                foundit = true;
                // while false do ...; -- probably nothing to worry about
              }

              else
              {
                // What else can it be?
              }
            }
          }
        }
      }
    }
  }

  // Let us notify ourselves if the block structure is something else,
  // so we can adjust the code to handle those cases as well.
  // If desired, disable this assert - the only outcome of that may be
  // that the warning will not be issued in some cases.
  INT_ASSERT(foundit);
}

SymExpr* WhileStmt::getWhileCondDef(VarSymbol* condSym)
{
  std::vector<SymExpr*> symExprs;
  SymExpr*              condDef = NULL;

  collectSymExprsSTL(this, symExprs);

  for_vector(SymExpr, se, symExprs)
  {
    if (se->var == condSym)
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
  }

  return condDef;
}

void WhileStmt::checkWhileLoopCondition(Expr* condExp)
{
  if (SymExpr* condSE = toSymExpr(condExp))
  {
    Symbol* condSym = condSE->var;

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
