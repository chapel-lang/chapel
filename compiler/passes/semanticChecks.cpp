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
  if (sym->parentScope == rootScope)
    return;
  if (sym->overloadNext) {
    int count = 0;
    for (Symbol* tmp = sym; tmp; tmp = tmp->overloadNext) {
      if (!tmp->getFnSymbol()) {
        count++;
      }
    }
    if (count >= 2) {
      char* redefinitionLocations = "";
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
    if (CallExpr* ret = dynamic_cast<CallExpr*>(ast))
      if (ret->isPrimitive(PRIMITIVE_RETURN) && ret->parentSymbol == fn)
        rets.add(ret);
  }
  if (rets.n == 0)
    return;
  bool returns_void = false;
  forv_Vec(CallExpr, ret, rets) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(ret->get(1)))
      if (sym->var == gVoid)
        returns_void = true;
  }
  forv_Vec(CallExpr, ret, rets) {
    if (returns_void && ret->typeInfo() != dtVoid)
      if (fn->getModule()->initFn == fn)
        USR_FATAL(ret, "return statement is not in a function");
      else
        USR_FATAL(fn, "Not all function returns return a value");
  }
}


static void
check_parsed_vars(VarSymbol* var) {
  if (var->isParam() && !var->immediate)
    if (!var->defPoint->init &&
        (dynamic_cast<FnSymbol*>(var->defPoint->parentSymbol) ||
         dynamic_cast<ModuleSymbol*>(var->defPoint->parentSymbol)) &&
        !dynamic_cast<FnSymbol*>(var->defPoint->parentScope->astParent))
      USR_FATAL(var, "Top-level params must be initialized.");
  if (var->varClass == VAR_CONFIG &&
      var->defPoint->parentSymbol != var->getModule()->initFn) {
    char *varType = NULL;
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
  Vec<char*> names;
  for_actuals(expr, call) {
    if (NamedExpr* named = dynamic_cast<NamedExpr*>(expr)) {
      forv_Vec(char, name, names) {
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

    if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
      check_named_arguments(call);

    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (!strcmp(def->sym->name, "_")) {
        USR_FATAL("Symbol cannot be named \"_\"");
      } else if (dynamic_cast<VarSymbol*>(def->sym)) {
        if (def->sym->hasPragma("internal var"))
          def->sym->isCompilerTemp = true;
        if (!def->init && !def->exprType && !def->sym->isCompilerTemp)
          if (dynamic_cast<BlockStmt*>(def->parentExpr))
            USR_FATAL_CONT(def->sym,
                           "Variable '%s' is not initialized or has no type",
                           def->sym->name);
      }
    }

    if (Symbol* sym = dynamic_cast<Symbol*>(ast))
      check_redefinition(sym);

    if (VarSymbol* a = dynamic_cast<VarSymbol*>(ast))
      check_parsed_vars(a);
    else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast))
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

    // do not check body of nested function (would be again)
    if (fn->defPoint->parentSymbol->astType == SYMBOL_FN)
      continue;

    ModuleSymbol* mod = fn->getModule();
    Vec<char*> reported;
    Vec<BaseAST*> asts;
    Vec<Symbol*> defined;
    collect_asts_postorder(&asts, fn);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr))
          if (dynamic_cast<ModuleSymbol*>(base->var))
            USR_FATAL_CONT(call, "illegal use of module '%s'", base->var->name);
        if (call->isPrimitive(PRIMITIVE_MOVE))
          defined.set_add(dynamic_cast<SymExpr*>(call->get(1))->var);
      } else if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
        if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr))
          if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == sym)
            continue;
        if (dynamic_cast<VarSymbol*>(sym->var))
          if (sym->var->defPoint &&
              (sym->var->defPoint->parentSymbol == fn ||
               (sym->var->defPoint->parentSymbol == mod && mod->initFn == fn)))
            if (!defined.set_in(sym->var))
              if (sym->var != fn->_this)
                USR_FATAL(sym, "'%s' used before defined", sym->var->name);
        CallExpr* parent = dynamic_cast<CallExpr*>(sym->parentExpr);
        if (!(parent && parent->baseExpr == sym))
          if (dynamic_cast<UnresolvedSymbol*>(sym->var)) {
            if (!reported.set_in(sym->var->name)) {
              USR_FATAL_CONT(sym, "'%s' undeclared (first use this function)",
                             sym->var->name);
              reported.set_add(sym->var->name);
            }
          }
      }
    }
  }
}


static void
check_resolved_syms(Symbol* var) {
  if (!var->isConst() && !var->isParam())
    return;
  if (VarSymbol* vs = dynamic_cast<VarSymbol*>(var))
    if (vs->immediate)
      return;
  if (var->defs.n > 1)
    USR_FATAL_CONT(var->defs.v[var->defs.n-1],
                   "Assigning to a constant expression");
  // need something like below to check constant assignment via refs
  forv_Vec(SymExpr, use, var->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(use->parentExpr))
      if (call->isPrimitive(PRIMITIVE_SET_REF))
        if (CallExpr* move = dynamic_cast<CallExpr*>(call->parentExpr))
          if (move->isPrimitive(PRIMITIVE_MOVE))
            if (SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1)))
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
    if (dynamic_cast<VarSymbol*>(ast) || dynamic_cast<ArgSymbol*>(ast))
      check_resolved_syms(dynamic_cast<Symbol*>(ast));
  }

  forv_Vec(TypeSymbol, type, gTypes) {
    if (EnumType* et = dynamic_cast<EnumType*>(type->type)) {
      for_alist(DefExpr, def, et->constants) {
        if (def->init) {
          SymExpr* sym = dynamic_cast<SymExpr*>(def->init);
          if (!sym || (dynamic_cast<VarSymbol*>(sym->var)->consClass != VAR_PARAM &&
                       !dynamic_cast<VarSymbol*>(sym->var)->immediate))
            USR_FATAL(def, "enumerator '%s' is not an int parameter", def->sym->name);
        }
      }
    }
  }
}
