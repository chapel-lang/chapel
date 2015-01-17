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

#include "ipeInsertVariables.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "IpeSymbol.h"
#include "stmt.h"
#include "symbol.h"
#include "WhileDoStmt.h"

#include <map>

static void collectVariables(ModuleSymbol* module);

static void replaceVariables(ModuleSymbol* module);
static void replaceVariables(FnSymbol*     fn);

static void replaceVariables(WhileDoStmt*  whileDoStmt);
static void replaceVariables(BlockStmt*    blockStmt);
static void replaceVariables(CondStmt*     condStmt);

static void replaceVariables(Expr*         expr);
static void replaceVariables(DefExpr*      defExpr);
static void replaceVariables(SymExpr*      symExpr);
static void replaceVariables(CallExpr*     callExpr);

static std::map<Symbol*, IpeSymbol*> sVariableMap;

// Currently only a single environment so we don't need depth
static int                           sVariableOffset        = 0;

void ipeInsertVariables()
{
  // Map every module-level VarSymbol to an IpeSymbol
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        collectVariables(module);
      }
    }
    else
      INT_ASSERT(false);
  }

  // Replace every module-level VarSymbol with appropriate IpeSymbol
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        replaceVariables(module);
      }
    }
    else
      INT_ASSERT(false);
  }

  sVariableMap.clear();
  sVariableOffset = 0;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void collectVariables(ModuleSymbol* module)
{
  for_alist(stmt, module->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (VarSymbol* varSym = toVarSymbol(defExpr->sym))
      {
        if (varSym->isParameter() == false)
        {
          SET_LINENO(varSym);

          IpeSymbol* variable  = new IpeSymbol(varSym->name,
                                               varSym->type,
                                               0,
                                               sVariableOffset);

          variable->defPoint   = defExpr;

          sVariableMap[varSym] = variable;

          // NOAKES 2015/01/14 Currently hard-coded
          sVariableOffset      = sVariableOffset + 8;
        }
      }

      else if (isFnSymbol(defExpr->sym) == true)
      {

      }

      else
      {
        AstDumpToNode logger(stdout, 3);

        printf("collectVariables unhandled DefExpr.sym\n");

        printf("   ");
        defExpr->accept(&logger);
        printf("\n");
        printf("\n");
      }
    }
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void replaceVariables(Expr* genExpr)
{
  if (FnSymbol*  expr = toFnSymbol(genExpr))
    replaceVariables(expr);

  else if (WhileDoStmt* expr = toWhileDoStmt(genExpr))
    replaceVariables(expr);

  else if (BlockStmt*   expr = toBlockStmt(genExpr))
    replaceVariables(expr);

  else if (CondStmt*    expr = toCondStmt(genExpr))
    replaceVariables(expr);

  else if (DefExpr*     expr = toDefExpr(genExpr))
    replaceVariables(expr);

  else if (SymExpr*     expr = toSymExpr(genExpr))
    replaceVariables(expr);

  else if (CallExpr*    expr = toCallExpr(genExpr))
    replaceVariables(expr);

  else
  {
    AstDumpToNode logger(stdout);

    printf("replaceVariables unhandled expr\n");
    genExpr->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }
}

static void replaceVariables(ModuleSymbol* module)
{
  for_alist(stmt, module->block->body)
    replaceVariables(stmt);
}

static void replaceVariables(FnSymbol* fn)
{
  replaceVariables(fn->body);
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
  if (VarSymbol* varSym = toVarSymbol(defExpr->sym))
  {
    if (varSym->isParameter() == false)
    {
      if (IpeSymbol* variable = sVariableMap[varSym])
      {
        defExpr->sym = variable;
      }

      else
      {
        AstDumpToNode logger(stdout, 3);

        printf("Failed to find an IPE variable for\n   ");
        varSym->accept(&logger);
        printf("\n\n\n");
      }
    }
  }

  else if (isFnSymbol(defExpr->sym))
  {

  }

  else
  {
    AstDumpToNode logger(stdout);

    printf("replaceVariables unhandled DefExpr\n");
    defExpr->accept(&logger);
    printf("\n\n\n");
  }
}

static void replaceVariables(SymExpr* symExpr)
{
  if (VarSymbol* varSym = toVarSymbol(symExpr->var))
  {
    if (varSym->isParameter() == false)
    {
      if (IpeSymbol* variable = sVariableMap[varSym])
      {
        symExpr->var = variable;
      }

      else
      {
        AstDumpToNode logger(stdout, 3);

        printf("Failed to find an IPE variable for\n   ");
        varSym->accept(&logger);
        printf("\n\n\n");
      }
    }
  }

  else
  {
    AstDumpToNode logger(stdout);

    printf("replaceVariables unhandled symExpr\n");
    symExpr->accept(&logger);
    printf("\n\n\n");
  }
}

static void replaceVariables(CallExpr* expr)
{
  int count = expr->numActuals();

  for (int i = 1; i <= count; i++)
    replaceVariables(expr->get(i));
}
