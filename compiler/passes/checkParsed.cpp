// checkParsed.cpp

#include "passes.h"

#include "stmt.h"
#include "expr.h"
#include "astutil.h"


static void check_named_arguments(CallExpr* call);
static void check_parsed_vars(VarSymbol* var);
static void check_functions(FnSymbol* fn);
static void check_exported_names();


void
checkParsed(void) {
  forv_Vec(CallExpr, call, gCallExprs) {
    check_named_arguments(call);
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
    check_parsed_vars(var);
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    check_functions(fn);
  }

  check_exported_names();
}


static void
check_named_arguments(CallExpr* call) {
  Vec<const char*> names;
  for_actuals(expr, call) {
    if (NamedExpr* named = toNamedExpr(expr)) {
      forv_Vec(const char, name, names) {
        if (!strcmp(name, named->name))
          USR_FATAL_CONT(named, "The named argument '%s' is used more "
                    "than once in the same function call.", name);
      }
      names.add(named->name);
    }
  }
}


static void
check_parsed_vars(VarSymbol* var) {
  if (var->isParameter() && !var->immediate)
    if (!var->defPoint->init &&
        (toFnSymbol(var->defPoint->parentSymbol) ||
         toModuleSymbol(var->defPoint->parentSymbol)))
      USR_FATAL_CONT(var, "Top-level params must be initialized.");
  if (var->defPoint->init && var->defPoint->init->isNoInitExpr()) {
    if (var->hasFlag(FLAG_CONST))
      USR_FATAL_CONT(var, "const variables specified with noinit must be explicitly initialized.");
  }
  if (var->hasFlag(FLAG_CONFIG) &&
      var->defPoint->parentSymbol != var->getModule()->initFn) {
    const char *varType = NULL;
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
check_functions(FnSymbol* fn) {
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

  Vec<CallExpr*> calls;
  collectMyCallExprs(fn, calls, fn);
  bool isIterator = fn->hasFlag(FLAG_ITERATOR_FN);
  bool notInAFunction = !isIterator && (fn->getModule()->initFn == fn);
  int numVoidReturns = 0, numNonVoidReturns = 0, numYields = 0;

  forv_Vec(CallExpr, call, calls) {
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
      fn->retTag == RET_VAR && 
      numNonVoidReturns == 0) {
    USR_FATAL_CONT(fn, "function declared 'var' but does not return anything");
  }
}


static void
check_exported_names()
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


