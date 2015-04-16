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

#include "ipeResolve.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "IpeBlockStmt.h"
#include "IpeCallExpr.h"
#include "IpeDefExpr.h"
#include "IpeEnv.h"
#include "IpeProcedure.h"
#include "IpeScopeBlock.h"
#include "IpeValue.h"
#include "ipeDriver.h"
#include "ipeEvaluate.h"
#include "stmt.h"
#include "WhileDoStmt.h"

#include <cstdio>

static SymExpr*      resolveSymExpr    (SymExpr*           expr, IpeEnv* env);
static SymExpr*      resolveUnresExpr  (UnresolvedSymExpr* expr, IpeEnv* env);

static IpeDefExpr*   resolveDefExpr    (DefExpr*           expr, IpeEnv* env);
static IpeDefExpr*   resolveDefVar     (DefExpr*           expr, IpeEnv* env);
static IpeDefExpr*   resolveDefProc    (FnSymbol*          fn,   IpeEnv* env);

static CondStmt*     resolveCondStmt   (CondStmt*          expr, IpeEnv* env);
static WhileDoStmt*  resolveWhileDoStmt(WhileDoStmt*       expr, IpeEnv* env);
static IpeBlockStmt* resolveBlockStmt  (BlockStmt*         expr, IpeEnv* env);

static IpeCallExpr*  resolveCallExpr   (CallExpr*          expr, IpeEnv* env);

Expr* resolveExpr(Expr* expr, IpeEnv* env)
{
  Expr* retval = NULL;

  if      (SymExpr*           sel = toSymExpr(expr))
    retval = resolveSymExpr(sel, env);

  else if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr))
    retval = resolveUnresExpr(sel, env);

  else if (DefExpr*           sel = toDefExpr(expr))
    retval = resolveDefExpr(sel, env);

  else if (CondStmt*          sel = toCondStmt(expr))
    retval = resolveCondStmt(sel, env);

  else if (WhileDoStmt*       sel = toWhileDoStmt(expr))
    retval = resolveWhileDoStmt(sel, env);

  else if (BlockStmt*         sel = toBlockStmt(expr))
    retval = resolveBlockStmt(sel, env);

  else if (CallExpr*          sel = toCallExpr(expr))
    retval = resolveCallExpr(sel, env);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   resolveExpr(Expr*, IpeEnv* env)  unsupported\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

static SymExpr* resolveSymExpr(SymExpr* expr, IpeEnv* env)
{
  return expr->copy();
}

static SymExpr* resolveUnresExpr(UnresolvedSymExpr* expr, IpeEnv* env)
{
  SymExpr* retval = NULL;

  if (LcnSymbol* varSym = env->findVariable(expr))
  {
    SET_LINENO(expr);

    retval = new SymExpr(varSym);
  }

  else
  {
    printf("   resolveExpr(Expr*, IpeEnv* env)  Failed to find a variable for %s\n",
           expr->unresolved);
  }

  return retval;
}

static IpeDefExpr* resolveDefExpr(DefExpr* defExpr, IpeEnv* env)
{
  SET_LINENO(defExpr);

  IpeDefExpr* retval = 0;

  if      (isVarSymbol(defExpr->sym))
  {
    retval = resolveDefVar(defExpr, env);
  }

  else if (FnSymbol*   fn  = toFnSymbol(defExpr->sym))
    retval = resolveDefProc(fn, env);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   resolveDefExpr(DefExpr*, IpeEnv* env)  unsupported\n");
    printf("   ");
    defExpr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

static IpeDefExpr* resolveDefVar(DefExpr* defExpr, IpeEnv* env)
{
  VarSymbol*  var    = toVarSymbol(defExpr->sym);
  IpeDefExpr* retval = NULL;

  if (env->isEnvForBlockStmt()         == false &&
      env->isDefinedLocally(var->name) ==  true)
  {
    printf("   Attempt to redefine \"%s\" in\n", var->name);
    env->describe(3);
    printf("\n\n\n");
    INT_ASSERT(false);
  }

  else
  {
    Expr* exprTypeExpr = NULL;
    Type* exprTypeType = NULL;

    Expr* initExpr     = NULL;
    Type* initType     = NULL;

    if (defExpr->exprType != NULL)
    {
      Expr* expr = resolveExpr(defExpr->exprType, env);

      INT_ASSERT(expr->typeInfo() == gIpeTypeType);

      exprTypeExpr = expr;
      exprTypeType = evaluateExpr(exprTypeExpr, env).typeGet();
    }

    if (defExpr->init     != NULL)
    {
      initExpr = resolveExpr(defExpr->init, env);

      if (CallExpr* callExpr = toCallExpr(initExpr))
      {
        IpeCallExpr* ipeCall = (IpeCallExpr*) callExpr;

        initType = ipeCall->typeGet();
      }
      else
        initType = initExpr->typeInfo();

      INT_ASSERT(initType);
    }

    if      (exprTypeType == NULL && initType != NULL)
      var->type = initType;

    else if (exprTypeType != NULL && initType == NULL)
      var->type = exprTypeType;

    else if (exprTypeType != NULL && initType == exprTypeType)
      var->type = exprTypeType;

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("   resolveDefVar(DefExpr*, IpeEnv* env)  unsupported\n");
      printf("   ");
      defExpr->accept(&logger);
      printf("\n\n");

      if (initExpr != NULL)
      {
        printf("   initExpr\n");
        printf("   ");
        initExpr->accept(&logger);
        printf("\n\n");
      }

      if (initType != NULL)
      {
        printf("   initType\n");
        printf("   ");
        initType->accept(&logger);
        printf("\n\n");
      }

      INT_ASSERT(false);
    }

    if (env->isEnvForBlockStmt() == false)
      env->varAdd(var);

    retval = new IpeDefExpr(var, initExpr, exprTypeExpr);
  }

  return retval;
}

