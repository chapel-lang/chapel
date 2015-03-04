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

// checkParsed.cpp

#include "passes.h"

#include "stmt.h"
#include "expr.h"
#include "astutil.h"
#include "stlUtil.h"


static void checkNamedArguments(CallExpr* call);
static void checkParsedVar(VarSymbol* var);
static void checkFunction(FnSymbol* fn);
static void checkExportedNames();
static void checkModule(ModuleSymbol* mod);

void
checkParsed() {
  forv_Vec(CallExpr, call, gCallExprs) {
    checkNamedArguments(call);
  }

  forv_Vec(DefExpr, def, gDefExprs) {
    if (toVarSymbol(def->sym))
      // The test for FLAG_TEMP allows compiler-generated (temporary) variables
      // to be declared without an explicit type or initializer expression.
      if ((!def->init || def->init->isNoInitExpr())
          && !def->exprType && !def->sym->hasFlag(FLAG_TEMP))
        if (isBlockStmt(def->parentExpr) && !isArgSymbol(def->parentSymbol))
          if (def->parentExpr != rootModule->block)
            if (!def->sym->hasFlag(FLAG_INDEX_VAR))
              USR_FATAL_CONT(def->sym,
                             "Variable '%s' is not initialized or has no type",
                             def->sym->name);
  }

  forv_Vec(VarSymbol, var, gVarSymbols) {
    checkParsedVar(var);
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkFunction(fn);
  }

  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    checkModule(mod);
  }

  checkExportedNames();
}


static void
checkNamedArguments(CallExpr* call) {
  Vec<const char*> names;

  for_actuals(expr, call) {
    if (NamedExpr* named = toNamedExpr(expr)) {
      forv_Vec(const char, name, names) {
        if (!strcmp(name, named->name))
          USR_FATAL_CONT(named,
                         "The named argument '%s' is used more "
                         "than once in the same function call.",
                         name);
      }

      names.add(named->name);
    }
  }
}


static void
checkParsedVar(VarSymbol* var) {
  if (var->isParameter() && !var->immediate)
    if (!var->defPoint->init &&
        (toFnSymbol(var->defPoint->parentSymbol) ||
         toModuleSymbol(var->defPoint->parentSymbol)))
      USR_FATAL_CONT(var, "Top-level params must be initialized.");

  if (var->defPoint->init && var->defPoint->init->isNoInitExpr()) {
    if (var->hasFlag(FLAG_CONST))
      USR_FATAL_CONT(var, "const variables specified with noinit must be explicitly initialized.");
  }

  //
  // Verify that config variables are only at Module scope i.e. it is
  // an error if any config variable is not an immediate child of a
  // module

  if (var->hasFlag(FLAG_CONFIG) &&
      isModuleSymbol(var->defPoint->parentSymbol) == false) {
    const char* varType = NULL;

    if (var->hasFlag(FLAG_PARAM))
      varType = "parameters";
    else if (var->hasFlag(FLAG_CONST))
      varType = "constants";
    else
      varType = "variables";

    USR_FATAL_CONT(var->defPoint,
                   "Configuration %s are allowed only at module scope.", varType);
  }
}


static void
checkFunction(FnSymbol* fn) {
  // Ensure that the lhs of "=" and "<op>=" is passed by ref.
  if (fn->hasFlag(FLAG_ASSIGNOP))
    if (fn->getFormal(1)->intent != INTENT_REF)
      USR_WARN(fn, "The left operand of '=' and '<op>=' should have 'ref' intent.");

  if (!strcmp(fn->name, "this") && fn->hasFlag(FLAG_NO_PARENS))
    USR_FATAL_CONT(fn, "method 'this' must have parentheses");

  if (!strcmp(fn->name, "these") && fn->hasFlag(FLAG_NO_PARENS))
    USR_FATAL_CONT(fn, "method 'these' must have parentheses");

  if (fn->retTag == RET_PARAM && fn->retExprType != NULL)
    USR_WARN(fn, "providing an explicit return type on a 'param' function currently leads to incorrect results; as a workaround, remove the return type specification in function '%s'", fn->name);

  std::vector<CallExpr*> calls;
  collectMyCallExprsSTL(fn, calls, fn);
  bool isIterator = fn->isIterator();
  bool notInAFunction = !isIterator && (fn->getModule()->initFn == fn);
  int numVoidReturns = 0, numNonVoidReturns = 0, numYields = 0;

  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_RETURN)) {
      if (notInAFunction)
        USR_FATAL_CONT(call, "return statement is not in a function or iterator");
      else {
        SymExpr* sym = toSymExpr(call->get(1));
        if (sym && sym->var == gVoid)
          numVoidReturns++;
        else {
          if (isIterator)
            USR_FATAL_CONT(call, "returning a value in an iterator");
          else
            numNonVoidReturns++;
        }
      }
    }
    else if (call->isPrimitive(PRIM_YIELD)) {
      if (notInAFunction)
        USR_FATAL_CONT(call, "yield statement is outside an iterator");
      else if (!isIterator)
        USR_FATAL_CONT(call, "yield statement is in a non-iterator function");
      else
        numYields++;
    }
  }

  if (numVoidReturns != 0 && numNonVoidReturns != 0)
    USR_FATAL_CONT(fn, "Not all returns in this function return a value");
  if (isIterator && numYields == 0)
    USR_FATAL_CONT(fn, "iterator does not yield a value");
  if (!isIterator &&
      fn->retTag == RET_REF &&
      numNonVoidReturns == 0) {
    USR_FATAL_CONT(fn, "function declared 'var' but does not return anything");
  }
}

//
// This is a special test to ensure that there are no instances of a return
// or yield statement at the top level of a module.  This "special" semantic
// check is needed to resolve 4 test applications that failed once the
// insertion of a Module init function was moved to a later pass.
//
// Those tests have historically relied on the matching call in checkFunction
// which was executed when scanning the module initFunction.
//
// This is probably a good anchor for a family of tests of this form.
//

static void
checkModule(ModuleSymbol* mod) {
  for_alist(stmt, mod->block->body) {
    if (CallExpr* call = toCallExpr(stmt)) {
      if (call->isPrimitive(PRIM_RETURN)) {
        USR_FATAL_CONT(call, "return statement is not in a function or iterator");

      } else if (call->isPrimitive(PRIM_YIELD)) {
        USR_FATAL_CONT(call, "yield statement is outside an iterator");
      }
    }
  }
}

static void
checkExportedNames()
{
  // The right side of the map is a dummy Boolean.
  // We are just using the map to implement a set.
  HashMap<const char*, StringHashFns, bool> names;
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->hasFlag(FLAG_EXPORT))
      continue;

    const char* name = fn->cname;
    if (names.get(name))
      USR_FATAL_CONT(fn, "The name %s cannot be exported twice from the same compilation unit.", name);
    names.put(name, true);
  }
}


