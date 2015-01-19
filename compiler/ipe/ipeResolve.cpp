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
#include "DefScope.h"
#include "expr.h"
#include "ipe.h"
#include "stmt.h"
#include "symbol.h"
#include "VisibleSymbol.h"
#include "WhileDoStmt.h"

static void  rootScopeInit(DefScope* scope);

static Expr* resolve(ModuleSymbol*      module, const DefScope* scope);
static Expr* resolve(FnSymbol*          fn,     const DefScope* scope);

static Expr* resolve(BlockStmt*         expr,   const DefScope* scope);
static Expr* resolve(CondStmt*          expr,   const DefScope* scope);
static Expr* resolve(WhileDoStmt*       expr,   const DefScope* scope);

static Expr* resolve(Expr*              expr,   const DefScope* scope);
static Expr* resolve(DefExpr*           expr,   const DefScope* scope);
static Expr* resolve(SymExpr*           expr,   const DefScope* scope);
static Expr* resolve(UnresolvedSymExpr* expr,   const DefScope* scope);
static Expr* resolve(CallExpr*          expr,   const DefScope* scope);

static Type* typeForExpr(Expr* expr);

static bool  blockCreatesScope(BlockStmt* block);

static Expr* selectFunc(Expr*               funName,
                        std::vector<Type*>& actualTypes,
                        const DefScope*     scope);

static int   sRootModuleIndex = 0;

/************************************ | *************************************
*                                                                           *
* IPE needs to extract core definitions from the Root Module but this       *
* module is constantly updated.  We arrange for the core initialization     *
* routine to call this function so that it can establish a high water mark. *
*                                                                           *
************************************* | ************************************/

void ipeRootInit()
{
  sRootModuleIndex = rootModule->block->body.length;
}

/************************************ | *************************************
*                                                                           *
* The entry point for the IPE's stripped down version of type and function  *
* resolution.                                                               *
*                                                                           *
************************************* | ************************************/

void ipeResolve()
{
  DefScope* scopeBase = DefScope::extend(NULL);

  rootScopeInit(scopeBase);

  // Scan the Root Module Declaration
  //   Every expression should be a DefExpr.
  //   Process every Top Level Module Declaration
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        resolve(module, scopeBase);
      }
    }
    else
      INT_ASSERT(false);
  }
}

/************************************ | *************************************
*                                                                           *
* We combine DefExprs in _root and ChapelBase in to a single DefScope       *
*                                                                           *
************************************* | ************************************/

static void rootScopeInit(DefScope* scope)
{
  // Append the first N slots in rootModule
  for (int i = 1; i <= sRootModuleIndex; i++)
  {
    if (DefExpr* expr = toDefExpr(rootModule->block->body.get(i)))
      scope->addDefinition(expr);
    else
      INT_ASSERT(false);
  }

  // Any top level module definitions in rootModule
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* expr = toDefExpr(stmt))
    {
      if (isModuleSymbol(expr->sym) == true)
        scope->addDefinition(expr);
    }
  }

  // All definitions in ChapelBase
  scope->extendByModule(baseModule);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static Expr* resolve(Expr* expr, const DefScope* scope)
{
  Expr* retval = 0;

  if (DefExpr* sel = toDefExpr(expr))
  {
    resolve(sel, scope);
    retval = 0;
  }

  else if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr))
    retval = resolve(sel, scope);

  else if (SymExpr*           sel = toSymExpr(expr))
    retval = resolve(sel, scope);

  else if (CallExpr*          sel = toCallExpr(expr))
    retval = resolve(sel, scope);

  else if (WhileDoStmt*       sel = toWhileDoStmt(expr))
    retval = resolve(sel, scope);

  else if (BlockStmt*         sel = toBlockStmt(expr))
    retval = resolve(sel, scope);

  else if (CondStmt*          sel = toCondStmt(expr))
    retval = resolve(sel, scope);

  else
  {
    AstDumpToNode logger(stdout);

    printf("resolve Unhandled expr\n");
    expr->accept(&logger);
    printf("\n\n\n");
    INT_ASSERT(false);
  }

  return retval;
}

static Expr* resolve(ModuleSymbol* module, const DefScope* parent)
{
  BlockStmt*      moduleBody = module->block;
  const DefScope* scope      = 0;

  if (blockCreatesScope(moduleBody) == true)
  {
    DefScope* newScope = DefScope::extend(parent);

    // Extend scope with the top-level DefExprs
    for_alist(stmt, moduleBody->body)
    {
      if (DefExpr* expr = toDefExpr(stmt))
        newScope->addDefinition(expr);
    }

    scope = newScope;
  }
  else
  {
    scope = parent;
  }

  // Resolve every statement in the block
  for_alist(stmt, moduleBody->body)
  {
    resolve(stmt, scope);
  }

  return 0;
}

static Expr* resolve(FnSymbol* fn, const DefScope* parent)
{
  const DefScope* scope = 0;

  if (fn->formals.length > 0)
  {
    DefScope* formalsScope = DefScope::extend(parent);

    for (int i = 1; i <= fn->formals.length; i++)
    {
      DefExpr*   defExpr = 0;
      ArgSymbol* formal  = 0;

      defExpr = toDefExpr(fn->formals.get(i));
      INT_ASSERT(defExpr);

      formal  = toArgSymbol(defExpr->sym);
      INT_ASSERT(formal);
      INT_ASSERT(formal->type);

      formalsScope->addDefinition(defExpr);
    }

    scope = formalsScope;
  }
  else
  {
    scope = parent;
  }

  if (fn->body)
    resolve(fn->body,        scope);

  if (fn->retExprType)
    resolve(fn->retExprType, scope);

  return 0;
}

