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

#include "ipeEvaluate.h"

#include "AstDumpToNode.h"
#include "IpeEnvironment.h"
#include "IpeSymbol.h"
#include "IpeValue.h"

#include "alist.h"
#include "stmt.h"
#include "symbol.h"
#include "WhileDoStmt.h"

static void     collectVariables(std::vector<IpeSymbol*>& variables);

static void     execute(IpeEnvironment* env);

static IpeValue evaluate(Expr*         expr,    IpeEnvironment* env);

static IpeValue evaluate(Symbol*       sym,     IpeEnvironment* env);
static IpeValue evaluate(VarSymbol*    sym,     IpeEnvironment* env);
static IpeValue evaluate(ModuleSymbol* module,  IpeEnvironment* env);

static IpeValue evaluate(WhileDoStmt*  expr,    IpeEnvironment* env);
static IpeValue evaluate(BlockStmt*    expr,    IpeEnvironment* env);
static IpeValue evaluate(CondStmt*     expr,    IpeEnvironment* env);

static IpeValue evaluate(SymExpr*      expr,    IpeEnvironment* env);
static IpeValue evaluate(DefExpr*      expr,    IpeEnvironment* env);
static IpeValue evaluate(CallExpr*     expr,    IpeEnvironment* env);

void ipeEvaluate()
{
  std::vector<IpeSymbol*> variables;
  IpeEnvironment*         rootEnv = 0;

  collectVariables(variables);

  rootEnv = new IpeEnvironment(0, variables);

  execute(rootEnv);

  rootEnv->describe();

  delete rootEnv;
}

static void collectVariables(std::vector<IpeSymbol*>& variables)
{
  for_alist(rootStmt, rootModule->block->body)
  {
    if (DefExpr* rootDefExpr = toDefExpr(rootStmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(rootDefExpr->sym))
      {
        if (module != rootModule)
        {
          for_alist(stmt, module->block->body)
          {
            if (DefExpr* defExpr = toDefExpr(stmt))
            {
              if (IpeSymbol* symbol = toIpeSymbol(defExpr->sym))
              {
                variables.push_back(symbol);
              }
            }
          }
        }
      }
    }
  }
}

static void execute(IpeEnvironment* env)
{
  for_alist(rootStmt, rootModule->block->body)
  {
    if (DefExpr* rootDefExpr = toDefExpr(rootStmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(rootDefExpr->sym))
      {
        if (module != rootModule)
          evaluate(module, env);
      }
    }
  }
}

