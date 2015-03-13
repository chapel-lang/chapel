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
#include "IpeModule.h"
#include "IpeProcedure.h"
#include "IpeVars.h"
#include "ipeDriver.h"
#include "misc.h"
#include "stmt.h"
#include "WhileDoStmt.h"

#include <cstdio>

static IpeValue evaluate(VarSymbol*    sym,  IpeVars* vars);
static IpeValue evaluate(FnSymbol*     sym,  IpeVars* vars);
static IpeValue evaluate(ModuleSymbol* sym,  IpeVars* vars);

static IpeValue evaluate(SymExpr*      expr, IpeVars* vars);
static IpeValue evaluate(DefExpr*      expr, IpeVars* vars);
static IpeValue evaluate(CallExpr*     expr, IpeVars* vars);

static IpeValue evaluate(BlockStmt*    expr, IpeVars* vars);
static IpeValue evaluate(CondStmt*     expr, IpeVars* vars);
static IpeValue evaluate(WhileDoStmt*  expr, IpeVars* vars);

static bool     isPrint (FnSymbol* fn);
static void     ipePrint(IpeValue value, Type* type);


static bool     isWriteln (FnSymbol* fn);
static void     ipeWriteln(IpeValue msg,   IpeValue value, Type* type);

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

IpeValue ipeEvaluate(Expr* expr, IpeVars* vars)
{
  IpeValue retval;

  if (gDebugLevelEvaluate > 0)
  {
    printf("ipeEvaluate\n");
    AstDumpToNode logger(stdout, 3);
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

#if 0
    IpeVars::describe(vars, 3);
    printf("\n\n");
#endif
  }

  if (false)
    ;

  else if (SymExpr*     sel = toSymExpr(expr))
    retval = evaluate(sel, vars);

  else if (DefExpr*     sel = toDefExpr(expr))
    retval = evaluate(sel, vars);

  else if (CallExpr*    sel = toCallExpr(expr))
    retval = evaluate(sel, vars);

  else if (CondStmt*    sel = toCondStmt(expr))
    retval = evaluate(sel, vars);

  else if (WhileDoStmt* sel = toWhileDoStmt(expr))
    retval = evaluate(sel, vars);

  // This must go after Loops etc
  else if (BlockStmt*   sel = toBlockStmt(expr))
    retval = evaluate(sel, vars);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   ipeEvaluate(Expr*, IpeVars*)  unsupported\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    IpeVars::describe(vars, 3);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluate(SymExpr* expr, IpeVars* vars)
{
  IpeValue retval;

  if      (ArgSymbol* sym = toArgSymbol(expr->var))
  {
    retval = IpeVars::fetch(sym, vars);
  }

  else if (VarSymbol* sym = toVarSymbol(expr->var))
  {
    retval = evaluate(sym, vars);
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("evaluate(SymExpr*, IpeVars*) Unsupported\n");

    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluate(DefExpr* defExpr, IpeVars* vars)
{
  IpeValue retval;

  if (false)
    ;

  else if (isTypeSymbol(defExpr->sym))
    ;

  else if (isFnSymbol(defExpr->sym))
    ;

  else if (ModuleSymbol* sym = toModuleSymbol(defExpr->sym))
    evaluate(sym, vars);

  else if (VarSymbol*    sym = toVarSymbol(defExpr->sym))
  {
    if (Immediate* imm = sym->immediate)
    {
      Type* type = sym->type;

      INT_ASSERT(type);
      INT_ASSERT(type->symbol);
      INT_ASSERT(type->symbol->name);

      if (strcmp(type->symbol->name, "bool") == 0)
      {
        IpeValue value(imm->v_bool);

        IpeVars::store(sym, value, vars);
      }

      else
      {
        INT_ASSERT(false);
      }
    }

    else
    {
      IpeValue value;

      if (defExpr->init == 0)
        value = ipeEvaluate(sym->type->defaultValue, vars);

      else
        value = ipeEvaluate(defExpr->init,           vars);

      IpeVars::store(sym, value, vars);
    }
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   evaluate(DefExpr*, IpeVars*) unsupported\n");
    printf("   ");
    defExpr->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);

  }

  return retval;
}

static IpeValue evaluate(BlockStmt* expr, IpeVars* vars)
{
  IpeValue retval;

  if (gDebugLevelEvaluate > 0)
  {
    AstDumpToNode logger(stdout, 3);
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");
  }

  for_alist(stmt, expr->body)
  {
    retval = ipeEvaluate(stmt, vars);
  }

  // NOAKES 2015/03/01  Needs to be updated to handle return stmt.
#if 0
  retval.iValue = 0;
#endif

  return retval;
}

static IpeValue evaluate(CondStmt* expr, IpeVars* vars)
{
  IpeValue condValue = ipeEvaluate(expr->condExpr, vars);
  IpeValue retval;

  if (condValue.boolGet() == true)
    retval = ipeEvaluate(expr->thenStmt, vars);

  else if (expr->elseStmt != 0)
    retval = ipeEvaluate(expr->elseStmt, vars);

  return retval;
}

static IpeValue evaluate(WhileDoStmt* whileDoStmt, IpeVars* vars)
{
  bool     proceed = true;
  IpeValue retval;

  while (proceed == true)
  {
    IpeValue cond = ipeEvaluate(whileDoStmt->condExprGet(), vars);

    proceed = cond.boolGet();

    if (proceed == true)
    {
      for_alist(expr, whileDoStmt->body)
        ipeEvaluate(expr, vars);
    }
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static IpeValue evaluateCall  (CallExpr* callExpr, IpeVars* vars);
static IpeValue evaluatePrimop(CallExpr* callExpr, IpeVars* vars);

static IpeValue evaluate(CallExpr* callExpr, IpeVars* vars)
{
  IpeValue retval;

  if (callExpr->primitive == 0)
    retval = evaluateCall  (callExpr, vars);
  else
    retval = evaluatePrimop(callExpr, vars);

  return retval;
}

static bool isReference(ArgSymbol* arg)
{
  return (arg->intent & INTENT_FLAG_REF) ? true : false;
}

static IpeValue evaluateCall(CallExpr* callExpr, IpeVars* vars)
{
  AstDumpToNode logger(stdout, 3);

  IpeValue      procValue = ipeEvaluate(callExpr->baseExpr, vars);
  IpeProcedure* proc      = procValue.procedureGet();
  FnSymbol*     fnSymbol  = (proc != 0) ? proc->fnSymbol() : 0;

  IpeVars*      locals    = 0;

  IpeValue      retval;

  INT_ASSERT(proc);

  proc->ensureBodyResolved();

  if (gDebugLevelCalls > 0)
  {
    proc->describe(3);
    printf("\n\n");

    printf("   ");
    callExpr->accept(&logger);
    printf("\n\n");
  }

  locals = IpeVars::allocate(proc->frameSize());

  for (int i = 1; i <= callExpr->numActuals(); i++)
  {
    DefExpr*   defExpr = toDefExpr(fnSymbol->formals.get(i));
    ArgSymbol* formal  = toArgSymbol(defExpr->sym);
    Expr*      actual  = callExpr->get(i);
    IpeValue   value;

    INT_ASSERT(formal);

    if (gDebugLevelCalls > 1)
    {
      printf("   Formal %3d\n", i);
      printf("   ");
      formal->accept(&logger);
      printf("\n");

      printf("   Actual %3d\n", i);
      printf("   ");
      actual->accept(&logger);
      printf("\n\n\n\n");
    }

    if (isReference(formal) == true)
    {
      SymExpr*   symExpr = toSymExpr(actual);
      INT_ASSERT(symExpr);

      VarSymbol* varSym  = toVarSymbol(symExpr->var);
      INT_ASSERT(symExpr);

      value = IpeVars::addrOf(varSym, vars);
    }
    else
    {
      value = ipeEvaluate(callExpr->get(i), vars);
    }

    IpeVars::store(formal, value, locals);
  }

  if (isPrint(fnSymbol) == true)
  {
    DefExpr*   defExpr = toDefExpr(fnSymbol->formals.get(1));
    ArgSymbol* formal  = toArgSymbol(defExpr->sym);

    ipePrint(locals->valueGet(0), formal->type);
  }

  else if (isWriteln(fnSymbol) == true)
  {
    if (fnSymbol->formals.length == 2)
    {
      DefExpr*   defExpr = toDefExpr(fnSymbol->formals.get(2));
      ArgSymbol* formal  = toArgSymbol(defExpr->sym);

      ipeWriteln(locals->valueGet(0), locals->valueGet(1), formal->type);
    }
    else
    {
      DefExpr*   defExpr = toDefExpr(fnSymbol->formals.get(1));
      ArgSymbol* formal  = toArgSymbol(defExpr->sym);
      IpeValue   dummy;

      ipeWriteln(locals->valueGet(0), dummy, formal->type);
    }
  }

  else
  {
    retval = evaluate(fnSymbol->body, locals);
  }

  IpeVars::deallocate(locals);

  return retval;
}

static IpeValue evaluatePrimop(CallExpr* callExpr, IpeVars* env)
{
  bool     handled = false;
  IpeValue retval;

  if (gDebugLevelCalls > 0)
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n\n\n");
    printf("******\n");
    printf("evaluatePrimop\n");
    printf("   ");
    callExpr->accept(&logger);
    printf("\n\n");
  }

  if (false)
  {

  }

  else if (callExpr->isPrimitive(PRIM_ASSIGN) == true)
  {
    if (gDebugLevelCalls > 1)
    {
      AstDumpToNode logger(stdout, 3);

      printf("   PRIM_ASSIGN\n");
      printf("   ");
      callExpr->accept(&logger);
      printf("\n\n");
    }

    IpeValue addr  = ipeEvaluate(callExpr->get(1), env);
    IpeValue value = ipeEvaluate(callExpr->get(2), env);

    if (gDebugLevelCalls > 1)
    {
      printf("   PRIM_ASSIGN   value.iValue = %ld\n", value.integerGet());
    }

    *addr.refGet() = value;
    handled        = true;
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
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(1 * arg1.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(-1.0 * arg1.realGet());

    handled       = true;
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
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() + arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() + arg2.realGet());

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_SUBTRACT) == true)
  {
    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() - arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() - arg2.realGet());

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_MULT) == true)
  {
    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() * arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() * arg2.realGet());

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_DIV) == true)
  {
    Type*    type = callExpr->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.integerSet(arg1.integerGet() / arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.realSet(arg1.realGet() / arg2.realGet());

    handled       = true;
  }

  else if (callExpr->isPrimitive(PRIM_EQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "bool") == 0)
      retval.boolSet(arg1.boolGet()    == arg2.boolGet());

    else if (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() == arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    == arg2.realGet());

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_NOTEQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if (strcmp(type->symbol->name, "bool") == 0)
      retval.boolSet(arg1.boolGet()    != arg2.boolGet());

    else if (strcmp(type->symbol->name, "int") == 0)
      retval.boolSet(arg1.integerGet() != arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    != arg2.realGet());

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_GREATER) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() >  arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    >  arg2.realGet());

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_LESS) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() <  arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    <  arg2.realGet());

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_GREATEROREQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() >= arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    >= arg2.realGet());

    handled = true;
  }

  else if (callExpr->isPrimitive(PRIM_LESSOREQUAL) == true)
  {
    Type*    type = callExpr->get(1)->typeInfo();
    IpeValue arg1 = ipeEvaluate(callExpr->get(1), env);
    IpeValue arg2 = ipeEvaluate(callExpr->get(2), env);

    if      (strcmp(type->symbol->name, "int")  == 0)
      retval.boolSet(arg1.integerGet() <= arg2.integerGet());

    else if (strcmp(type->symbol->name, "real") == 0)
      retval.boolSet(arg1.realGet()    <= arg2.realGet());

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
      retval  = ipeEvaluate(callExpr->get(1), env);
      handled = true;
    }
  }

  else if (callExpr->isPrimitive(PRIM_USE) == true)
  {
    if (SymExpr* sym = toSymExpr(callExpr->get(1)))
    {
      if (VarSymbol* var = toVarSymbol(sym->var))
      {
        IpeValue   value  = IpeVars::fetch(var, env);
        IpeModule* module = value.moduleGet();

        module->ensureInitialized(env);
        handled = true;
      }
      else
        INT_ASSERT(false);
    }
    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("   ");
      callExpr->get(1)->accept(&logger);
      printf("\n\n\n");

      INT_ASSERT(false);
    }
  }

  else
  {

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

IpeValue ipeEvaluate(Symbol* sym, IpeVars* env)
{
  IpeValue retval;

  if (false)
    ;

  else if  (FnSymbol*     sel = toFnSymbol(sym))
    retval = evaluate(sel, env);

  else if  (ModuleSymbol* sel = toModuleSymbol(sym))
    retval = evaluate(sel, env);

  else if  (VarSymbol*    sel = toVarSymbol(sym))
    retval = evaluate(sel, env);

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   ipeEvaluate(Symbol*, IpeVars*) unsupported\n");
    printf("   ");
    sym->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

static IpeValue evaluate(FnSymbol* sym, IpeVars* env)
{
  AstDumpToNode logger(stdout, 3);
  IpeValue      retval;

  printf("\n\n");
  printf("   evaluate(FnSymbol*, IpeVars*) unsupported\n");
  printf("   ");
  sym->accept(&logger);
  printf("\n\n");

  INT_ASSERT(false);

  return retval;
}

static IpeValue evaluate(ModuleSymbol* sym, IpeVars* env)
{
  IpeValue retval;

  for_alist(expr, sym->block->body)
    ipeEvaluate(expr, env);

  return retval;
}

static IpeValue evaluate(VarSymbol* sym, IpeVars* env)
{
  IpeValue retval;

  if (Immediate* imm = sym->immediate)
  {
    Type* type = sym->type;

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
  }

  else
    retval = IpeVars::fetch(sym, env);

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static bool isPrint(FnSymbol* fn)
{
  bool retval = false;

  if (strcmp(fn->name, "print") == 0)
  {
    if (ModuleSymbol* mod = fn->getModule())
      retval = (strcmp(mod->name, "ChapelBase") == 0) ? true : false;
  }

  return retval;
}

static void ipePrint(IpeValue value, Type* type)
{
  printf("     ");

  if      (type == dtBool)
    printf("bool  %s\n", (value.boolGet() == true) ? " true" : "false");

  else if (type == dtInt[INT_SIZE_64])
    printf("int   %5ld\n", value.integerGet());

  else if (type == dtReal[FLOAT_SIZE_64])
    printf("real  %6.2f\n", value.realGet());

  else if (type == dtStringC)
    printf("string \"%s\"\n", value.cstringGet());

  else
    printf("???\n\n");
}



static bool isWriteln(FnSymbol* fn)
{
  bool retval = false;

  if (strcmp(fn->name, "writeln") == 0)
  {
    if (ModuleSymbol* mod = fn->getModule())
      retval = (strcmp(mod->name, "ChapelBase") == 0) ? true : false;
  }

  return retval;
}

static void ipeWriteln(IpeValue msg,   IpeValue value, Type* type)
{
  printf("     ");
  fputs(msg.cstringGet(), stdout);

  if      (type == dtBool)
    printf("%s\n", (value.boolGet() == true) ? " true" : "false");

  else if (type == dtInt[INT_SIZE_64])
    printf("%5ld\n", value.integerGet());

  else if (type == dtReal[FLOAT_SIZE_64])
    printf("%6.2f\n", value.realGet());

  else if (type == dtStringC)
    printf("%s\n", msg.cstringGet());

  else
    printf("???\n");
}
