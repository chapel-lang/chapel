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
#include "ipe.h"
#include "ScopeBlock.h"
#include "ScopeFunction.h"
#include "ScopeModule.h"
#include "stmt.h"
#include "symbol.h"
#include "VisibleSymbol.h"
#include "WhileDoStmt.h"

static void  resolve(ModuleSymbol*      module);

static Expr* resolve(Expr*              expr,   const ScopeBase* scope);

static Expr* resolve(FnSymbol*          fn,     const ScopeBase* scope);

static Expr* resolve(BlockStmt*         expr,   const ScopeBase* scope);
static Expr* resolve(CondStmt*          expr,   const ScopeBase* scope);
static Expr* resolve(WhileDoStmt*       expr,   const ScopeBase* scope);

static Expr* resolve(DefExpr*           expr,   const ScopeBase* scope);
static Expr* resolve(SymExpr*           expr,   const ScopeBase* scope);
static Expr* resolve(UnresolvedSymExpr* expr,   const ScopeBase* scope);
static Expr* resolve(CallExpr*          expr,   const ScopeBase* scope);

static Type* typeForExpr(Expr* expr);

static Expr* selectFunc(Expr*               funName,
                        std::vector<Type*>& actualTypes,
                        const ScopeBase*    scope);

static void resolveFuncFormals(FnSymbol*        fn,
                               const ScopeBase* scope);

/************************************ | *************************************
*                                                                           *
* The entry point for the IPE's stripped down version of type and function  *
* resolution.                                                               *
*                                                                           *
************************************* | ************************************/

