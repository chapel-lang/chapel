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
#include "IpeVars.h"
#include "IpeModule.h"
#include "IpeProcedure.h"
#include "IpeScope.h"
#include "IpeScopeModule.h"
#include "IpeScopeProcedure.h"
#include "ipeDriver.h"
#include "ipeEvaluate.h"
#include "misc.h"
#include "stmt.h"
#include "VisibleSymbols.h"
#include "WhileDoStmt.h"

#include <cstdio>

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static Expr*    resolve(SymExpr*           expr, IpeScope* scope, IpeVars* vars);
static Expr*    resolve(UnresolvedSymExpr* expr, IpeScope* scope, IpeVars* vars);
static Expr*    resolve(DefExpr*           expr, IpeScope* scope, IpeVars* vars);
static Expr*    resolve(CallExpr*          expr, IpeScope* scope, IpeVars* vars);

static Expr*    resolve(BlockStmt*         expr, IpeScope* scope, IpeVars* vars);
static Expr*    resolve(CondStmt*          expr, IpeScope* scope, IpeVars* vars);
static Expr*    resolve(WhileDoStmt*       expr, IpeScope* scope, IpeVars* vars);

static Expr*    resolve(ModuleSymbol*      expr, IpeScope* scope, IpeVars* vars);

static Type*    typeForExpr(Expr* expr, IpeVars* vars);
static bool     isUseStmt(Expr* expr);
static DefExpr* findModuleDefinition(UnresolvedSymExpr* modSymExpr);

Expr* ipeResolve(Expr* expr, IpeScope* scope, IpeVars* vars)
{
  Expr* retval = NULL;

  if (gDebugLevelResolve > 0)
  {
    AstDumpToNode logger(stdout, 3);

    printf("ipeResolve\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");

    IpeVars::describe(vars, 3);
    printf("\n\n");
  }

  if (false)
    retval = NULL;

  else if (SymExpr*           sel = toSymExpr(expr))
    retval = resolve(sel, scope, vars);

  else if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr))
    retval = resolve(sel, scope, vars);

  else if (DefExpr*           sel = toDefExpr(expr))
    retval = resolve(sel, scope, vars);

  else if (CallExpr*          sel = toCallExpr(expr))
    retval = resolve(sel, scope, vars);

  else if (CondStmt*          sel = toCondStmt(expr))
    retval = resolve(sel, scope, vars);

  else if (WhileDoStmt*       sel = toWhileDoStmt(expr))
    retval = resolve(sel, scope, vars);

  // This must go after Loops etc
  else if (BlockStmt*         sel = toBlockStmt(expr))
    retval = resolve(sel, scope, vars);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   ipeResolve(Expr*, IpeScope*) unsupported\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);

    retval = expr;
  }

  return retval;
}

static Expr* resolve(SymExpr* expr, IpeScope* scope, IpeVars* vars)
{
  return expr;
}

static Expr* resolve(UnresolvedSymExpr* expr, IpeScope* scope, IpeVars* vars)
{
  VisibleSymbols symbols = scope->visibleSymbols(expr);
  Expr*          retval  = 0;

  if (symbols.count() == 1)
  {
    SET_LINENO(expr);

    retval = new SymExpr(symbols.symbol(0));
  }
  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   resolve(UnresolvedSymExpr*, IpeScope*). Failed to find a definition\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    printf("   symbols.count %d\n", symbols.count());

    scope->describe(3, true);

    INT_ASSERT(false);
  }

  return retval;
}

