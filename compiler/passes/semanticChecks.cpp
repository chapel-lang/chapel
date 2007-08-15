#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symscope.h"
#include "stringutil.h"


static void
check_redefinition(Symbol* sym) {
  if (sym->isCompilerTemp)
    return;
  if (sym->parentScope == theProgram->block->blkScope)
    return;
  if (sym->overloadNext) {
    int count = 0;
    for (Symbol* tmp = sym; tmp; tmp = tmp->overloadNext) {
      if (!tmp->getFnSymbol()) {
        count++;
      }
    }
    if (count >= 2) {
      const char* redefinitionLocations = "";
      for (Symbol* tmp = sym->overloadNext; tmp; tmp = tmp->overloadNext) {
        if (!tmp->getFnSymbol()) {
          redefinitionLocations =
            stringcat(redefinitionLocations, "\n  ", tmp->stringLoc());
        }
      }
      USR_FATAL(sym, "'%s' has multiple definitions, redefined at:%s",
                sym->name, redefinitionLocations);
    }
  }
}


static void
check_functions(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  Vec<CallExpr*> rets;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* ret = toCallExpr(ast))
      if (ret->isPrimitive(PRIMITIVE_RETURN) && ret->parentSymbol == fn)
        rets.add(ret);
  }
  if (rets.n == 0)
    return;
  bool returns_void = false;
  forv_Vec(CallExpr, ret, rets) {
    if (SymExpr* sym = toSymExpr(ret->get(1)))
      if (sym->var == gVoid)
        returns_void = true;
  }
  forv_Vec(CallExpr, ret, rets) {
    if (returns_void && ret->typeInfo() != dtVoid) {
      if (fn->getModule()->initFn == fn) {
        USR_FATAL(ret, "return statement is not in a function");
      } else {
        USR_FATAL(fn, "Not all function returns return a value");
      }
    }
  }
}


static void
check_parsed_vars(VarSymbol* var) {
  if (var->isParam() && !var->immediate)
    if (!var->defPoint->init &&
        (toFnSymbol(var->defPoint->parentSymbol) ||
         toModuleSymbol(var->defPoint->parentSymbol)) &&
        !toFnSymbol(var->defPoint->parentScope->astParent))
      USR_FATAL(var, "Top-level params must be initialized.");
  if (var->varClass == VAR_CONFIG &&
      var->defPoint->parentSymbol != var->getModule()->initFn) {
    const char *varType = NULL;
    switch (var->consClass) {
      case VAR_VAR:   varType = "variables"; break;
      case VAR_CONST: varType = "constants"; break;
      case VAR_PARAM: varType = "parameters"; break;
      default: INT_FATAL("Illegal constant class.");
    }
    USR_FATAL_CONT(var->defPoint,
                   "Configuration %s only allowed at module scope.", varType);
  }
}


static void
check_named_arguments(CallExpr* call) {
  Vec<const char*> names;
  for_actuals(expr, call) {
    if (NamedExpr* named = toNamedExpr(expr)) {
      forv_Vec(const char, name, names) {
        if (!strcmp(name, named->name))
          USR_FATAL(named, "The named argument '%s' is used more "
                    "than once in the same function call.", name);
      }
      names.add(named->name);
    }
  }
}


void
checkParsed(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {

    if (CallExpr* call = toCallExpr(ast))
      check_named_arguments(call);

    if (DefExpr* def = toDefExpr(ast)) {
      if (!strcmp(def->sym->name, "_")) {
        USR_FATAL("Symbol cannot be named \"_\"");
      } else if (toVarSymbol(def->sym)) {
        if (def->sym->hasPragma("internal var"))
          def->sym->isCompilerTemp = true;
        if (!def->init && !def->exprType && !def->sym->isCompilerTemp)
          if (toBlockStmt(def->parentExpr))
            USR_FATAL_CONT(def->sym,
                           "Variable '%s' is not initialized or has no type",
                           def->sym->name);
      }
    }

    if (Symbol* sym = toSymbol(ast))
      check_redefinition(sym);

    if (VarSymbol* a = toVarSymbol(ast))
      check_parsed_vars(a);
    else if (FnSymbol* fn = toFnSymbol(ast))
      check_functions(fn);
  }
}


void
checkNormalized(void) {
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->fnClass == FN_ITERATOR) {
      for_formals(formal, fn) {
        if (formal->intent == INTENT_IN ||
            formal->intent == INTENT_INOUT ||
            formal->intent == INTENT_OUT ||
            formal->intent == INTENT_REF) {
          USR_FATAL(formal, "formal argument of iterator cannot have intent");
        }
      }
    }
  }
}


static void
check_resolved_syms(Symbol* var) {
  if (!var->isConst() && !var->isParam())
    return;
  if (VarSymbol* vs = toVarSymbol(var))
    if (vs->immediate)
      return;
  if (var->defs.n > 1)
    USR_FATAL_CONT(var->defs.v[var->defs.n-1],
                   "Assigning to a constant expression");
  // need something like below to check constant assignment via refs
  forv_Vec(SymExpr, use, var->uses) {
    if (CallExpr* call = toCallExpr(use->parentExpr))
      if (call->isPrimitive(PRIMITIVE_SET_REF))
        if (CallExpr* move = toCallExpr(call->parentExpr))
          if (move->isPrimitive(PRIMITIVE_MOVE))
            if (SymExpr* lhs = toSymExpr(move->get(1)))
              if (lhs->var->defs.n > 1)
                USR_FATAL_CONT(lhs->var->defs.v[lhs->var->defs.n-1], "Assigning to a constant expression");
  }
}


void
checkResolved(void) {
  compute_sym_uses();
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (toVarSymbol(ast) || toArgSymbol(ast))
      check_resolved_syms(toSymbol(ast));
  }

  forv_Vec(TypeSymbol, type, gTypes) {
    if (EnumType* et = toEnumType(type->type)) {
      for_defs(def, et->constants) {
        if (def->init) {
          SymExpr* sym = toSymExpr(def->init);
          if (!sym || (toVarSymbol(sym->var)->consClass != VAR_PARAM &&
                       !toVarSymbol(sym->var)->immediate))
            USR_FATAL(def, "enumerator '%s' is not an int parameter", def->sym->name);
        }
      }
    }
  }
}
