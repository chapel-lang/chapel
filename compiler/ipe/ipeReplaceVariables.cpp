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

#include "ipeReplaceVariables.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "IpeSymbol.h"
#include "stmt.h"
#include "symbol.h"
#include "WhileDoStmt.h"

static void            replaceVariables(Expr*         expr);

static void            replaceVariables(ModuleSymbol* module);
static void            replaceVariables(FnSymbol*     fn);

static void            replaceVariables(WhileDoStmt*  whileDoStmt);
static void            replaceVariables(BlockStmt*    blockStmt);
static void            replaceVariables(CondStmt*     condStmt);

static void            replaceVariables(DefExpr*      defExpr);
static void            replaceVariables(CallExpr*     callExpr);

void ipeReplaceVariables()
{
  // Replace every user VarSymbol and ArgSymbol with an IpeSymbol
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        if (module->modTag == MOD_USER)
          replaceVariables(module);
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

static void replaceVariables(ModuleSymbol* module)
{
  for_alist(stmt, module->block->body)
    replaceVariables(stmt);
}

static void replaceVariables(FnSymbol* fn)
{
  for (int i = 1; i <= fn->formals.length; i++)
  {
    DefExpr* defExpr = toDefExpr(fn->formals.get(i));

    replaceVariables(defExpr);
  }

  replaceVariables(fn->body);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void replaceVariables(Expr* expression)
{
  if (false)
    ;

  else if (DefExpr*     expr = toDefExpr(expression))
    replaceVariables(expr);

  else if (WhileDoStmt* expr = toWhileDoStmt(expression))
    replaceVariables(expr);

  else if (CondStmt*    expr = toCondStmt(expression))
    replaceVariables(expr);

  else if (CallExpr*    expr = toCallExpr(expression))
    replaceVariables(expr);

  // This must appear after WhileDoStmt etc
  else if (BlockStmt*   expr = toBlockStmt(expression))
    replaceVariables(expr);

  else if (isUnresolvedSymExpr(expression))
    ;

  else if (isSymExpr(expression))
    ;

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("replaceVariables unhandled expr\n");
    printf("   ");
    expression->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }
}

static void replaceVariables(WhileDoStmt* whileDoStmt)
{
  replaceVariables(whileDoStmt->condExprGet());

  for_alist(expr, whileDoStmt->body)
    replaceVariables(expr);
}

static void replaceVariables(BlockStmt* blockStmt)
{
  for_alist(expr, blockStmt->body)
    replaceVariables(expr);
}

static void replaceVariables(CondStmt* condStmt)
{
  replaceVariables(condStmt->condExpr);
  replaceVariables(condStmt->thenStmt);

  if (condStmt->elseStmt != 0)
    replaceVariables(condStmt->elseStmt);

}

static void replaceVariables(DefExpr* defExpr)
{
  if (false)
    ;

  else if (VarSymbol* varSym = toVarSymbol(defExpr->sym))
  {
    if (varSym->isParameter() == false)
    {
      SET_LINENO(varSym);

      defExpr->sym = new IpeSymbol(varSym);
    }
  }

  else if (ArgSymbol* argSym = toArgSymbol(defExpr->sym))
  {
    SET_LINENO(argSym);

    defExpr->sym = new IpeSymbol(argSym);
  }

  else if (FnSymbol* fnSym = toFnSymbol(defExpr->sym))
  {
    replaceVariables(fnSym);
  }

  else
  {
    AstDumpToNode logger(stdout);

    printf("replaceVariables unhandled DefExpr\n");
    defExpr->accept(&logger);
    printf("\n\n\n");
  }
}

static void replaceVariables(CallExpr* expr)
{
  int count = expr->numActuals();

  for (int i = 1; i <= count; i++)
    replaceVariables(expr->get(i));
}