void ipeResolve()
{
  // Scan the Root Module Declaration
  //   Every expression should be a DefExpr.
  //   Process every User module
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        if (module->modTag == MOD_USER)
          resolve(module);
      }
    }
    else
      INT_ASSERT(false);
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void resolve(ModuleSymbol* module)
{
  ScopeModule* scope = new ScopeModule(module);

  for_alist(stmt, module->block->body)
    resolve(stmt, scope);

  delete scope;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static Expr* resolve(Expr* expr, const ScopeBase* scope)
{
  Expr* retval = 0;

  if (false)
    ;

  else if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr))
    retval = resolve(sel, scope);

  else if (SymExpr*           sel = toSymExpr(expr))
    retval = resolve(sel, scope);

  else if (DefExpr*           sel = toDefExpr(expr))
    retval = resolve(sel, scope);

  else if (CallExpr*          sel = toCallExpr(expr))
    retval = resolve(sel, scope);

  else if (FnSymbol*          sel = toFnSymbol(expr))
    retval = resolve(sel, scope);

  else if (WhileDoStmt*       sel = toWhileDoStmt(expr))
    retval = resolve(sel, scope);

  else if (CondStmt*          sel = toCondStmt(expr))
    retval = resolve(sel, scope);

  // This must come after WhileDoStmt etc
  else if (BlockStmt*         sel = toBlockStmt(expr))
    retval = resolve(sel, scope);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("resolve Unhandled expr\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

static Expr* resolve(FnSymbol* fn, const ScopeBase* parent)
{
  ScopeFunction* scope = new ScopeFunction(fn, parent);

  resolveFuncFormals(fn, scope);

  if (fn->body)
    resolve(fn->body,        scope);

  if (fn->retExprType)
    resolve(fn->retExprType, scope);

  delete scope;

  return 0;
}

static Expr* resolve(BlockStmt* blockStmt, const ScopeBase* parent)
{
  ScopeBase*       frame =   NULL;
  const ScopeBase* scope = parent;

  if ((blockStmt->blockTag & BLOCK_SCOPELESS) == 0)
  {
    frame = new ScopeBlock(blockStmt, parent);
    scope = frame;
  }

  for_alist(stmt, blockStmt->body)
  {
    Expr* resolvedExpr = resolve(stmt, scope);

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
      stmt->replace(resolvedExpr);
  }

  if (frame != NULL)
    delete frame;

  return blockStmt;
}

static Expr* resolve(CondStmt* stmt, const ScopeBase* scope)
{
  resolve(stmt->condExpr, scope);
  resolve(stmt->thenStmt, scope);

  if (stmt->elseStmt != 0)
    resolve(stmt->elseStmt, scope);

  return stmt;
}

static Expr* resolve(WhileDoStmt* expr, const ScopeBase* scope)
{
  resolve(expr->condExprGet(), scope);

  for_alist(stmt, expr->body)
  {
    Expr* resolvedExpr = resolve(stmt, scope);

    INT_ASSERT(resolvedExpr);

    if (resolvedExpr != stmt)
      stmt->replace(resolvedExpr);
  }

  return expr;
}

static Expr* resolve(DefExpr* defExpr, const ScopeBase* scope)
{
  Type* typeType = 0;
  Type* initType = 0;

  if (defExpr->sym      != 0)
  {
    if (FnSymbol* fnSymbol = toFnSymbol(defExpr->sym))
      resolve(fnSymbol, scope);
  }

  if (defExpr->exprType != 0)
  {
    Expr* resolvedExpr = resolve(defExpr->exprType, scope);

    INT_ASSERT(resolvedExpr);

    if (resolvedExpr != defExpr->exprType)
      defExpr->exprType->replace(resolvedExpr);

    typeType = typeForExpr(defExpr->exprType);

    INT_ASSERT(typeType);
  }

  if (defExpr->init     != 0)
  {
    Expr* resolvedExpr = resolve(defExpr->init, scope);

    INT_ASSERT(resolvedExpr);

    if (resolvedExpr != defExpr->init)
      defExpr->init->replace(resolvedExpr);

    initType = typeForExpr(defExpr->init);

    INT_ASSERT(initType);
  }

  if (typeType == 0 && initType == 0)
  {
    // It would be a surprise if the parser generated this case
    if (isFnSymbol(defExpr->sym) == false)
    {
      AstDumpToNode logger(stdout, 3);

      printf("resolve DefExpr.  Unexpected\n   ");
      defExpr->accept(&logger);
      printf("\n\n");

      INT_ASSERT(false);
    }
  }

  else if (typeType == 0 && initType != 0)
    defExpr->sym->type = initType;

  else if (typeType != 0 && initType == 0)
    defExpr->sym->type = typeType;

  else
  {
    if (typeType == initType)
      defExpr->sym->type = typeType;
    else
      INT_ASSERT(false);
  }

  if (defExpr->exprType != 0)
    defExpr->exprType->remove();

  return defExpr;
}

static Expr* resolve(SymExpr* expr, const ScopeBase* scope)
{
  return expr;
}

static Expr* resolve(UnresolvedSymExpr* expr, const ScopeBase* scope)
{
  std::vector<VisibleSymbol> symbols;
  Expr*                      retval = 0;

  scope->visibleSymbols(expr, symbols);

  if (symbols.size() == 0)
  {
    AstDumpToNode logger(stdout, 3);

    printf("resolve UnresolvedSymExpr. Failed to find a definition\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }
  else
  {
    SET_LINENO(expr);

    retval = new SymExpr(symbols[0].symbol());
  }

  return retval;
}

static Expr* resolve(CallExpr* expr, const ScopeBase* scope)
{
  bool               isUseStmt = expr->isPrimitive(PRIM_USE);
  int                count     = expr->numActuals();
  std::vector<Type*> actualTypes;

  // Resolve the actuals
  for (int i = 1; i <= count; i++)
  {
    Expr* actual = expr->get(i);
    Expr* res    = resolve(actual, scope);
    Type* type   = typeForExpr(res);

    // Do not currently have a type for Module
    if (res == 0 || (isUseStmt == false && type == 0))
    {
      AstDumpToNode logger(stdout, 3);

      printf("Failed to handle actual %2d\n   ", i);
      actual->accept(&logger);
      printf("\n\n");

      printf("in\n\n   ");
      expr->accept(&logger);
      printf("\n\n\n");

      if (expr->typeInfo() != 0)
      {
        printf("expr->typeInfo()\n\n   ");
        expr->typeInfo()->accept(&logger);
        printf("\n\n\n");
      }

      if (res != 0)
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

    actualTypes.push_back(type);

    if (res != actual)
      actual->replace(res);
  }

  // Select the function
  if (UnresolvedSymExpr* baseExpr = toUnresolvedSymExpr(expr->baseExpr))
  {
    Expr*     funcRef = selectFunc(baseExpr, actualTypes, scope);
    FnSymbol* func    = 0;

    INT_ASSERT(funcRef);
    baseExpr->replace(funcRef);

    if (SymExpr* symExpr = toSymExpr(funcRef))
      func = toFnSymbol(symExpr->var);

    INT_ASSERT(func);
    INT_ASSERT(func->formals.length == count);
  }

  // Remove Use Statements from the tree
  if (isUseStmt == true)
  {
    expr->remove();
    expr = 0;
  }

  return expr;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static Type* typeForExpr(Expr* expr)
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
      SymExpr* funExpr = toSymExpr(callExpr->baseExpr);
      INT_ASSERT(funExpr);

      FnSymbol*  fnSym = toFnSymbol(funExpr->var);
      INT_ASSERT(fnSym);

      BlockStmt* bs    = fnSym->retExprType;
      INT_ASSERT(bs);
      INT_ASSERT(bs->length() == 1);

      retval = typeForExpr(bs->body.only());
    }
  }

  else
    retval = 0;

  return retval;
}

/************************************ | *************************************
*                                                                           *
* A drastically simplified form of Function Resolution for IPE.             *
*                                                                           *
************************************* | ************************************/

static void resolveFormalType(ArgSymbol* arg, const ScopeBase* scope);

static bool ipeFunctionExactMatch(FnSymbol*           fn,
                                  std::vector<Type*>& actualTypes);

static Expr* selectFunc(Expr*               funName,
                        std::vector<Type*>& actualTypes,
                        const ScopeBase*    scope)
{
  Expr* retval = 0;

  if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(funName))
  {
    std::vector<VisibleSymbol> visibleSymbols;

    scope->visibleSymbols(sel, visibleSymbols);

    // Ensure we know the type of every formal
    for (size_t i = 0; i < visibleSymbols.size(); i++)
    {
      if (FnSymbol* fn = toFnSymbol(visibleSymbols[i].symbol()))
      {
        resolveFuncFormals(fn, scope);
      }
    }

    // Select a function if there is an exact match
    for (size_t i = 0; i < visibleSymbols.size() && retval == 0; i++)
    {
      if (FnSymbol* fn = toFnSymbol(visibleSymbols[i].symbol()))
      {
        if (ipeFunctionExactMatch(fn, actualTypes) == true)
        {
          SET_LINENO(funName);

          retval = new SymExpr(fn);

          resolve(fn, visibleSymbols[i].scope());
        }
      }
    }

    if (retval == 0)
    {
      AstDumpToNode logger(stdout, 3);

      printf("resolve.selectFunc.  Failed to find a function\n");
      printf("   ");
      funName->accept(&logger);
      printf("\n\n\n");

      INT_ASSERT(false);
    }
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("selectFunc   Unhandled\n");
    printf("   ");
    funName->accept(&logger);
    printf("\n\n\n");
  }

  return retval;
}

static void resolveFuncFormals(FnSymbol* fn, const ScopeBase* scope)
{
  for_alist(formal, fn->formals)
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
}

static void resolveFormalType(ArgSymbol* formal, const ScopeBase* scope)
{
  BlockStmt*                 bs    = formal->typeExpr;

  INT_ASSERT(bs);
  INT_ASSERT(bs->body.length == 1);

  UnresolvedSymExpr*         unres = toUnresolvedSymExpr(bs->body.get(1));

  INT_ASSERT(unres);

  std::vector<VisibleSymbol> symbols;

  scope->visibleSymbols(unres, symbols);

  if (symbols.size() == 0)
  {
    AstDumpToNode logger(stdout);

    printf("resolveFormalType Failed to find a definition\n");
    unres->accept(&logger);
    printf("\n\n");
    INT_ASSERT(false);
  }
  else
  {
    TypeSymbol* typeSym = toTypeSymbol(symbols[0].symbol());

    INT_ASSERT(typeSym);

    formal->type = typeSym->type;
    formal->typeExpr->remove();
  }
}

static bool ipeFunctionExactMatch(FnSymbol*           fn,
                                  std::vector<Type*>& actualsTypes)
{
  bool retval = false;

  if (fn->formals.length == (int) actualsTypes.size())
  {
    bool match = true;

    for (size_t i = 0; i < actualsTypes.size() && match == true; i++)
    {
      DefExpr*   expr = toDefExpr(fn->formals.get(i + 1));
      INT_ASSERT(expr);

      ArgSymbol* arg  = toArgSymbol(expr->sym);

      INT_ASSERT(arg);

      match = (actualsTypes[i] == arg->type) ? true : false;
    }

    retval = match;
  }

  return retval;
}
