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

#include "IpeBlockStmt.h"
#include "IpeCallExpr.h"
#include "IpeDefExpr.h"
#include "IpeEnv.h"
#include "IpeMethod.h"
#include "IpeModuleRoot.h"
#include "IpeProcedure.h"
#include "IpeSequence.h"
#include "IpeScopeBlock.h"
#include "IpeValue.h"

#include "ipeDriver.h"
#include "ipeEvaluate.h"
#include "ipeUtils.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "stmt.h"
#include "WhileDoStmt.h"

#include <cstdio>

static SymExpr*     resolveSymExpr    (SymExpr*           expr, IpeEnv* env);
static SymExpr*     resolveUnresExpr  (UnresolvedSymExpr* expr, IpeEnv* env);

static IpeDefExpr*  resolveDefExpr    (DefExpr*           expr, IpeEnv* env);

static CondStmt*    resolveCondStmt   (CondStmt*          expr, IpeEnv* env);
static WhileDoStmt* resolveWhileDoStmt(WhileDoStmt*       expr, IpeEnv* env);

static IpeSequence* resolveBlockStmt  (BlockStmt*         expr, IpeEnv* env);

static IpeCallExpr* resolveCallExpr   (CallExpr*          expr, IpeEnv* env);

Expr* resolveExpr(Expr* expr, IpeEnv* env)
{
  Expr* retval = NULL;

#if 0
  AstDumpToNode logger(stdout, 3);

  printf("\n\nresolveExpr Enter\n");
  printf("   ");
  expr->accept(&logger);
  printf("\n\n");
#endif

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

#if 0
  printf("\n\nresolveExpr Exit\n");
  printf("   ");
  expr->accept(&logger);
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#endif

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static SymExpr* resolveSymExpr(SymExpr* expr, IpeEnv* env)
{
  return expr->copy();
}

static SymExpr* resolveUnresExpr(UnresolvedSymExpr* expr, IpeEnv* env)
{
  SymExpr* retval = NULL;

  if (LcnSymbol* varSym = env->findVariable(expr->unresolved))
  {
    SET_LINENO(expr);

    retval = new SymExpr(varSym);
  }

  else
  {
    printf("   resolveExpr(UnresolvedSymExpr*, IpeEnv* env) "
           "Failed to find a variable for %s\n",
           expr->unresolved);
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static IpeDefExpr* resolveDefVar   (DefExpr*      expr, IpeEnv* env);
static IpeDefExpr* resolveDefModule(ModuleSymbol* mod,  IpeEnv* env);
static IpeDefExpr* resolveDefProc  (FnSymbol*     fn,   IpeEnv* env);

static IpeDefExpr* resolveDefExpr(DefExpr* defExpr, IpeEnv* env)
{
  SET_LINENO(defExpr);

  IpeDefExpr* retval = 0;

  if      (isVarSymbol(defExpr->sym))
  {
    retval = resolveDefVar(defExpr, env);
  }

  else if (ModuleSymbol* mod = toModuleSymbol(defExpr->sym))
    retval = resolveDefModule(mod, env);

  else if (FnSymbol*     fn  = toFnSymbol(defExpr->sym))
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
  int         depth  = env->depth();
  VarSymbol*  var    = toVarSymbol(defExpr->sym);
  const char* name   = var->name;
  IpeDefExpr* retval = NULL;

  if (depth == 0 && env->findLocal(name) !=  NULL)
  {
    printf("   Attempt to redefine \"%s\" in\n", name);
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

    if (env->depth() == 0)
      env->varAdd(var);

    retval = new IpeDefExpr(var, initExpr, exprTypeExpr);
  }

  return retval;
}

static IpeDefExpr* resolveDefModule(ModuleSymbol* mod,  IpeEnv* env)
{
  LcnSymbol* procLcn = env->findLocal(mod->name);

  if (procLcn == NULL)
  {
    VarSymbol* modVar = new VarSymbol(mod->name);
    IpeModule* module = IpeModuleRoot::create(mod);
    int        offset = env->allocateValue(module);

    modVar->addFlag(FLAG_CONST);
    modVar->type = gIpeTypeModule;
    modVar->locationSet(env->depth(), offset);

    env->varAdd(modVar);

    procLcn = modVar;
  }
  else
  {
    INT_ASSERT(procLcn->type == gIpeTypeModule);
  }

  return new IpeDefExpr(procLcn, mod);
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
    procVar->locationSet(env->depth(), offset);

    env->varAdd(procVar);

    procLcn = procVar;
  }
  else
  {
    INT_ASSERT(procLcn->type == gIpeTypeProcedure);
  }

  return new IpeDefExpr(procLcn, fn);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

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

static WhileDoStmt* resolveWhileDoStmt(WhileDoStmt* stmt, IpeEnv* env)
{
#if 0
  AstDumpToNode logger(stdout, 3);

  printf("\n\n\n\nresolveWhileDoStmt Enter\n");

  printf("   ");
  stmt->accept(&logger);
  printf("\n\n");

  env->describe(3);
  printf("\n\n");
#endif

  SET_LINENO(stmt);

  Expr*        cond   = stmt->condExprGet();
  Expr*        body   = stmt->body.get(1);
  WhileDoStmt* retval = NULL;

  INT_ASSERT(stmt->body.length == 1);
  INT_ASSERT(isBlockStmt(body));

  retval = new WhileDoStmt(resolveExpr(cond, env),
                           toBlockStmt(resolveExpr(body, env)));


#if 0
  printf("\n\n\n\nresolveWhileDoStmt Exit\n");
  printf("   ");
  retval->accept(&logger);
  printf("\n\n");
#endif


  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static bool         blockRequiresScope(BlockStmt* stmt);

static int          blockEstimateFrameSize(BlockStmt* stmt);
static IpeSequence* blockResolve(BlockStmt* stmt,
                                 IpeEnv*    env,
                                 bool       genBlockStmt);
static bool         blockCollectLocals  (BlockStmt* stmt, IpeEnv* env);
static bool         blockProcessUseStmts(BlockStmt* stmt, IpeEnv* env);

static bool         blockCollectResolved(BlockStmt*          stmt,
                                         IpeEnv*             env,
                                         std::vector<Expr*>& resolved);

IpeSequence* blockResolve(BlockStmt* stmt, IpeEnv* env)
{
  return blockResolve(stmt, env, false);
}

static IpeSequence* resolveBlockStmt(BlockStmt* stmt, IpeEnv* env)
{
  static int sCount = 0;
  int        count  = sCount;

  sCount = sCount + 1;

  IpeSequence* retval = NULL;

  SET_LINENO(stmt);

  AstDumpToNode logger(stdout, 3);

#if 0
  printf("\n\n\n\n\n\n\n");
  printf("   resolveBlockStmt           00100 %3d\n", count);
  printf("   ");
  stmt->accept(&logger);
  printf("\n\n");
#else
  (void) count;
#endif

  if (blockRequiresScope(stmt) == true)
  {
    int            frameSize = blockEstimateFrameSize(stmt);
    IpeScopeBlock* scope     = new IpeScopeBlock(env->scopeGet());
    IpeEnv*        blockEnv  = new IpeEnv(env, scope, frameSize);

    retval = blockResolve(stmt, blockEnv, true);

    INT_ASSERT(retval != NULL);
  }

  else
  {
    std::vector<Expr*> resolved;

    if (blockCollectResolved(stmt, env, resolved) == false)
      retval = NULL;
    else
      retval = new IpeSequence(resolved);
  }

#if 0
  printf("   resolveBlockStmt           00900 %3d\n", count);
  retval->describe(3);
  printf("\n\n");
#endif

  return retval;
}

static bool blockRequiresScope(BlockStmt* stmt)
{
  bool retval = false;

  if (stmt->isScopeless() == false)
  {
    for (int i = 1; i <= stmt->body.length && retval == false; i++)
    {
      Expr* expr = stmt->body.get(i);

      retval = (isDefExpr(expr) == true || isUseStmt(expr)) ? true : false;
    }
  }

  return retval;
}

static int blockEstimateFrameSize(BlockStmt* stmt)
{
  int delta  = 8;   // NOAKES: Currently all variables are 8 bytes
  int retval = 0;

  for (int i = 1; i <= stmt->body.length; i++)
  {
    Expr* expr = stmt->body.get(i);

    if (isDefExpr(expr) == true)
    {
      retval = retval + delta;
    }

    else if (BlockStmt* bs = toBlockStmt(expr))
    {
      if (bs->isScopeless() == true)
        retval = retval + blockEstimateFrameSize(bs);
    }
  }

  return retval;
}

static IpeSequence* blockResolve(BlockStmt* stmt,
                                 IpeEnv*    env,
                                 bool       genBlockStmt)
{
  static int sCount = 0;
  int count = sCount;

  sCount = sCount + 1;

  std::vector<Expr*> resolved;
  IpeSequence*       retval = NULL;

#if 0
  AstDumpToNode logger(stdout, 3);

  printf("\n\n\n\n\n");
  printf("   blockResolve(stmt, env, %s)           00100 %5d\n",
         (genBlockStmt) ? "true" : "false",
         count);

  printf("   ");
  stmt->accept(&logger);
  printf("\n\n");
#else
  (void) count;
#endif

  if      (blockCollectLocals(stmt, env)             == false)
    retval = NULL;

  else if (blockProcessUseStmts(stmt, env)           == false)
    retval = NULL;

  else if (blockCollectResolved(stmt, env, resolved) == false)
    retval = NULL;

  else if (genBlockStmt == false)
    retval = new IpeSequence(resolved);

  else
    retval = new IpeBlockStmt(resolved, env);

#if 0
  printf("   blockResolve(stmt, env, %s)           00900 %5d\n",
         (genBlockStmt) ? "true" : "false",
         count);
  retval->describe(3);
  printf("\n");
#endif

  return retval;
}

// Return false if an error occurs while collecting definitions
static bool blockCollectLocals(BlockStmt* stmt, IpeEnv* env)
{
  bool retval = true;

  for (int i = 1; i <= stmt->body.length && retval == true; i++)
  {
    Expr* expr = stmt->body.get(i);

    if (DefExpr* defExpr = toDefExpr(expr))
    {
      Symbol*     sym  = defExpr->sym;
      const char* name = sym->name;

      if      (VarSymbol* varSym = toVarSymbol(sym))
      {
        if (env->findLocal(name) == NULL)
        {
          env->varAdd(varSym);
        }
        else
        {
          printf("Attempt to redefine %s\n", name);
          retval = false;
        }
      }

      else if (FnSymbol*  fnSym  = toFnSymbol(sym))
      {
        LcnSymbol*    lcn  = env->findLocal(name);
        VarSymbol*    var  = toVarSymbol(lcn);
        IpeProcedure* proc = NULL;

        if (lcn == NULL)
        {
          proc      = new IpeProcedure(name);

          var       = new VarSymbol(name);
          var->type = gIpeTypeProcedure;
          var->locationSet(env->depth(), env->allocateValue(proc));

          env->varAdd(var);
        }

        if (var->type == gIpeTypeProcedure)
        {
          if (proc == NULL)
            proc = (IpeProcedure*) env->fetchPtr(var);

          proc->methodAdd(new IpeMethod(fnSym, env));
        }
        else
        {
          printf("Attempt to redefine %s\n", name);
          retval = false;
        }
      }

      else
      {
        AstDumpToNode logger(stdout, 3);

        printf("   blockCollectLocals(BLockStmt*, IpeEnv*)  incomplete\n");
        printf("   ");
        defExpr->accept(&logger);
        printf("\n\n");
        INT_ASSERT(false);
      }
    }

    else if (BlockStmt* bs = toBlockStmt(expr))
    {
      if (bs->isScopeless() == true)
        blockCollectLocals(bs, env);
    }
  }

  return retval;
}

static bool blockProcessUseStmts(BlockStmt* stmt, IpeEnv* env)
{
  bool retval = true;

  for (int i = 1; i <= stmt->body.length && retval == true; i++)
  {
    Expr* expr = stmt->body.get(i);

    if (isUseStmt(expr) == true)
    {
      Expr*              modName = toCallExpr(expr)->get(1);
      UnresolvedSymExpr* sel     = toUnresolvedSymExpr(modName);
      LcnSymbol*         sym     = env->findVariable(sel->unresolved);

      if (sym != NULL && sym->type == gIpeTypeModule && sym->offset() >= 0)
      {
        IpeModule* module = (IpeModule*) env->fetchPtr(sym);

        module->moduleResolve();
        env->useAdd(module);
      }
      else
      {
        INT_ASSERT(sym           != NULL);
        INT_ASSERT(sym->type     == gIpeTypeModule);
        INT_ASSERT(sym->offset() >= 0);

        retval = false;
      }
    }
  }

  return retval;
}

static bool blockCollectResolved(BlockStmt*          stmt,
                                 IpeEnv*             env,
                                 std::vector<Expr*>& resolved)
{
  bool retval = true;

  for (int i = 1; i <= stmt->body.length && retval == true; i++)
  {
    Expr* expr = stmt->body.get(i);

    if (isUseStmt(expr) == false)
    {
      if (Expr* exprRes = resolveExpr(expr, env))
        resolved.push_back(exprRes);
      else
        retval = false;
    }
  }

  return retval;
}

#if 0
static void fooBarExpr(ModuleSymbol* modSym, IpeEnv* env)
{
  BlockStmt* block = modSym->block;

  for_alist(expr, block->body)
  {
    if (DefExpr* defExpr = toDefExpr(expr))
    {
      const char* identifier = defExpr->sym->name;

      if (env->findLocal(identifier) == NULL)
        env->varAdd(new VarSymbol(identifier));
    }
  }

  for_alist(expr, block->body)
  {
    if (isUseStmt(expr) == true)
    {
      Expr*              modName = toCallExpr(expr)->get(1);
      UnresolvedSymExpr* sel     = toUnresolvedSymExpr(modName);
      LcnSymbol*         modSym  = env->findVariable(sel);
      IpeModule*         module  = NULL;

      INT_ASSERT(modSym->type     == gIpeTypeModule);
      INT_ASSERT(modSym->offset() >= 0);

      module = (IpeModule*) env->fetchPtr(modSym);
      module->moduleResolve();
      env->useAdd(module);
    }
  }

  for_alist(expr, block->body)
  {
    if (DefExpr* defExpr = toDefExpr(expr))
    {
      Symbol*    sym = defExpr->sym;
      LcnSymbol* var = env->findLocal(sym->name);

      if (FnSymbol* fnSymbol = toFnSymbol(sym))
      {
        if (var->type == dtUnknown)
        {
          INT_ASSERT(env->findVariable(sym->name) == var);

          IpeProcedure* procedure = new IpeProcedure(sym->name);
          int           offset    = env->allocateValue(procedure);

          var->type = gIpeTypeProcedure;
          var->locationSet(env->depth(), offset);
        }

        if (var->type == gIpeTypeProcedure)
        {
          IpeProcedure* procedure = (IpeProcedure*) env->fetchPtr(var);
          IpeMethod*    method    = new IpeMethod(fnSymbol, env);

          procedure->methodAdd(method);
        }
        else
          printf("Error: The variable %s is already defined as a non-procedure\n", sym->name);
      }

      else
        printf("Error: The variable %s is already defined\n", sym->name);
    }
  }
}
#endif

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static IpeCallExpr* resolveCallExpr(CallExpr* callExpr, IpeEnv* env)
{
  IpeCallExpr* retval = NULL;

  if      (callExpr->baseExpr  != NULL)
  {
    if (SymExpr* procSymExpr = toSymExpr(resolveExpr(callExpr->baseExpr, env)))
    {
      if (procSymExpr->typeInfo() == gIpeTypeProcedure)
      {
        IpeValue           procValue = env->fetch(toLcnSymbol(procSymExpr->var));
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