static Expr* resolve(DefExpr* defExpr, IpeScope* scope, IpeVars* vars)
{
  Expr* retval = 0;

  if (gDebugLevelResolve > 1)
  {
    AstDumpToNode logger(stdout, 3);

    printf("   resolve(DefExpr*,   IpeScope*, IpeVars*)\n");
    printf("   ");
    defExpr->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");

    IpeVars::describe(vars, 3);
    printf("\n\n");
  }

  if      (TypeSymbol*   sel    = toTypeSymbol(defExpr->sym))
  {
    IpeValue value;

    value.iValue = 0;

    scope->extend(sel, value, vars);

    retval = defExpr;
  }

  else if (ModuleSymbol* modSym = toModuleSymbol(defExpr->sym))
  {
    IpeModule*      module   = new IpeModule(modSym);
    IpeScopeModule* modScope = module->scope();
    VarSymbol*      var      = new VarSymbol(modSym->name, gIpeTypeModule);

    IpeValue        modValue;

    INT_ASSERT(defExpr->exprType == NULL);
    INT_ASSERT(defExpr->init     == NULL);

    var->addFlag(FLAG_PARAM);

    modValue.modulePtr = module;

    scope->extend(var, modValue, vars);

    resolve(modSym, modScope, vars);

    retval = defExpr;
  }

  else if (FnSymbol*     funSym = toFnSymbol(defExpr->sym))
  {
    IpeProcedure* procedure = new IpeProcedure(funSym, scope);
    VarSymbol*    var       = new VarSymbol(funSym->name, gIpeTypeProcedure);
    IpeValue      funValue;

    INT_ASSERT(defExpr->exprType == NULL);
    INT_ASSERT(defExpr->init     == NULL);

    var->addFlag(FLAG_PARAM);

    funValue.procedurePtr = procedure;

    scope->extend(var, funValue, vars);

    retval = defExpr;
  }

  else if (VarSymbol*    var    = toVarSymbol(defExpr->sym))
  {
    if (var->isImmediate() == false)
    {
      Type*    typeType      = 0;
      Type*    initType      = 0;
      IpeValue defaultValue;

      if (defExpr->exprType != 0)
      {
        Expr* resolvedExpr = ipeResolve(defExpr->exprType, scope, vars);

        INT_ASSERT(resolvedExpr);

        if (resolvedExpr != defExpr->exprType)
          defExpr->exprType->replace(resolvedExpr);

        typeType = typeForExpr(defExpr->exprType, vars);
        INT_ASSERT(typeType);
      }

      if (defExpr->init     != 0)
      {
        Expr* resolvedExpr = ipeResolve(defExpr->init,     scope, vars);

        INT_ASSERT(resolvedExpr);

        if (resolvedExpr != defExpr->init)
          defExpr->init->replace(resolvedExpr);

        initType = typeForExpr(defExpr->init, vars);
        INT_ASSERT(initType);
      }

      if      (typeType == 0 && initType == 0)
      {
        AstDumpToNode logger(stdout, 3);

        printf("resolve DefExpr.  Unexpected\n");
        printf("   ");
        defExpr->accept(&logger);
        printf("\n\n");

        INT_ASSERT(false);
      }

      else if (typeType == 0 && initType != 0)
        var->type = initType;

      else if (typeType != 0 && initType == 0)
        var->type = typeType;

      else if (typeType == initType)
        var->type = typeType;

      else
        INT_ASSERT(false);

      defaultValue = ipeEvaluate(var->type->defaultValue, NULL);

      scope->extend(var, defaultValue, vars);
    }
    else
    {
      IpeValue value;

      value.iValue = 0;

      scope->extend(var, value, vars);
    }

    retval = defExpr;
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   resolve(DefExpr*, IpeScope*)  unsupported\n");
    printf("   ");
    defExpr->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");

    INT_ASSERT(false);
    retval = 0;
  }

  return retval;
}

static Expr* resolve(ModuleSymbol* modSym, IpeScope* scope, IpeVars* vars)
{
  // Resolve use-statements and defExprs
  for_alist(expr, modSym->block->body)
  {
    if (isUseStmt(expr) == true)
    {
      CallExpr*  callExpr = toCallExpr(expr);
      Expr*      modExpr  = NULL;
      SymExpr*   modName  = NULL;
      VarSymbol* varSym   = NULL;
      IpeValue   value;

      ipeResolve(expr, scope, vars);

      modExpr = callExpr->get(1);
      modName = toSymExpr(modExpr);

      INT_ASSERT(modName);

      varSym = toVarSymbol(modName->var);
      INT_ASSERT(varSym);

      value  = IpeVars::fetch(varSym, vars);

      scope->useAdd(value.modulePtr);
    }

    else if (DefExpr* defExpr = toDefExpr(expr))
      ipeResolve(defExpr, scope, vars);
  }

  // Resolve the remaining expressions
  for_alist(expr, modSym->block->body)
  {
    if (isUseStmt(expr) == false && isDefExpr(expr) == false)
      ipeResolve(expr, scope, vars);
  }

  return NULL;
}

