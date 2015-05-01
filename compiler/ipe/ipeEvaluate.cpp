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
#include "expr.h"
#include "IpeEnv.h"

IpeEnv* sFooEnv = NULL;

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

#include "IpeValue.h"

#include "ipeDriver.h"
#include "ipeResolve.h"

IpeValue evaluate(Expr* untypedExpr, IpeEnv* env)
{
  AstDumpToNode logger(stdout, 3);
  Expr*         exprRes = NULL;
  IpeValue      retval;

  if (gDebugLevelResolve > 0 || gDebugLevelEvaluate > 0)
  {
    printf("\n\n\n\nevaluate\n");

    printf("   untyped expr\n");
    printf("   ");
    untypedExpr->accept(&logger);
    printf("\n\n");

    printf("   env\n");
    env->describe(3);
    printf("\n\n");
  }

  exprRes = resolveExpr(untypedExpr, env);

  if (gDebugLevelResolve > 0 || gDebugLevelEvaluate > 0)
  {
    printf("   resolved expr\n");
    printf("   ");
    exprRes->accept(&logger);
    printf("\n\n");
  }

  if (exprRes != NULL)
    retval = evaluateExpr(exprRes, env);

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

#include "IpeDefExpr.h"
#include "IpeCallExpr.h"
#include "IpeMethod.h"
#include "IpeProcedure.h"
#include "ipeDriver.h"
#include "misc.h"
#include "stmt.h"
#include "WhileDoStmt.h"

static bool     isImmediate   (Expr*      expr);
static bool     isImmediate   (VarSymbol* var);
static IpeValue immediateValue(VarSymbol* var);

static IpeValue evaluateImmediate  (Expr*        expr);
static IpeValue evaluateDefExpr    (IpeDefExpr*  expr, IpeEnv* env);
static IpeValue evaluateCondStmt   (CondStmt*    expr, IpeEnv* env);
static IpeValue evaluateWhileDoStmt(WhileDoStmt* expr, IpeEnv* env);
static IpeValue evaluateBlockStmt  (BlockStmt*   expr, IpeEnv* env);
static IpeValue evaluateCallExpr   (IpeCallExpr* expr, IpeEnv* env);

IpeValue evaluateExpr(Expr* expr, IpeEnv* env)
{
  static int sCount = 0;
  int        count  = sCount;
  bool       debug  = (count >= 9) ? true : false;

  sCount = sCount + 1;

  IpeValue retval;

#if 0
  if (debug == true)
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n\n\n\n\n\n\n\n");
    printf("   evaluateExpr(Expr*, IpeEnv*)        %6d Enter\n", count);

    printf("   Expr\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    printf("   Env before\n");
    env->describe(3);
    printf("\n\n");
  }
#endif

  if (isImmediate(expr) == true)
    retval = evaluateImmediate(expr);

  else if (SymExpr*     sel = toSymExpr(expr))
  {
    LcnSymbol* sym = toLcnSymbol(sel->var);

    INT_ASSERT(sym);

    retval = env->fetch(sym);
  }

  else if (DefExpr*     sel = toDefExpr(expr))
  {
    IpeDefExpr* sel2 = (IpeDefExpr*) sel;

    retval = evaluateDefExpr(sel2, env);
  }

  else if (CallExpr*    sel = toCallExpr(expr))
  {
    IpeCallExpr* sel2 = (IpeCallExpr*) sel;

    INT_ASSERT(sel2);

    retval = evaluateCallExpr(sel2, env);
  }

  else if (CondStmt*    sel = toCondStmt(expr))
    retval = evaluateCondStmt(sel, env);

  else if (WhileDoStmt* sel = toWhileDoStmt(expr))
    retval = evaluateWhileDoStmt(sel, env);

  else if (BlockStmt*   sel = toBlockStmt(expr))
    retval = evaluateBlockStmt(sel, env);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   evaluateExpr(Expr*, Env*)  unsupported\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

#if 0
  if (debug == true)
  {
    printf("   evaluateExpr(Expr*, IpeEnv*)        %6d\n", count);
    printf("   Env after\n");
    env->describe(3);
    printf("\n\n");
  }
#else
  (void) debug;
#endif

  return retval;
}

static bool isImmediate(Expr* expr)
{
  bool retval = false;

  if (SymExpr* symExpr = toSymExpr(expr))
  {
    if (VarSymbol* var = toVarSymbol(symExpr->var))
      retval = var->isImmediate();
  }

  return retval;
}

static bool isImmediate(VarSymbol* var)
{
  return var->isImmediate();
}

static IpeValue evaluateImmediate(Expr* expr)
{
  IpeValue retval;

  if (SymExpr* symExpr = toSymExpr(expr))
  {
    if (VarSymbol* var = toVarSymbol(symExpr->var))
      retval = immediateValue(var);
  }

  return retval;
}

static IpeValue immediateValue(VarSymbol* var)
{
  INT_ASSERT(var->isImmediate());

  Immediate* imm    = var->immediate;
  Type*      type   = var->type;
  IpeValue   retval;

  INT_ASSERT(type);
  INT_ASSERT(type->symbol);
  INT_ASSERT(type->symbol->name);

  if      (strcmp(type->symbol->name, "bool")     == 0)
    retval.boolSet(imm->v_bool);

  else if (strcmp(type->symbol->name, "int")      == 0)
    retval.integerSet(imm->v_int64);

  else if (strcmp(type->symbol->name, "real")     == 0)
    retval.realSet(imm->v_float64);

  else if (strcmp(type->symbol->name, "c_string") == 0)
    retval.cstringSet(imm->v_string);

  else
    INT_ASSERT(false);

  return retval;
}

static IpeValue evaluateDefExpr(IpeDefExpr* defExpr, IpeEnv* env)
{
  IpeValue retval;

  if      (defExpr->moduleSymbolGet() != NULL)
  {

  }

  else if (FnSymbol*  fnSym = defExpr->fnSymbolGet())
  {
    VarSymbol*    var       = toVarSymbol(defExpr->sym);

    INT_ASSERT(var);
    INT_ASSERT(var->type == gIpeTypeProcedure);

    IpeProcedure* procedure = (IpeProcedure*) env->fetchPtr(var);
    IpeMethod*    method    = new IpeMethod(fnSym, env);

    procedure->methodAdd(method);
  }

  else if (VarSymbol* var   = toVarSymbol(defExpr->sym))
  {
    IpeValue value;

#if 0
  AstDumpToNode logger(stdout, 3);

  printf("\n\n\n\n\n\n\n\n\n");
  printf("   evaluateDefExpr\n");
  printf("   defExpr\n");
  printf("   ");
  defExpr->accept(&logger);
  printf("\n");
  printf("   env\n");
  env->describe(3);
  printf("\n");
#endif

    if (defExpr->init == NULL)
    {
      VarSymbol* defaultValue = toVarSymbol(var->type->defaultValue);

      if (isImmediate(defaultValue) == true)
        value = immediateValue(defaultValue);
      else
        value = env->fetch(defaultValue);
    }

    else
      value = evaluateExpr(defExpr->init, env);

    if (var->offset() < 0)
    {
      INT_ASSERT(env->depth() == 0);
      env->allocate(var, value);
    }
    else
    {
      env->store(var, value);
    }

#if 0
  printf("   env after\n");
  env->describe(3);
  printf("\n");
#endif


  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   evaluateDefExpr(DefExpr*, Env*)  unsupported\n");
    printf("   ");
    defExpr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluateCondStmt(CondStmt* expr, IpeEnv* env)
{
  IpeValue condValue = evaluateExpr(expr->condExpr, env);
  IpeValue retval;

  if      (condValue.boolGet() == true)
    retval = evaluateExpr(expr->thenStmt, env);

  else if (expr->elseStmt      != NULL)
    retval = evaluateExpr(expr->elseStmt, env);

  return retval;
}

static IpeValue evaluateWhileDoStmt(WhileDoStmt* whileDoStmt, IpeEnv* env)
{
  bool     proceed = true;
  IpeValue retval;

  while (proceed == true)
  {
    IpeValue cond = evaluateExpr(whileDoStmt->condExprGet(), env);

    proceed = cond.boolGet();

    if (proceed == true)
    {
      for (int i = 1; i <= whileDoStmt->body.length; i++)
        evaluateExpr(whileDoStmt->body.get(i), env);
    }
  }

  return retval;
}

static IpeValue evaluateBlockStmt(BlockStmt* expr, IpeEnv* env)
{
  IpeValue retval;

  for (int i = 1; i <= expr->body.length; i++)
    retval = evaluateExpr(expr->body.get(i), env);

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static IpeValue evaluateCall(IpeCallExpr* callExpr, IpeEnv* env);
static IpeValue evaluatePrim(IpeCallExpr* callExpr, IpeEnv* env);

static IpeValue evaluateCallExpr(IpeCallExpr* callExpr, IpeEnv* env)
{
  IpeValue retval;

  if (callExpr->baseExpr != NULL)
    retval = evaluateCall(callExpr, env);
  else
    retval = evaluatePrim(callExpr, env);

  return retval;
}

static IpeValue evaluateCall(IpeCallExpr* callExpr, IpeEnv* env)
{
  SymExpr* symExpr = toSymExpr(callExpr->baseExpr);
  IpeValue retval;

  INT_ASSERT(symExpr);

  if (VarSymbol* var = toVarSymbol(symExpr->var))
  {
    INT_ASSERT(var->type == gIpeTypeProcedure);

    IpeProcedure* ipeProcedure = (IpeProcedure*) env->fetchPtr(var);

    INT_ASSERT(ipeProcedure);

    if (ipeProcedure->isValid(callExpr->procedureGeneration()) == true)
    {
      int        methodId  = callExpr->methodId();
      IpeMethod* ipeMethod = ipeProcedure->methodGet(methodId);

      INT_ASSERT(ipeMethod);

      retval = ipeMethod->apply(callExpr, env);
    }

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("\n\n");
      printf("   evaluateCallExpr(IpeCallExpr*, Env*)  incomplete\n");
      printf("   ");
      callExpr->accept(&logger);
      printf("\n\n");

      ipeProcedure->describe(3);
      printf("\n");

      INT_ASSERT(false);
    }
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   evaluateCallExpr(IpeCallExpr*, Env*)  unexpected\n");
    printf("   ");
    callExpr->accept(&logger);
    printf("\n\n");
    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluatePrim(IpeCallExpr* callExpr, IpeEnv* env)
{
  IpeValue retval;

  if      (callExpr->isPrimitive(PRIM_ADDR_OF) == true)
  {
    SymExpr* sym = toSymExpr(callExpr->get(1));

    retval = env->addrOf(toVarSymbol(sym->var));
  }

  else if (callExpr->isPrimitive(PRIM_ASSIGN) == true)
  {
    SymExpr* dstExpr  = toSymExpr(callExpr->get(1));
    IpeValue value;

    Expr*    srcExpr  = callExpr->get(2);
    IpeValue srcValue = evaluateExpr(srcExpr, env);

    if      (ArgSymbol* dstSym = toArgSymbol(dstExpr->var))
    {
      value = env->fetch(dstSym);
    }

    else if (VarSymbol* dstSym = toVarSymbol(dstExpr->var))
    {
      value = env->fetch(dstSym);
    }

    else
      INT_ASSERT(false);

    *(value.refGet()) = srcValue;
  }

  else if (callExpr->isPrimitive(PRIM_UNARY_MINUS) == true)
  {
    if (gDebugLevelCalls > 1)
    {
      AstDumpToNode logger(stdout, 3);

      printf("   PRIM_UNARY_MINUS\n");
      printf("   ");
      callExpr->accept(&logger);
      printf("\n\n");
    }

    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(-1 * arg1.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(-1.0 * arg1.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_ADD) == true)
  {
    if (gDebugLevelCalls > 1)
    {
      AstDumpToNode logger(stdout, 3);

      printf("   PRIM_ADD\n");
      printf("   ");
      callExpr->accept(&logger);
      printf("\n\n");
    }

    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() + arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() + arg2.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_SUBTRACT) == true)
  {
    if (gDebugLevelCalls > 1)
    {
      AstDumpToNode logger(stdout, 3);

      printf("   PRIM_SUBTRACT\n");
      printf("   ");
      callExpr->accept(&logger);
      printf("\n\n");
    }

    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() - arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() - arg2.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_MULT) == true)
  {
    if (gDebugLevelCalls > 1)
    {
      AstDumpToNode logger(stdout, 3);

      printf("   PRIM_MULT\n");
      printf("   ");
      callExpr->accept(&logger);
      printf("\n\n");
    }

    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() * arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() * arg2.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_DIV) == true)
  {
    if (gDebugLevelCalls > 1)
    {
      AstDumpToNode logger(stdout, 3);

      printf("   PRIM_DIV\n");
      printf("   ");
      callExpr->accept(&logger);
      printf("\n\n");
    }

    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() / arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() / arg2.realGet());
  }

  //
  //
  //

  else if (callExpr->isPrimitive(PRIM_EQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "bool") == 0)
      retval.boolSet(arg1.boolGet()    == arg2.boolGet());

    else if (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() == arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    == arg2.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_NOTEQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "bool") == 0)
      retval.boolSet(arg1.boolGet()    != arg2.boolGet());

    else if (strcmp(type->symbol->name, "int") == 0)
      retval.boolSet(arg1.integerGet() != arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    != arg2.realGet());
  }

  //
  //
  //

  else if (callExpr->isPrimitive(PRIM_GREATER) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() >  arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    >  arg2.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_LESS) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() <  arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    <  arg2.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_GREATEROREQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() >= arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    >= arg2.realGet());
  }

  else if (callExpr->isPrimitive(PRIM_LESSOREQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = evaluateExpr(callExpr->get(1), env);
    IpeValue arg2 = evaluateExpr(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() <= arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    <= arg2.realGet());
  }

  //
  //
  //

  // NOAKES 2015/01/30 This version only supports return of value
  // It does not handle flow of control
  else if (callExpr->isPrimitive(PRIM_RETURN) == true)
  {
    // This case implies a structured goto
    if (callExpr->numActuals() == 0)
      INT_ASSERT(false);
    else
      retval = evaluateExpr(callExpr->get(1), env);
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeMethod::evaluate(CallExpr*, IpeEnv*)  unsupported\n");
    printf("   ");
    callExpr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}
