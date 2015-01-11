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

class BlockStmt;
class CallExpr;
class Expr;
class FnSymbol;
class SymExpr;

static void  rootScopeInit(DefScope* scope);

static Expr* resolve(const DefScope* scope, ModuleSymbol*      module);
static Expr* resolve(const DefScope* scope, FnSymbol*          fn);

static Expr* resolve(const DefScope* scope, BlockStmt*         expr);

static Expr* resolve(const DefScope* scope, Expr*              expr);
static Expr* resolve(const DefScope* scope, DefExpr*           expr);
static Expr* resolve(const DefScope* scope, SymExpr*           expr);
static Expr* resolve(const DefScope* scope, UnresolvedSymExpr* expr);
static Expr* resolve(const DefScope* scope, CallExpr*          expr);

static void  coerceActualToFormal(Expr* actual, Expr* formal);
static Type* exprType(Expr* expr);

static bool  blockCreatesScope(BlockStmt* block);
static Expr* selectFunc(const DefScope*     scope,
                        Expr*               funName,
                        std::vector<Type*>& actualTypes);

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
        resolve(scopeBase, module);
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

static Expr* resolve(const DefScope* parent, ModuleSymbol* module)
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
    resolve(scope, stmt);
  }

  return 0;
}

static Expr* resolve(const DefScope* parent, FnSymbol* fn)
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

  resolve(scope, fn->body);

  return 0;
}

static Expr* resolve(const DefScope* parent, BlockStmt* bs)
{
  DefScope* bodyScope = DefScope::extend(parent);

  for_alist(stmt, bs->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
      bodyScope->addDefinition(defExpr);
  }

  for_alist(expr, bs->body)
  {
    resolve(bodyScope, expr);
  }

  return 0;
}

static Expr* resolve(const DefScope* scope, Expr* expr)
{
  Expr* retval = 0;

  if (DefExpr* sel = toDefExpr(expr))
  {
    resolve(scope, sel);
    retval = 0;
  }

  else if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr))
    retval = resolve(scope, sel);

  else if (SymExpr*           sel = toSymExpr(expr))
    retval = resolve(scope, sel);

  else if (CallExpr*          sel = toCallExpr(expr))
    retval = resolve(scope, sel);

  else
  {
    AstDumpToNode logger(stdout);

    printf("resolve Unhandled expr\n");
    expr->accept(&logger);
    printf("\n\n\n");
  }

  return retval;
}

static Expr* resolve(const DefScope* scope, DefExpr* defExpr)
{
  Type* typeType = 0;
  Type* initType = 0;

  if (defExpr->exprType != 0)
  {
    Expr* resolvedExpr = resolve(scope, defExpr->exprType);

    INT_ASSERT(resolvedExpr);

    if (resolvedExpr != defExpr->exprType)
      defExpr->exprType->replace(resolvedExpr);

    typeType = exprType(defExpr->exprType);

    INT_ASSERT(typeType);
  }

  if (defExpr->init     != 0)
  {
    Expr* resolvedExpr = resolve(scope, defExpr->init);

    INT_ASSERT(resolvedExpr);

    if (resolvedExpr != defExpr->init)
      defExpr->init->replace(resolvedExpr);

    initType = exprType(defExpr->init);

    INT_ASSERT(initType);
  }

  if (typeType == 0 && initType == 0)
  {
    // It would be a surprise if the parser generated this case
    if (isFnSymbol(defExpr->sym) == false)
      INT_ASSERT(false);
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

static Expr* resolve(const DefScope* scope, SymExpr* expr)
{
  return expr;
}

static Expr* resolve(const DefScope* scope, UnresolvedSymExpr* expr)
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

static Expr* resolve(const DefScope* scope, CallExpr* expr)
{
  int                count = expr->numActuals();
  std::vector<Type*> actualTypes;

  // Resolve the actuals
  for (int i = 1; i <= count; i++)
  {
    Expr* actual = expr->get(i);
    Expr* res    = resolve(scope, actual);
    Type* type   = exprType(res);

    INT_ASSERT(res);
    INT_ASSERT(type);

    actualTypes.push_back(type);

    if (res != actual)
      actual->replace(res);
  }

  // Select the function
  if (expr->baseExpr)
  {
    Expr*     funcRef = selectFunc(scope, expr->baseExpr, actualTypes);
    FnSymbol* func    = 0;

    INT_ASSERT(funcRef);
    expr->baseExpr->replace(funcRef);

    if (SymExpr* symExpr = toSymExpr(funcRef))
      func = toFnSymbol(symExpr->var);

    INT_ASSERT(func);
    INT_ASSERT(func->formals.length == count);

    // Coerce, as required, the actual to match the formal
    for (int i = 1; i <= count; i++)
    {
      Expr* actual = expr->get(i);
      Expr* formal = func->formals.get(i);

      coerceActualToFormal(actual, formal);
    }
  }

  return expr;
}

/************************************ | *************************************
*                                                                           *
* 2015-01-06 Preliminary/Simplified                                         *
*                                                                           *
************************************* | ************************************/

static void coerceActualToFormal(Expr* actual, Expr* formal)
{
  DefExpr*   formalDef = toDefExpr(formal);

  INT_ASSERT(formalDef);

  ArgSymbol* formalSym = toArgSymbol(formalDef->sym);

  INT_ASSERT(formalSym);

  if ((formalSym->intent & INTENT_REF) != 0)
  {
    SET_LINENO(actual);

    CallExpr* addrOf = new CallExpr(PRIM_ADDR_OF);

    actual->replace(addrOf);
    addrOf->insertAtTail(actual);
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static Type* exprType(Expr* expr)
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

  else
    retval = 0;

  if (retval == 0)
  {
    AstDumpToNode logger(stdout);

    printf("exprType  Failed to find type for\n   ");
    expr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

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

static void resolveFuncFormals(const DefScope* scope, FnSymbol*  fn);
static void resolveFormalType (const DefScope* scope, ArgSymbol* formal);
static bool ipeFunctionExactMatch(FnSymbol*           fn,
                                  std::vector<Type*>& actualTypes);

static Expr* selectFunc(const DefScope*     scope,
                        Expr*               funName,
                        std::vector<Type*>& actualTypes)
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
        resolveFuncFormals(scope, fn);
      }
    }

    // Determine if there is an exact match
    for (size_t i = 0; i < visibleSymbols.size() && retval == 0; i++)
    {
      if (FnSymbol* fn = toFnSymbol(visibleSymbols[i].symbol()))
      {
        if (ipeFunctionExactMatch(fn, actualTypes) == true)
        {
          SET_LINENO(funName);

          retval = new SymExpr(fn);

          resolve(visibleSymbols[i].scope(), fn);
        }
      }
    }

    INT_ASSERT(retval);
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

static void resolveFuncFormals(const DefScope* scope, FnSymbol* fn)
{
  for_alist(formal, fn->formals)
  {
    DefExpr*   def = toDefExpr(formal);
    ArgSymbol* arg = toArgSymbol(def->sym);

    INT_ASSERT(arg);

    if (arg->type == 0)
    {
      resolveFormalType(scope, arg);

      INT_ASSERT(arg->type);
    }
  }
}

static void resolveFormalType(const DefScope* scope, ArgSymbol* formal)
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

  if (fn->formals.length == actualsTypes.size())
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
