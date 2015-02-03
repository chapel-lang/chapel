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

#include "ipeInlinePrimitives.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "WhileDoStmt.h"

struct Binding
{
  ArgSymbol* arg;
  Expr*      expr;
};

static void  ipeInline(ModuleSymbol* module);
static void  ipeInline(FnSymbol*     fn);

static void  ipeInline(WhileDoStmt*  blockStmt);
static void  ipeInline(BlockStmt*    blockStmt);
static void  ipeInline(CondStmt*     condStmt);

static void  ipeInline(Expr*         expr);
static void  ipeInline(DefExpr*      defExpr);
static void  ipeInline(CallExpr*     callExpr);

static Expr* theInlineableExpr(FnSymbol*  func);
static Expr* theInlineableExpr(BlockStmt* body);

void ipeInlinePrimitives()
{
  // Scan the Root Module Declaration
  //   Every expression should be a DefExpr.
  //   Process every top level module
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        if (module->modTag == MOD_USER)
          ipeInline(module);
      }
    }
    else
      INT_ASSERT(false);
  }
}

static void ipeInline(ModuleSymbol* module)
{
  for_alist(stmt, module->block->body)
  {
    ipeInline(stmt);
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void ipeInline(Expr* genExpr)
{
  if (isSymExpr(genExpr) == true)
    ;

  else if (FnSymbol*    expr = toFnSymbol(genExpr))
    ipeInline(expr);

  else if (WhileDoStmt* expr = toWhileDoStmt(genExpr))
    ipeInline(expr);

  // This must appear after WhileDoStmt etc
  else if (BlockStmt*   expr = toBlockStmt(genExpr))
    ipeInline(expr);

  else if (CondStmt*    expr = toCondStmt(genExpr))
    ipeInline(expr);

  else if (DefExpr*     expr = toDefExpr(genExpr))
    ipeInline(expr);

  else if (CallExpr*    expr = toCallExpr(genExpr))
    ipeInline(expr);

  else
  {
    AstDumpToNode logger(stdout);

    printf("ipeInline unhandled expr\n");
    genExpr->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }
}

static void ipeInline(FnSymbol* fn)
{
  ipeInline(fn->body);
}

static void ipeInline(WhileDoStmt* whileDoStmt)
{
  ipeInline(whileDoStmt->condExprGet());

  for_alist(expr, whileDoStmt->body)
    ipeInline(expr);
}

static void ipeInline(BlockStmt* blockStmt)
{
  for_alist(expr, blockStmt->body)
    ipeInline(expr);
}

static void ipeInline(CondStmt* condStmt)
{
  ipeInline(condStmt->condExpr);
  ipeInline(condStmt->thenStmt);

  if (condStmt->elseStmt != 0)
    ipeInline(condStmt->elseStmt);
}

static void ipeInline(DefExpr* defExpr)
{
  if (defExpr->sym != 0)
  {
    if (FnSymbol* fn = toFnSymbol(defExpr->sym))
      ipeInline(fn);
  }

  if (defExpr->init != 0)
    ipeInline(defExpr->init);
}

// NOAKES 2015-01-07: This handles only the simplest possible
// cases required for the simplest primitives currently in IPE
// ChapelBase. Extremely fragile.
static void ipeInline(CallExpr* callExpr)
{
  if (callExpr->baseExpr)
  {
    SymExpr* funExpr = toSymExpr(callExpr->baseExpr);
    INT_ASSERT(funExpr);

    FnSymbol* func   = toFnSymbol(funExpr->var);
    INT_ASSERT(func);

    // Inline the formals
    for (int i = 1; i <= func->formals.length; i++)
    {
      ipeInline(callExpr->get(i));
    }

    if (func->hasFlag(FLAG_INLINE) == true)
    {
      if (Expr* inlineExpr = theInlineableExpr(func))
      {
        std::vector<Binding> bindings;

        for (int i = 1; i <= func->formals.length; i++)
        {
          DefExpr*   defExpr = toDefExpr(func->formals.get(i));
          ArgSymbol* formal  = toArgSymbol(defExpr->sym);
          Expr*      actual  = callExpr->get(i);

          Binding    binding;

          binding.arg  = formal;
          binding.expr = actual;

          bindings.push_back(binding);
        }

        if (SymExpr* symExpr = toSymExpr(inlineExpr))
        {
          ArgSymbol* argSym = toArgSymbol(symExpr->var);
          INT_ASSERT(argSym);

          int        index  = -1;
          Expr*      expr   =  0;

          for (size_t j = 0; j < bindings.size() && index == -1; j++)
          {
            index = (bindings[j].arg == argSym) ? j : -1;
          }

          INT_ASSERT(index != -1);

          expr = bindings[index].expr;
          INT_ASSERT(expr);

          // NOAKES 2015/01/15 Note that we are consuming the actual, not copying it
          bindings[index].expr = 0;
          expr->remove();

          callExpr->replace(expr);
        }

        else if (CallExpr* body = toCallExpr(inlineExpr))
        {
          SET_LINENO(body);

          CallExpr* copy = new CallExpr(body->primitive);

          for (int i = 1; i <= body->argList.length; i++)
          {
            SymExpr*   symExpr = toSymExpr(body->argList.get(i));
            INT_ASSERT(symExpr);

            ArgSymbol* argSym  = toArgSymbol(symExpr->var);
            INT_ASSERT(argSym);

            int        index   = -1;
            Expr*      expr    =  0;

            for (size_t j = 0; j < bindings.size() && index == -1; j++)
            {
              index = (bindings[j].arg == argSym) ? j : -1;
            }

            INT_ASSERT(index != -1);

            expr = bindings[index].expr;
            INT_ASSERT(expr);

            // NOAKES 2015/01/15 Note that we are consuming the actual, not copying
            bindings[index].expr = 0;
            expr->remove();

            copy->insertAtTail(expr);
          }

          callExpr->replace(copy);
        }

        else
        {
          AstDumpToNode logger(stdout, 3);

          printf("ipeInline CallExpr.  No support for\n   ");
          inlineExpr->accept(&logger);
          printf("\n\n");
          printf("in\n\n");
          printf("   ");
          func->accept(&logger);
          printf("\n\n");

          INT_ASSERT(false);
        }
      }
    }
  }

  else
  {
#if 0
    AstDumpToNode logger(stdout, 3);

    printf("CallExpr prim unhandled\n");
    printf("   ");
    callExpr->accept(&logger);
    printf("\n\n");
#endif

    for (int i = 1; i <= callExpr->numActuals(); i++)
    {
      ipeInline(callExpr->get(i));
    }
  }
}

static Expr* theInlineableExpr(FnSymbol* func)
{
  return theInlineableExpr(func->body);
}

static Expr* theInlineableExpr(BlockStmt* body)
{
  Expr* retval = 0;

  if (body->length() == 1)
  {
    Expr* stmt = body->body.only();

    if (CallExpr* callExpr = toCallExpr(stmt))
    {
      if (callExpr->primitive != 0)
      {
        if (callExpr->isPrimitive(PRIM_RETURN) == true)
          retval = callExpr->get(1);
        else
          retval = callExpr;
      }
    }

    else if (BlockStmt* bs = toBlockStmt(stmt))
    {
      retval = theInlineableExpr(bs);
    }
  }

  return retval;
}
