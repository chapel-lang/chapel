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

#include "ipeAssignLocations.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "symbol.h"
#include "WhileDoStmt.h"

static int   assignLocations(Symbol*       symbol,    int depth, int offset);
static int   assignLocations(ModuleSymbol* module,    int depth, int offset);
static int   assignLocations(FnSymbol*     fn,        int depth, int offset);
static int   assignLocations(LcnSymbol*    symbol,    int depth, int offset);

static int   assignLocations(Expr*         expr,      int depth, int offset);

static int   assignLocations(WhileDoStmt*  blockStmt, int depth, int offset);
static int   assignLocations(BlockStmt*    blockStmt, int depth, int offset);
static int   assignLocations(CondStmt*     condStmt,  int depth, int offset);

static int   assignLocations(DefExpr*      defExpr,   int depth, int offset);
static int   assignLocations(CallExpr*     callExpr,  int depth, int offset);

void ipeAssignLocations()
{
  int offset = 0;

  // Assign a location to every user-defined variable
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        if (module->modTag == MOD_USER)
          offset = assignLocations(module, 0, offset);
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

static int assignLocations(Symbol* symbol, int depth, int offset)
{
  if (false)
    ;

  else if (LcnSymbol*    sym = toLcnSymbol(symbol))
    offset = assignLocations(sym, depth, offset);

  else if (FnSymbol*     sym = toFnSymbol(symbol))
    assignLocations(sym, depth + 1, 0);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("assignLocations unhandled symbol\n");
    printf("   ");
    symbol->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return offset;
}

static int assignLocations(ModuleSymbol* module, int depth, int offset)
{
  for_alist(stmt, module->block->body)
    offset = assignLocations(stmt, 0, offset);

  return offset;
}

static int assignLocations(FnSymbol* fn, int depth, int offset)
{
  for (int i = 1; i <= fn->formals.length; i++)
  {
    DefExpr*   defExpr = 0;
    LcnSymbol* formal  = 0;

    defExpr = toDefExpr(fn->formals.get(i));
    INT_ASSERT(defExpr);

    formal  = toLcnSymbol(defExpr->sym);
    INT_ASSERT(formal);

    offset = assignLocations(formal, depth, offset);
  }

  assignLocations(fn->body, depth, offset);

  return offset;
}

// NOAKES 2015/01/26: LcnSymbol is currently of fixed size 8
static int assignLocations(LcnSymbol* symbol, int depth, int offset)
{
  symbol->locationSet(depth, offset);

  return offset + 8;
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int assignLocations(Expr* expression, int depth, int offset)
{
  if (false)
    ;

  else if (isSymExpr(expression) == true)
    ;

  else if (FnSymbol*    expr = toFnSymbol(expression))
    offset = assignLocations(expr, depth + 1, offset);

  else if (WhileDoStmt* expr = toWhileDoStmt(expression))
    offset = assignLocations(expr, depth + 0, offset);

  // This must appear after WhileDoStmt etc
  else if (BlockStmt*   expr = toBlockStmt(expression))
    offset = assignLocations(expr, depth + 0, offset);

  else if (CondStmt*    expr = toCondStmt(expression))
    offset = assignLocations(expr, depth + 0, offset);

  else if (DefExpr*     expr = toDefExpr(expression))
    offset = assignLocations(expr, depth + 0, offset);

  else if (CallExpr*    expr = toCallExpr(expression))
    offset = assignLocations(expr, depth + 0, offset);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("assignLocations unhandled expr\n");
    printf("   ");
    expression->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return offset;
}

static int assignLocations(WhileDoStmt* whileDoStmt, int depth, int offset)
{
  int localOffset = offset;

  for_alist(expr, whileDoStmt->body)
    localOffset = assignLocations(expr, depth, localOffset);

  return offset;
}

static int assignLocations(BlockStmt* blockStmt, int depth, int offset)
{
  int localOffset = offset;

  for_alist(expr, blockStmt->body)
    localOffset = assignLocations(expr, depth, localOffset);

  return offset;
}

static int assignLocations(CondStmt* condStmt, int depth, int offset)
{
  assignLocations(condStmt->thenStmt, depth, offset);

  if (condStmt->elseStmt != 0)
    assignLocations(condStmt->elseStmt, depth, offset);

  return offset;
}

static int assignLocations(DefExpr* defExpr, int depth, int offset)
{
  if (defExpr->sym  != 0)
    offset = assignLocations(defExpr->sym,  depth, offset);

  if (defExpr->init != 0)
    offset = assignLocations(defExpr->init, depth, offset);

  return offset;
}

static int assignLocations(CallExpr* callExpr, int depth, int offset)
{
  return offset;
}