static Expr* resolve(BlockStmt* blockStmt, IpeScope* scope, IpeVars* vars)
{
  if (gDebugLevelResolve > 1)
  {
    AstDumpToNode logger(stdout, 3);

    printf("   resolve(BlockStmt*, IpeScope*, IpeVars*)\n");
    printf("   ");
    blockStmt->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");
  }

  if ((blockStmt->blockTag & BLOCK_SCOPELESS) == 0)
  {
    scope->envPush();
  }

  for_alist(stmt, blockStmt->body)
  {
    Expr* resolvedExpr = ipeResolve(stmt, scope, vars);

    if (resolvedExpr == 0)
    {
      AstDumpToNode logger(stdout, 3);

      printf("\n\n\n\n");
      printf("\n\n\n\n");
      printf("Internal error.  Failed to resolve\n");

      printf("   ");
      stmt->accept(&logger);
      printf("\n\n");

      printf("in\n\n");

      printf("   ");
      blockStmt->accept(&logger);
      printf("\n\n");

      INT_ASSERT(false);
    }

    if (resolvedExpr != stmt)
    {
      stmt->replace(resolvedExpr);
    }
  }

  if ((blockStmt->blockTag & BLOCK_SCOPELESS) == 0)
  {
    scope->envPop();
  }

  return blockStmt;
}

static Expr* resolve(CondStmt* stmt, IpeScope* scope, IpeVars* vars)
{
  ipeResolve(stmt->condExpr, scope, vars);
  ipeResolve(stmt->thenStmt, scope, vars);

  if (stmt->elseStmt != 0)
    ipeResolve(stmt->elseStmt, scope, vars);

  return stmt;
}