static IpeValue evaluate(Expr* expr, IpeEnvironment* env)
{
  IpeValue retval;

  if (SymExpr* symExpr = toSymExpr(expr))
    retval = evaluate(symExpr, env);

  else if (DefExpr*     defExpr     = toDefExpr(expr))
    retval = evaluate(defExpr, env);

  else if (CallExpr*    callExpr    = toCallExpr(expr))
    retval = evaluate(callExpr, env);

  else if (WhileDoStmt* whileDoStmt = toWhileDoStmt(expr))
    retval = evaluate(whileDoStmt, env);

  else if (BlockStmt*   blockStmt   = toBlockStmt(expr))
    retval = evaluate(blockStmt, env);

  else if (CondStmt*    condStmt    = toCondStmt(expr))
    retval = evaluate(condStmt, env);

  else
  {
    AstDumpToNode logger(stdout);

    printf("evaluate(Expr, Env)\n\n");
    printf("Failed to handle\n");

    expr->accept(&logger);
    printf("\n\n\n");

    retval.iValue = 0;

    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluate(Symbol* sym, IpeEnvironment* env)
{
  IpeValue retval;

  if (VarSymbol* varSym = toVarSymbol(sym))
  {
    retval = evaluate(varSym, env);
  }

  else
  {
    AstDumpToNode logger(stdout);

    printf("evaluate(Symbol, Env)\n\n");
    printf("Failed to handle\n");

    sym->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);

    retval.iValue = 0;
  }

  return retval;
}

static IpeValue evaluate(VarSymbol* sym, IpeEnvironment* env)
{
  IpeValue retval;

  if (Immediate* imm = sym->immediate)
  {
    Type* type = sym->type;

    if (strcmp(type->symbol->name, "bool") == 0)
      retval.bValue = imm->v_bool;

    else if (strcmp(type->symbol->name, "int")  == 0)
      retval.iValue = imm->v_int64;

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.rValue = imm->v_float64;

    else
    {
      retval.iValue = 0;
      INT_ASSERT(false);
    }
  }
  else
  {
    retval.iValue = 0;
    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluate(ModuleSymbol* module, IpeEnvironment* env)
{
  IpeValue retval;

  for_alist(stmt, module->block->body)
    evaluate(stmt, env);

  retval.iValue = 0;

  return retval;
}

static IpeValue evaluate(WhileDoStmt* whileDoStmt, IpeEnvironment* env)
{
  bool     proceed = true;
  IpeValue retval;

  while (proceed == true)
  {
    IpeValue cond = evaluate(whileDoStmt->condExprGet(), env);

    proceed = cond.bValue;

    if (proceed == true)
    {
      for_alist(expr, whileDoStmt->body)
        evaluate(expr, env);
    }
  }

  retval.iValue = 0;

  return retval;
}

static IpeValue evaluate(BlockStmt* blockStmt, IpeEnvironment* env)
{
  IpeValue retval;

  for_alist(expr, blockStmt->body)
    evaluate(expr, env);

  retval.iValue = 0;

  return retval;
}

static IpeValue evaluate(CondStmt* expr, IpeEnvironment* env)
{
  IpeValue condValue = evaluate(expr->condExpr, env);
  IpeValue retval;

  if (condValue.bValue == 1)
    retval = evaluate(expr->thenStmt, env);

  else if (expr->elseStmt != 0)
    retval = evaluate(expr->elseStmt, env);

  else
    retval.iValue = 0;

  return retval;
}

static IpeValue evaluate(SymExpr* expr, IpeEnvironment* env)
{
  IpeValue retval;

  if (VarSymbol* symbol = toVarSymbol(expr->var))
    retval = evaluate(symbol, env);

  else if (IpeSymbol* variable = toIpeSymbol(expr->var))
    retval = env->lookup(variable);

  else
  {
    AstDumpToNode logger(stdout);

    printf("evaluate(SymExpr, Env)\n\n");
    printf("Failed to handle\n");

    expr->accept(&logger);
    printf("\n\n\n");

    retval.iValue = 0;
    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluate(DefExpr* defExpr, IpeEnvironment* env)
{
  IpeValue retval;

  if (IpeSymbol* sym = toIpeSymbol(defExpr->sym))
  {
    IpeValue value;

    if (defExpr->init == 0)
      value = evaluate(sym->type->defaultValue, env);

    else
      value = evaluate(defExpr->init,      env);

    env->assign(sym, value);
  }

  // Must be a param
  else if (isVarSymbol(defExpr->sym) == true)
  {

  }

  else if (isFnSymbol(defExpr->sym)  == true)
  {

  }

  else
  {
    AstDumpToNode logger(stdout);

    printf("evaluate(DefExpr, Env)\n\n");
    printf("Failed to handle\n");

    defExpr->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  retval.iValue = 0;

  return retval;
}

static IpeValue evaluate(CallExpr* callExpr, IpeEnvironment* env)
{
  bool     handled = false;
  IpeValue retval;

  if (callExpr->primitive == 0)
  {
    handled       = false;
    retval.iValue = 0;
  }

  else if (callExpr->isPrimitive(PRIM_ASSIGN) == true)
  {
    SymExpr*   dstSymExpr = toSymExpr(callExpr->get(1));
    IpeSymbol* variable   = toIpeSymbol(dstSymExpr->var);
    IpeValue   value      = evaluate(callExpr->get(2), env);

    env->assign(variable, value);
    retval.iValue = 0;
    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_ADD) == true)
  {
    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.iValue = arg1.iValue + arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.rValue = arg1.rValue + arg2.rValue;
    }

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_SUBTRACT) == true)
  {
    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.iValue = arg1.iValue - arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.rValue = arg1.rValue - arg2.rValue;
    }

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_MULT) == true)
  {
    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.iValue = arg1.iValue * arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.rValue = arg1.rValue * arg2.rValue;
    }

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_DIV) == true)
  {
    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.iValue = arg1.iValue / arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.rValue = arg1.rValue / arg2.rValue;
    }

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_EQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "bool") == 0)
    {
      retval.bValue = arg1.bValue == arg2.bValue;
    }

    else if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.bValue = arg1.iValue == arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.bValue = arg1.rValue == arg2.rValue;
    }

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_NOTEQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "bool") == 0)
    {
      retval.bValue = arg1.bValue != arg2.bValue;
    }

    else if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.bValue = arg1.iValue != arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.bValue = arg1.rValue != arg2.rValue;
    }

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_GREATER) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.bValue = arg1.iValue > arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.bValue = arg1.rValue > arg2.rValue;
    }

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_LESS) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.bValue = arg1.iValue < arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.bValue = arg1.rValue < arg2.rValue;
    }

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_GREATEROREQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.bValue = arg1.iValue >= arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.bValue = arg1.rValue >= arg2.rValue;
    }

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_LESSOREQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluate(callExpr->get(1), env);
    IpeValue arg2 = evaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "int") == 0)
    {
      retval.bValue = arg1.iValue <= arg2.iValue;
    }

    else if (strcmp(type->symbol->name, "real") == 0)
    {
      retval.bValue = arg1.rValue <= arg2.rValue;
    }

    handled = true;
  }

  else
  {
    retval.iValue = 0;
  }

  if (handled == false)
  {
    AstDumpToNode logger(stdout);

    printf("evaluate(CallExpr, Env)\n\n");
    printf("Failed to handle\n");

    callExpr->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return retval;
}
