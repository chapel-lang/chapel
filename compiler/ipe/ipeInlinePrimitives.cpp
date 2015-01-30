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

struct Binding
{
  ArgSymbol* arg;
  Expr*      expr;
};

static void ipeInline(ModuleSymbol* module);
static void ipeInline(FnSymbol*     fn);

static void ipeInline(BlockStmt*    bs);

static void ipeInline(Expr*         expr);
static void ipeInline(DefExpr*      defExpr);
static void ipeInline(CallExpr*     callExpr);

void ipeInlinePrimitives()
{
  // Scan the Root Module Declaration
  //   Every expression should be a DefExpr.
  //   Process every Top Level Module Declaration
  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        // ChapelBase currently contains only functions to define Chapel
        if (module != baseModule)
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

static void ipeInline(FnSymbol* fn)
{
  ipeInline(fn->body);
}

static void ipeInline(BlockStmt* bs)
{
  for_alist(expr, bs->body)
    ipeInline(expr);
}

static void ipeInline(Expr* genExpr)
{
  if (isSymExpr(genExpr) == true)
  {

  }

  else if (FnSymbol*  expr = toFnSymbol(genExpr))
    ipeInline(expr);

  else if (BlockStmt* expr = toBlockStmt(genExpr))
    ipeInline(expr);

  else if (DefExpr*   expr = toDefExpr(genExpr))
    ipeInline(expr);

  else if (CallExpr*  expr = toCallExpr(genExpr))
    ipeInline(expr);

  else
  {
    AstDumpToNode logger(stdout);

    printf("ipeInline unhandled expr\n");
    genExpr->accept(&logger);
    printf("\n\n\n");
  }
}

static void ipeInline(DefExpr* defExpr)
{
  if (defExpr->init != 0)
    ipeInline(defExpr->init);
}

// NOAKES 2015-01-07: This handles only the simplest possible
// cases required for the simplest primitives currently in IPE
// ChapelBase. Currently extremely fragile.
static void ipeInline(CallExpr* expr)
{
  if (expr->baseExpr)
  {
    SymExpr* funExpr = toSymExpr(expr->baseExpr);
    INT_ASSERT(funExpr);

    FnSymbol* func   = toFnSymbol(funExpr->var);
    INT_ASSERT(func);

    if (func->hasFlag(FLAG_INLINE)                     == true &&
        func->body->length()                           == 1    &&
        isCallExpr(func->body->body.only())            == true &&
        toCallExpr(func->body->body.only())->baseExpr  == 0    &&
        toCallExpr(func->body->body.only())->primitive != 0)
    {
      std::vector<Binding> bindings;

      for (int i = 1; i <= func->formals.length; i++)
      {
        DefExpr*   defExpr = toDefExpr(func->formals.get(i));
        ArgSymbol* formal  = toArgSymbol(defExpr->sym);
        Expr*      actual  = expr->get(i);

        Binding    binding;

        binding.arg  = formal;
        binding.expr = actual;

        bindings.push_back(binding);
      }

      {
        CallExpr* body = toCallExpr(func->body->body.only());

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

          bindings[index].expr = 0;
          expr->remove();

          copy->insertAtTail(expr);
        }

        expr->replace(copy);
      }
    }
  }
}