static Expr* resolve(WhileDoStmt* expr, IpeScope* scope, IpeVars* vars)
{
  ipeResolve(expr->condExprGet(), scope, vars);

  for_alist(stmt, expr->body)
  {
    Expr* resolvedExpr = ipeResolve(stmt, scope, vars);

    INT_ASSERT(resolvedExpr);

    if (resolvedExpr != stmt)
      stmt->replace(resolvedExpr);
  }

  return expr;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static Expr* selectFunc(UnresolvedSymExpr*  funName,
                        std::vector<Type*>& actualTypes,
                        IpeScope*           scope,
                        IpeVars*             vars);
static void  resolveFormalType(ArgSymbol* formal, IpeScopeProcedure* scope);

static Expr* resolve(CallExpr* callExpr, IpeScope* scope, IpeVars* vars)
{
  Expr* retval = NULL;

  if (isUseStmt(callExpr) == true)
  {
    UnresolvedSymExpr* modName = toUnresolvedSymExpr(callExpr->get(1));
    VisibleSymbols     symbols = scope->visibleSymbols(modName);

    // The module might not be loaded yet
    if (symbols.count() == 0)
    {
      // New modules should be resolved relative to root
      ipeResolve(findModuleDefinition(modName), gRootScope, vars);

      // Fetch the modName again. It will be found this time.
      symbols = scope->visibleSymbols(modName);
    }

    if (symbols.count() == 1)
    {
      Expr* expr = new SymExpr(symbols.symbol(0));

      callExpr->get(1)->replace(expr);
    }

    else
    {
      INT_ASSERT(false);
    }

    retval = callExpr;
  }

  else
  {
    int                count     = callExpr->numActuals();
    std::vector<Type*> actualTypes;

    for (int i = 1; i <= count; i++)
    {
      Expr* actual = callExpr->get(i);
      Expr* res    = ipeResolve(actual, scope, vars);
      Type* type   = typeForExpr(res, vars);

      if (res == 0 || type == 0)
      {
        AstDumpToNode logger(stdout, 3);

        printf("Failed to handle actual %2d\n   ", i);
        actual->accept(&logger);
        printf("\n\n");

        printf("in\n\n   ");
        callExpr->accept(&logger);
        printf("\n\n\n");

        if (callExpr->typeInfo() != 0)
        {
          printf("expr->typeInfo()\n\n   ");
          callExpr->typeInfo()->accept(&logger);
          printf("\n\n\n");
        }

        if (res  != 0)
        {
          printf("res:\n   ");
          res->accept(&logger);
          printf("\n\n\n");
        }

        if (type != 0)
        {
          printf("type:\n   ");
          type->accept(&logger);
          printf("\n\n\n");
        }

        INT_ASSERT(false);
      }
      else
      {
        actualTypes.push_back(type);

        if (res != actual)
          actual->replace(res);
      }
    }

    if (callExpr->primitive != NULL)
    {
      retval = callExpr;
    }

    // Select the function
    else if (UnresolvedSymExpr* baseExpr = toUnresolvedSymExpr(callExpr->baseExpr))
    {
      Expr* funcRef = selectFunc(baseExpr, actualTypes, scope, vars);

      INT_ASSERT(funcRef);
      baseExpr->replace(funcRef);

      retval = callExpr;
    }

    else
    {
      INT_ASSERT(false);
      retval = NULL;
    }
  }

  return retval;
}

static Expr* selectFunc(UnresolvedSymExpr*  funName,
                        std::vector<Type*>& actualTypes,
                        IpeScope*           scope,
                        IpeVars*             vars)
{
  VisibleSymbols visibleSymbols = scope->visibleSymbols(funName);
  VarSymbol*     varProcedure   = 0;
  Expr*          retval         = 0;

  for (int i = 0; i < visibleSymbols.count(); i++)
  {
    Symbol*    sym = visibleSymbols.symbol(i);
    VarSymbol* var = toVarSymbol(sym);

    if (var != 0 && var->type == gIpeTypeProcedure && var->depth() == 0)
    {
      IpeValue      value = IpeVars::fetch(var, vars);
      IpeProcedure* proc  = value.procedurePtr;

      if (proc->exactMatch(actualTypes) == true)
      {
        assert(varProcedure == 0);
        varProcedure = var;
      }
    }

    else
    {
      INT_ASSERT(false);
    }
  }

  INT_ASSERT(varProcedure != 0);

  retval = new SymExpr(varProcedure);

  if (retval == NULL)
  {
    AstDumpToNode logger(stdout, 3);

    printf("resolve.selectFunc.  Failed to find a function\n");
    printf("   ");
    funName->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

void ipeResolveFormalsTypes(IpeProcedure* procedure, IpeScopeProcedure* scope, IpeVars* vars)
{
  FnSymbol* fnSymbol = procedure->fnSymbol();

  if (gDebugLevelResolve > 1)
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n\n************\n");
    printf("   ipeResolveFormalsTypes Enter\n");
    printf("   ");
    fnSymbol->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");
  }

  for_alist(formal, fnSymbol->formals)
  {
    DefExpr* def = toDefExpr(formal);

    if (ArgSymbol* arg = toArgSymbol(def->sym))
    {
      if (arg->type == 0)
      {
        resolveFormalType(arg, scope);

        INT_ASSERT(arg->type);
      }
    }

    else
    {
      INT_ASSERT(false);
    }
  }

  if (gDebugLevelResolve > 1)
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n\n************\n");
    printf("   ipeResolveFormalsTypes Exit\n");
    printf("   ");
    fnSymbol->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");
  }
}

static void resolveFormalType(ArgSymbol* formal, IpeScopeProcedure* scope)
{
  BlockStmt*          bs      = formal->typeExpr;

  INT_ASSERT(bs);
  INT_ASSERT(bs->body.length == 1);

  UnresolvedSymExpr*  unres   = toUnresolvedSymExpr(bs->body.get(1));

  INT_ASSERT(unres);

  VisibleSymbols      symbols = scope->visibleSymbols(unres);

  if (symbols.count() > 0)
  {
    TypeSymbol* typeSym = toTypeSymbol(symbols.symbol(0));

    INT_ASSERT(typeSym);

    formal->type = typeSym->type;
  }
  else
  {
    AstDumpToNode logger(stdout);

    printf("resolveFormalType Failed to find a definition\n");
    unres->accept(&logger);
    printf("\n\n");
    INT_ASSERT(false);
  }
}

void ipeResolveBody(IpeProcedure* procedure, IpeScopeProcedure* scope, IpeVars* vars)
{
  FnSymbol* fnSymbol = procedure->fnSymbol();

  if (gDebugLevelResolve > 1)
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n\n************\n");
    printf("   ipeResolveBody Enter\n");
    printf("   ");
    fnSymbol->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");
  }

  for (int i = 1; i <= fnSymbol->formals.length; i++)
  {
    DefExpr*   formalDef = toDefExpr(fnSymbol->formals.get(i));
    ArgSymbol* formalArg = toArgSymbol(formalDef->sym);
    IpeValue   value;

    value.iValue = 0;

    scope->extend(formalArg, value, vars);
  }

  resolve(fnSymbol->body, scope, vars);

  if (gDebugLevelResolve > 1)
  {
    printf("   scope->maxFrameSize() = %d\n", scope->maxFrameSize());
  }

  procedure->frameSizeSet(scope->maxFrameSize());

  if (gDebugLevelResolve > 1)
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n\n************\n");
    printf("   ipeResolveBody Exit\n");
    printf("   ");
    fnSymbol->accept(&logger);
    printf("\n\n");

    scope->describe(3);
    printf("\n\n");
  }
}

