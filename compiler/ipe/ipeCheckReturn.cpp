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

#include "ipeCheckReturn.h"

#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "symbol.h"

#include "AstDumpToNode.h"

static void  checkReturn(ModuleSymbol* module);

static void  checkReturn(FnSymbol*     fnSymbol);

static void  checkReturn(Expr*         expr,
                         Type*         type,
                         bool          isLastStmt);

static Type* typeForExpr(Expr*         expr);

void ipeCheckReturn()
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
          checkReturn(module);
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

static void checkReturn(ModuleSymbol* module)
{
  for_alist(stmt, module->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (FnSymbol* fnSymbol = toFnSymbol(defExpr->sym))
      {
        checkReturn(fnSymbol);
      }
    }
  }
}

static void checkReturn(FnSymbol* fnSymbol)
{
  INT_ASSERT(fnSymbol->retExprType->body.length == 1);

  BlockStmt*  bs       = fnSymbol->body;
  int         count    = bs->length();

  SymExpr*    typeExpr = toSymExpr(fnSymbol->retExprType->body.get(1));

  INT_ASSERT(isTypeSymbol(typeExpr->var));

  TypeSymbol* typeSym  = toTypeSymbol(typeExpr->var);
  INT_ASSERT(typeSym);

  Type*       type     = typeSym->type;

  // The body is an empty BlockStmt and so the return type must be void
  if (count == 0)
  {
    if (type != dtVoid)
    {
      printf("%s:%d: In function '%s':\n",
             cleanFilename(fnSymbol),
             fnSymbol->linenum(),
             fnSymbol->name);

      printf("%s:%d: error: control reaches end of function that returns a value\n",
             cleanFilename(fnSymbol),
             fnSymbol->linenum());
    }
  }

  // Confirm that the final stmt actually returns the correct type
  else
  {
    for (int i = 1; i <= count; i++)
    {
      Expr* stmt = bs->body.get(i);

      checkReturn(stmt, type, (i == count) ? true : false);
    }
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void checkReturn(BlockStmt* bs,   Type* type, bool isLastStmt);
static void checkReturn(CallExpr*  expr, Type* type, bool isLastStmt);

static void checkReturn(Expr* expr, Type* type, bool isLastStmt)
{
  if (false)
    ;

  else if (isDefExpr(expr) == true)
    ;

  else if (CallExpr*  callExpr = toCallExpr(expr))
    checkReturn(callExpr, type, isLastStmt);

  // This must come after while etc
  else if (BlockStmt* bs       = toBlockStmt(expr))
    checkReturn(bs,       type, isLastStmt);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("checkReturn unhandled\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }
}

static void checkReturn(BlockStmt* bs, Type* type, bool isLastStmt)
{
  int count = bs->length();

  for (int i = 1; i <= count; i++)
  {
    Expr* stmt = bs->body.get(i);

    checkReturn(stmt, type, (i == count) ? isLastStmt : false);
  }
}

static void checkReturn(CallExpr*  expr, Type* type, bool isLastStmt)
{
  if (expr->isPrimitive(PRIM_RETURN))
  {
    if (Type* exprType = typeForExpr(expr))
    {
      if (type != exprType)
      {
        AstDumpToNode logger(stdout, 3);

        printf("exprType:\n");
        printf("   ");
        exprType->accept(&logger);
        printf("\n\n");

        printf("Type:\n");
        printf("   ");
        type->accept(&logger);
        printf("\n\n");

        INT_ASSERT(false);
      }
    }
    else
    {
      INT_ASSERT(false);
    }
  }
}

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

    // Var/Arg in user level modules and Immediates
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
  {
    AstDumpToNode logger(stdout, 3);

    printf("typeForExpr(Expr*)\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    retval = 0;
  }

  return retval;
}
