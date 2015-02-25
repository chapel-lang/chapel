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
#include "IpeValue.h"

#include "alist.h"
#include "stmt.h"
#include "symbol.h"
#include "WhileDoStmt.h"

static IpeEnvironment* createRootEnvironment();
static int             environmentSize(FnSymbol* symbol);

static void            execute(IpeEnvironment* env);

static IpeValue        evaluate(Expr*         expr,    IpeEnvironment* env);

static IpeValue        evaluate(Symbol*       sym,     IpeEnvironment* env);
static IpeValue        evaluate(VarSymbol*    sym,     IpeEnvironment* env);
static IpeValue        evaluate(ModuleSymbol* module,  IpeEnvironment* env);

static IpeValue        evaluate(WhileDoStmt*  expr,    IpeEnvironment* env);
static IpeValue        evaluate(BlockStmt*    expr,    IpeEnvironment* env);
static IpeValue        evaluate(CondStmt*     expr,    IpeEnvironment* env);

static IpeValue        evaluate(SymExpr*      expr,    IpeEnvironment* env);
static IpeValue        evaluate(DefExpr*      expr,    IpeEnvironment* env);
static IpeValue        evaluate(CallExpr*     expr,    IpeEnvironment* env);

static IpeValue        evaluateCall  (CallExpr* callExpr, IpeEnvironment* env);
static IpeValue        evaluatePrimop(CallExpr* callExpr, IpeEnvironment* env);

static bool            isImmediate(Symbol* sym);