void ipeResolveReturnType(IpeProcedure* procedure, IpeScopeProcedure* scope, IpeVars* vars)
{
  FnSymbol* fnSymbol = procedure->fnSymbol();

  if (fnSymbol->retExprType)
    resolve(fnSymbol->retExprType, scope, vars);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static Type* typeForExpr(Expr* expr, IpeVars* vars)
{
  Type* retval = 0;

  if (expr == 0)
  {
    retval = 0;
  }

  else if (SymExpr* value = toSymExpr(expr))
  {
    if (false)
      ;

    else if (TypeSymbol* type = toTypeSymbol(value->var))
      retval = type->type;

    // Var/Arg in user level modules and immediates
    else if (LcnSymbol*  sym  = toLcnSymbol(value->var))
      retval = sym->type;
  }

  else if (CallExpr* callExpr = toCallExpr(expr))
  {
    if (callExpr->primitive)
      retval = callExpr->primitive->returnInfo(callExpr);
    else
    {
#if 0
      AstDumpToNode logger(stdout, 3);
#endif

      SymExpr*   funExpr = toSymExpr(callExpr->baseExpr);
      INT_ASSERT(funExpr);

      VarSymbol* varSym  = toVarSymbol(funExpr->var);
      INT_ASSERT(varSym);

#if 0
      printf("   typeForExpr\n");
      printf("   ");
      varSym->accept(&logger);
      printf("\n\n");
#endif

      INT_ASSERT(varSym->type == gIpeTypeProcedure);

      IpeValue      proc      = IpeVars::fetch(varSym, vars);
      IpeProcedure* procedure = proc.procedurePtr;
      FnSymbol*     fnSym     = procedure->fnSymbol();

#if 0
      printf("   Before resolving the body\n");
      printf("   ");
      fnSym->accept(&logger);
      printf("\n\n");
#endif

      procedure->ensureBodyResolved();

#if 0
      printf("   After  resolving the body\n");
      printf("   ");
      fnSym->accept(&logger);
      printf("\n\n");
#endif


      INT_ASSERT(fnSym);

      BlockStmt* bs    = fnSym->retExprType;
      INT_ASSERT(bs);
      INT_ASSERT(bs->length() == 1);

      retval = typeForExpr(bs->body.only(), vars);
    }
  }

  else
    retval = 0;

  return retval;
}

static bool isUseStmt(Expr* expr)
{
  bool retval = false;

  if (CallExpr* callExpr = toCallExpr(expr))
    retval = callExpr->isPrimitive(PRIM_USE);

  return retval;
}

// Scan rootModule to find the DefExpr for the desired module
static DefExpr* findModuleDefinition(UnresolvedSymExpr* modExpr)
{
  const char* name   = modExpr->unresolved;
  DefExpr*    retval = NULL;

  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* sym = toModuleSymbol(defExpr->sym))
      {
        if (strcmp(sym->name, name) == 0)
        {
          retval = defExpr;
          break;
        }
      }
    }
  }

  return retval;
}