static IpeDefExpr* resolveDefProc(FnSymbol* fn, IpeEnv* env)
{
  LcnSymbol* procLcn = env->findLocal(fn->name);

  if (procLcn == NULL)
  {
    VarSymbol*    procVar   = new VarSymbol(fn->name);
    IpeProcedure* procedure = new IpeProcedure(fn->name);
    int           offset    = env->allocateValue(procedure);

    INT_ASSERT(env->isEnvForModule() == true);

    procVar->type = gIpeTypeProcedure;
    procVar->locationSet(0, offset);

    env->varAdd(procVar);

    procLcn = procVar;
  }
  else
  {
    INT_ASSERT(procLcn->type == gIpeTypeProcedure);
  }

  return new IpeDefExpr(procLcn, fn);
}

static CondStmt* resolveCondStmt(CondStmt* stmt, IpeEnv* env)
{
  SET_LINENO(stmt);

  CondStmt* retval = NULL;

  if (stmt->elseStmt == NULL)
    retval = new CondStmt(resolveExpr(stmt->condExpr, env),
                          resolveExpr(stmt->thenStmt, env),
                          NULL);
  else
    retval = new CondStmt(resolveExpr(stmt->condExpr, env),
                          resolveExpr(stmt->thenStmt, env),
                          resolveExpr(stmt->elseStmt, env));

  return retval;
}

static IpeBlockStmt* resolveBlockStmt(BlockStmt* stmt, IpeEnv* parent)
{
  SET_LINENO(stmt);

  IpeBlockStmt*  ipeBlockStmt = new IpeBlockStmt(stmt, parent);
  IpeScopeBlock* scope        = ipeBlockStmt->scopeGet();
  IpeEnv         env(scope);

  if (stmt->isScopeless() == false)
  {
    // Extend the scope with every top-level variable
    for (int i = 1; i <= stmt->body.length; i++)
    {
      if (DefExpr* defExpr = toDefExpr(stmt->body.get(i)))
      {
        VarSymbol* var = toVarSymbol(defExpr->sym);

        INT_ASSERT(var);

        if (env.isDefinedLocally(var->name) == false)
        {
          env.varAdd(var);
        }
        else
        {
          printf("Attempt to redefine %s\n", var->name);
          INT_ASSERT(false);
        }
      }
    }
  }

  for (int i = 1; i <= stmt->body.length; i++)
    ipeBlockStmt->insertAtTail(resolveExpr(stmt->body.get(i), &env));

  return ipeBlockStmt;
}

static WhileDoStmt* resolveWhileDoStmt(WhileDoStmt* stmt, IpeEnv* env)
{
  SET_LINENO(stmt);

  Expr* cond = stmt->condExprGet();
  Expr* body = stmt->body.get(1);

  INT_ASSERT(stmt->body.length == 1);
  INT_ASSERT(isBlockStmt(body));

  return new WhileDoStmt(resolveExpr(cond, env),
                         toBlockStmt(resolveExpr(body, env)));
}

static IpeCallExpr* resolveCallExpr(CallExpr* callExpr, IpeEnv* env)
{
  IpeCallExpr* retval = NULL;

  if      (callExpr->baseExpr  != NULL)
  {
    if (SymExpr* procSymExpr = toSymExpr(resolveExpr(callExpr->baseExpr, env)))
    {
      if (procSymExpr->typeInfo() == gIpeTypeProcedure)
      {
        IpeValue           procValue = env->valueForVariable(toLcnSymbol(procSymExpr->var));
        IpeProcedure*      procPtr   = procValue.procedureGet();
        std::vector<Expr*> resolvedActuals;

        for (int i = 1; i <= callExpr->numActuals(); i++)
          resolvedActuals.push_back(resolveExpr(callExpr->get(i), env));

        retval = procPtr->resolve(procSymExpr, resolvedActuals);

        if (retval == NULL)
        {
          INT_ASSERT(false);
        }
      }
      else
        INT_ASSERT(false);
    }
    else
      INT_ASSERT(false);
  }

  else if (callExpr->primitive != NULL)
  {
    retval = new IpeCallExpr(callExpr->primitive);

    for (int i = 1; i <= callExpr->numActuals(); i++)
    {
      Expr* expr     = callExpr->get(i);
      Expr* resolved = resolveExpr(expr, env);

      retval->insertAtTail(resolved);
    }
  }

  else
  {
    INT_ASSERT(false);
  }

  INT_ASSERT(retval);
  INT_ASSERT(dynamic_cast<IpeCallExpr*>(retval));

  return retval;
}
