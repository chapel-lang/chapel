#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "stringutil.h"


static void
check_functions(FnSymbol* fn) {
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
check_parsed_vars(VarSymbol* var) {
  if (var->isParameter() && !var->immediate)
    if (!var->defPoint->init &&
        (toFnSymbol(var->defPoint->parentSymbol) ||
         toModuleSymbol(var->defPoint->parentSymbol)))
      USR_FATAL_CONT(var, "Top-level params must be initialized.");
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


void
checkParsed(void) {
  forv_Vec(CallExpr, call, gCallExprs) {
    check_named_arguments(call);
  }

  forv_Vec(DefExpr, def, gDefExprs) {
    if (toVarSymbol(def->sym))
      if (!def->init && !def->exprType && !def->sym->hasFlag(FLAG_TEMP))
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


void
checkNormalized(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      // <hilde:2011-10-13> Removed check that iterator formals have no intents.
      if (fn->retTag == RET_TYPE)
        USR_FATAL_CONT(fn, "iterators may not yield or return types");
      if (fn->retTag == RET_PARAM)
        USR_FATAL_CONT(fn, "iterators may not yield or return parameters");
    } else if (fn->hasFlag(FLAG_CONSTRUCTOR) &&
               !fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)) {
      if (fn->retExprType)
        USR_FATAL_CONT(fn, "constructors may not declare a return type");
      for_formals(formal, fn) {
        Vec<SymExpr*> symExprs;
        collectSymExprs(formal, symExprs);
        forv_Vec(SymExpr, se, symExprs) {
          if (se->var == fn->_this) {
            USR_FATAL_CONT(se, "invalid access of class member in constructor header");
            break;
          }
        }
      }
    } else if (fn->hasFlag(FLAG_DESTRUCTOR)) {
      if (fn->retExprType)
        USR_FATAL_CONT(fn, "destructors may not declare a return type");
    }
  }
}


static int
isDefinedAllPaths(Expr* expr, Symbol* ret) {
  if (!expr)
    return 0;
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_MOVE) || call->isNamed("="))
      if (SymExpr* lhs = toSymExpr(call->get(1)))
        if (lhs->var == ret)
          return 1 + isDefinedAllPaths(expr->next, ret);
    //
    // should mark functions that exit rather than relying on string
    //
    if (call->isNamed("halt"))
      return 1 + isDefinedAllPaths(expr->next, ret);
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    if (!block->blockInfo ||
        block->blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP))
      if (int result = isDefinedAllPaths(block->body.head, ret))
        return result;
  } else if (isGotoStmt(expr)) {
    return 0;
  } else if (CondStmt* cond = toCondStmt(expr)) {
    if (isDefinedAllPaths(cond->thenStmt, ret) &&
        isDefinedAllPaths(cond->elseStmt, ret))
      return 1;
  }
  return isDefinedAllPaths(expr->next, ret);
}


static void
checkReturnPaths(FnSymbol* fn) {
  // Check to see if the function returns a value.
  if (fn->hasFlag(FLAG_ITERATOR_FN) ||
      !strcmp(fn->name, "=") ||
      !strcmp(fn->name, "chpl__buildArrayRuntimeType") ||
      fn->retType == dtVoid ||
      fn->retTag == RET_TYPE ||
      fn->hasFlag(FLAG_EXTERN) ||
      fn->hasFlag(FLAG_FUNCTION_PROTOTYPE) ||
      fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) ||
      fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) ||
      fn->hasFlag(FLAG_AUTO_II))
    return; // No.

  // Check to see if the returned value is initialized.
  Symbol* ret = fn->getReturnSymbol();
  VarSymbol* var = toVarSymbol(ret);
  if (var)
  {
    // If it has an immediate initializer, it is initialized.
    if (var->immediate)
      return;
  }

  if (isEnumSymbol(ret))
    return;

  int result = isDefinedAllPaths(fn->body, ret);

  //
  // Issue a warning if there is a path that has zero definitions or
  // there is a path that has one definition and the function has a
  // specified return type; we care about there being a specified
  // return type because this specified return type is used to
  // initialize the return symbol but we don't want that to count as a
  // definition of a return value.
  //
  if (result == 0 || (result == 1 && fn->hasFlag(FLAG_SPECIFIED_RETURN_TYPE)))
    USR_WARN(fn->body, "control reaches end of function that returns a value");
}


void
checkResolved(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkReturnPaths(fn);
    if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
        !fn->hasFlag(FLAG_ITERATOR_FN) &&
        fn->retType->initializer->defPoint->parentSymbol == fn)
      USR_FATAL_CONT(fn, "functions cannot return nested iterators or loop expressions");
  }

  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (EnumType* et = toEnumType(type->type)) {
      for_enums(def, et) {
        if (def->init) {
          SymExpr* sym = toSymExpr(def->init);
          if (!sym || (!sym->var->hasFlag(FLAG_PARAM) &&
                       !toVarSymbol(sym->var)->immediate))
            USR_FATAL_CONT(def, "enumerator '%s' is not an integer param value", 
                           def->sym->name);
        }
      }
    }
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    if (!call->isResolved())
      continue;

    if (call->isResolved()->hasFlag(FLAG_DESTRUCTOR)) {
      // Statements of the form 'delete x' (PRIM_DELETE) are replaced
      //  during the normalize pass with a call to the destructor
      //  followed by a call to chpl_mem_free(), so here we just check
      //  if the type of the variable being passed to chpl_mem_free()
      //  is a record.
      if (isRecord(call->get(1)->typeInfo()->getValType()))
        USR_FATAL_CONT(call, "delete not allowed on records");
    }
  }
}