static Expr* resolve(BlockStmt* blockStmt, const DefScope* parent)
{
  const DefScope* scope = 0;

  if ((blockStmt->blockTag & BLOCK_SCOPELESS) == 0)
  {
    DefScope* newScope = DefScope::extend(parent);

    for_alist(stmt, blockStmt->body)
    {
      if (DefExpr* defExpr = toDefExpr(stmt))
        newScope->addDefinition(defExpr);
    }

    scope = newScope;
  }
  else
  {
    scope = parent;
  }

  for_alist(stmt, blockStmt->body)
  {
    Expr* resolvedExpr = resolve(stmt, scope);

    INT_ASSERT(resolvedExpr);

    if (resolvedExpr != stmt)
      stmt->replace(resolvedExpr);
  }

  return blockStmt;
}

static Expr* resolve(CondStmt* stmt, const DefScope* scope)
{
  resolve(stmt->condExpr, scope);
  resolve(stmt->thenStmt, scope);

  if (stmt->elseStmt != 0)
    resolve(stmt->elseStmt, scope);

  return stmt;
}

static Expr* resolve(WhileDoStmt* expr, const DefScope* scope)
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

static Expr* resolve(DefExpr* defExpr, const DefScope* scope)
{
  Type* typeType = 0;
  Type* initType = 0;

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

  return 0;
}

static Expr* resolve(SymExpr* expr, const DefScope* scope)
{
  return expr;
}

static Expr* resolve(UnresolvedSymExpr* expr, const DefScope* scope)
{
  std::vector<VisibleSymbol> symbols;
  Expr*                      retval = 0;

  scope->visibleSymbols(expr, symbols);

  if (symbols.size() == 0)
  {
    AstDumpToNode logger(stdout);

    printf("resolve UnresolvedSymExpr. Failed to find a definition\n");
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

static Expr* resolve(CallExpr* expr, const DefScope* scope)
{
  int                count = expr->numActuals();
  std::vector<Type*> actualTypes;

  // Resolve the actuals
  for (int i = 1; i <= count; i++)
  {
    Expr* actual = expr->get(i);
    Expr* res    = resolve(actual, scope);
    Type* type   = typeForExpr(res);

    if (res == 0 || type == 0)
    {
      AstDumpToNode logger(stdout, 3);

      printf("Failed to handle actual %2d\n   ", i);
      actual->accept(&logger);
      printf("\n\n");

      printf("in\n\n   ");
      expr->accept(&logger);
      printf("\n\n\n");

      printf("expr->typeInfo()\n\n   ");
      expr->typeInfo()->accept(&logger);
      printf("\n\n\n");

      INT_ASSERT(false);
    }

    actualTypes.push_back(type);

    if (res != actual)
      actual->replace(res);
  }

  // Select the function
  if (expr->baseExpr)
  {
    Expr*     funcRef = selectFunc(expr->baseExpr, actualTypes, scope);
    FnSymbol* func    = 0;

    INT_ASSERT(funcRef);
    expr->baseExpr->replace(funcRef);

    if (SymExpr* symExpr = toSymExpr(funcRef))
      func = toFnSymbol(symExpr->var);

    INT_ASSERT(func);
    INT_ASSERT(func->formals.length == count);
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
    if (TypeSymbol* type = toTypeSymbol(value->var))
      retval = type->type;

    else if (VarSymbol* var = toVarSymbol(value->var))
      retval = var->type;

    else if (ArgSymbol* arg = toArgSymbol(value->var))
      retval = arg->type;
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
*                                                                           *
*                                                                           *
************************************* | ************************************/

static bool blockCreatesScope(BlockStmt* block)
{
  bool retval = false;

  if (block->blockTag == BLOCK_NORMAL)
  {
    Expr* head = block->body.head;

    for (Expr* stmt = head; stmt && retval == false; stmt = stmt->next)
      retval = isDefExpr(stmt);
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
* A drastically simplified form of Function Resolution for IPE.             *
*                                                                           *
************************************* | ************************************/

static void resolveFuncFormals(FnSymbol*  fn,     const DefScope* scope);

static void resolveFormalType (ArgSymbol* formal, const DefScope* scope);

static bool ipeFunctionExactMatch(FnSymbol*           fn,
                                  std::vector<Type*>& actualTypes);

static Expr* selectFunc(Expr*               funName,
                        std::vector<Type*>& actualTypes,
                        const DefScope*     scope)
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
    AstDumpToNode logger(stdout);

    printf("selectFunc   Unhandled\n");
    funName->accept(&logger);
    printf("\n\n\n");
  }

  return retval;
}

static void resolveFuncFormals(FnSymbol* fn, const DefScope* scope)
{
  for_alist(formal, fn->formals)
  {
    DefExpr*   def = toDefExpr(formal);
    ArgSymbol* arg = toArgSymbol(def->sym);

    INT_ASSERT(arg);

    if (arg->type == 0)
    {
      resolveFormalType(arg, scope);

      INT_ASSERT(arg->type);
    }
  }
}

static void resolveFormalType(ArgSymbol* formal, const DefScope* scope)
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