void ipeEvaluate()
{
  IpeEnvironment* rootEnv = createRootEnvironment();

  execute(rootEnv);

  rootEnv->describe();

  delete rootEnv;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void collectTopLevelVariables(std::vector<LcnSymbol*>& variables);

static IpeEnvironment* createRootEnvironment()
{
  std::vector<LcnSymbol*> variables;

  collectTopLevelVariables(variables);

  return new IpeEnvironment(0, variables);
}

static void collectTopLevelVariables(std::vector<LcnSymbol*>& variables)
{
  for_alist(rootStmt, rootModule->block->body)
  {
    if (DefExpr* rootDefExpr = toDefExpr(rootStmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(rootDefExpr->sym))
      {
        if (module->modTag == MOD_USER)
        {
          for_alist(stmt, module->block->body)
          {
            if (DefExpr* defExpr = toDefExpr(stmt))
            {
              if (LcnSymbol* symbol = toLcnSymbol(defExpr->sym))
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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void execute(IpeEnvironment* env)
{
  for_alist(rootStmt, rootModule->block->body)
  {
    if (DefExpr* rootDefExpr = toDefExpr(rootStmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(rootDefExpr->sym))
      {
        if (module->modTag == MOD_USER)
          evaluate(module, env);
      }
    }
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

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

    INT_ASSERT(type);
    INT_ASSERT(type->symbol);
    INT_ASSERT(type->symbol->name);

    if (strcmp(type->symbol->name, "bool") == 0)
      retval.bValue = imm->v_bool;

    else if (strcmp(type->symbol->name, "int")  == 0)
    {
      retval.iValue = imm->v_int64;
    }

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

  // NOAKES 2015/01/30.  Needs to be updated to track RETURN state
  for_alist(expr, blockStmt->body)
  {
    retval = evaluate(expr, env);
  }

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

  if (isImmediate(expr->var) == true)
  {
    VarSymbol* var  = toVarSymbol(expr->var);

    retval = evaluate(var, env);
  }

  else if (LcnSymbol* variable = toLcnSymbol(expr->var))
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

  if (LcnSymbol* sym = toLcnSymbol(defExpr->sym))
  {
    IpeValue value;

    if (defExpr->init == 0)
      value = evaluate(sym->type->defaultValue, env);

    else
      value = evaluate(defExpr->init,      env);

    env->bind(sym);
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
  IpeValue retval;

  if (callExpr->primitive == 0)
    retval = evaluateCall(callExpr, env);
  else
    retval = evaluatePrimop(callExpr, env);

  return retval;
}

static IpeValue evaluateCall(CallExpr* callExpr, IpeEnvironment* env)
{
  IpeValue retval;

  if (FnSymbol* fnSymbol = callExpr->isResolved())
  {
    int                     newEnvSize = environmentSize(fnSymbol);
    IpeEnvironment*         newEnv     = NULL;

    std::vector<LcnSymbol*> formals;
    std::vector<IpeValue>   actuals;

    for (int i = 0; i < callExpr->numActuals(); i++)
    {
      DefExpr*   defExpr = toDefExpr(fnSymbol->formals.get(i + 1));
      LcnSymbol* formal  = toLcnSymbol(defExpr->sym);

      INT_ASSERT(formal);

      formals.push_back(formal);
      actuals.push_back(evaluate(callExpr->get(i + 1), env));
    }

    newEnv = new IpeEnvironment(env,
                                formals,
                                actuals,
                                newEnvSize / 8,
                                newEnvSize);

    retval = evaluate(fnSymbol->body, newEnv);
  }
  else
  {
    AstDumpToNode logger(stdout);

    printf("evaluateCall(CallExpr, Env)\n\n");
    printf("Function has not been resolved\n");

    callExpr->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
    retval.iValue = 0;
  }

  return retval;
}

static IpeValue evaluatePrimop(CallExpr* callExpr, IpeEnvironment* env)
{
  bool     handled = false;
  IpeValue retval;

  if (false)
  {

  }

  else if (callExpr->isPrimitive(PRIM_ASSIGN) == true)
  {
    SymExpr*   dstSymExpr = toSymExpr(callExpr->get(1));
    LcnSymbol* variable   = toLcnSymbol(dstSymExpr->var);
    IpeValue   value      = evaluate(callExpr->get(2), env);

    INT_ASSERT(variable);

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

  // NOAKES 2015/01/30 This version only supports return of
  // value.  It does not handle flow of control
  else if (callExpr->isPrimitive(PRIM_RETURN) == true)
  {
    // This case implies a structured goto
    if (callExpr->numActuals() == 0)
    {
      INT_ASSERT(false);
    }
    else
    {
      retval  = evaluate(callExpr->get(1), env);
      handled = true;
    }
  }

  else
  {
    retval.iValue = 0;
  }

  if (handled == false)
  {
    AstDumpToNode logger(stdout, 3);

    printf("evaluatePrimop(CallExpr, Env)\n\n");
    printf("Failed to handle\n");

    printf("   ");
    callExpr->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int environmentSize(Symbol*       symbol,    int currMax);
static int environmentSize(LcnSymbol*    symbol,    int currMax);

static int environmentSize(Expr*         expr,      int currMax);

static int environmentSize(WhileDoStmt*  blockStmt, int currMax);
static int environmentSize(BlockStmt*    blockStmt, int currMax);
static int environmentSize(CondStmt*     condStmt,  int currMax);

static int environmentSize(DefExpr*      defExpr,   int currMax);
static int environmentSize(CallExpr*     callExpr,  int currMax);

static int environmentSize(FnSymbol* symbol)
{
  // NOAKES 2015/01/27 Values are currently fixed size
  int currMax = 8 * symbol->numFormals();

  return environmentSize(symbol->body, currMax);
}

static int environmentSize(Symbol* symbol, int currMax)
{
  if (false)
    ;

  else if (LcnSymbol*    sym = toLcnSymbol(symbol))
    currMax = environmentSize(sym, currMax);

  else if (FnSymbol*     sym = toFnSymbol(symbol))
    currMax = environmentSize(sym, currMax);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("environmentSize unhandled symbol\n");
    printf("   ");
    symbol->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return currMax;
}

// NOAKES 2015/01/26: LcnSymbol is currently of fixed size 8
static int environmentSize(LcnSymbol* symbol, int currMax)
{
  return symbol->offset() + 8;
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int environmentSize(Expr* expression, int currMax)
{
  if (false)
    ;

  else if (isSymExpr(expression) == true)
    ;

  else if (FnSymbol*    expr = toFnSymbol(expression))
    currMax = environmentSize(expr, currMax);

  else if (WhileDoStmt* expr = toWhileDoStmt(expression))
    currMax = environmentSize(expr, currMax);

  // This must appear after WhileDoStmt etc
  else if (BlockStmt*   expr = toBlockStmt(expression))
    currMax = environmentSize(expr, currMax);

  else if (CondStmt*    expr = toCondStmt(expression))
    currMax = environmentSize(expr, currMax);

  else if (DefExpr*     expr = toDefExpr(expression))
    currMax = environmentSize(expr, currMax);

  else if (CallExpr*    expr = toCallExpr(expression))
    currMax = environmentSize(expr, currMax);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("environmentSize unhandled expr\n");
    printf("   ");
    expression->accept(&logger);
    printf("\n\n\n");

    INT_ASSERT(false);
  }

  return currMax;
}

static int environmentSize(WhileDoStmt* whileDoStmt, int currMax)
{
  for_alist(expr, whileDoStmt->body)
    currMax = environmentSize(expr, currMax);

  return currMax;
}

static int environmentSize(BlockStmt* blockStmt, int currMax)
{
  for_alist(expr, blockStmt->body)
    currMax = environmentSize(expr, currMax);

  return currMax;
}

static int environmentSize(CondStmt* condStmt, int currMax)
{
  currMax = environmentSize(condStmt->thenStmt, currMax);

  if (condStmt->elseStmt != 0)
    currMax = environmentSize(condStmt->elseStmt, currMax);

  return currMax;
}

static int environmentSize(DefExpr* defExpr, int currMax)
{
  if (LcnSymbol* ipe = toLcnSymbol(defExpr->sym))
    currMax = environmentSize(ipe, currMax);

  return currMax;
}

static int environmentSize(CallExpr* callExpr, int currMax)
{
  return currMax;
}

static bool isImmediate(Symbol* sym)
{
  bool retval = false;

  if (VarSymbol* var = toVarSymbol(sym))
    retval = (var->immediate != 0) ? true : false;

  return retval;
}
