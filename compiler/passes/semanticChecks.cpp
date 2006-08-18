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
  if (sym->overload) {
    int count = 0;
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (!tmp->getFnSymbol()) {
        count++;
      }
    }
    if (count >= 2) {
      char* redefinitionLocations = "";
      for (Symbol* tmp = sym->overload; tmp; tmp = tmp->overload) {
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
check_returns(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  Vec<ReturnStmt*> rets;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* ret = dynamic_cast<ReturnStmt*>(ast)) {
      if (ret->parentSymbol == fn)
        rets.add(ret);
    }
  }
  if (rets.n == 0)
    return;
  bool returns_void = false;
  forv_Vec(ReturnStmt, ret, rets) {
    if (ret->returnsVoid())
      returns_void = true;
  }
  forv_Vec(ReturnStmt, ret, rets) {
    if (returns_void && !ret->returnsVoid())
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
}


static void
check_named_arguments(CallExpr* call) {
  Vec<char*> names;
  for_alist(Expr, expr, call->argList) {
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
check_parsed(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {

    if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
      check_named_arguments(call);

    if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
      if (dynamic_cast<VarSymbol*>(def->sym))
        if (dynamic_cast<FnSymbol*>(def->parentSymbol) ||
            dynamic_cast<ModuleSymbol*>(def->parentSymbol))
          if (!def->init && !def->exprType && !def->sym->isCompilerTemp)
            if (!dynamic_cast<CallExpr*>(def->parentExpr))
              USR_FATAL_CONT(def->sym,
                             "Variable '%s' is not initialized or has no type",
                             def->sym->name);

    if (Symbol* sym = dynamic_cast<Symbol*>(ast))
      check_redefinition(sym);

    if (VarSymbol* a = dynamic_cast<VarSymbol*>(ast))
      check_parsed_vars(a);
    else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast))
      check_returns(fn);
  }
}


static void
check_normalized_calls(CallExpr* call) {
  if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
    if (dynamic_cast<ModuleSymbol*>(base->var)) {
      USR_FATAL_CONT(call, "Illegal call of module %s", base->var->name);
    }
  }
}


static void
check_normalized_vars(Symbol* var) {
  if (!var->isConst() && !var->isParam())
    return;
  int num_moves = 0;
  CallExpr* move = 0;
  forv_Vec(SymExpr*, sym, var->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == sym) {
        num_moves++;
        move = call;
      }
    }
  }
  if (num_moves >= 3) {
    USR_FATAL_CONT(move, "Assigning to a constant expression");
  }
  if (num_moves >= 1 && dynamic_cast<ArgSymbol*>(var)) {
    USR_FATAL_CONT(move, "Assigning to a constant expression");
  }
}


static void
check_normalized_functions(FnSymbol* fn) {
  if (fn->noParens && !fn->_this)
    USR_FATAL_CONT(fn, "Non-member functions must have parenthesized argument lists");
}


static void
check_normalized_def_before_use(FnSymbol* fn) {
  Vec<Symbol*> defined;
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_MOVE))
        defined.set_add(dynamic_cast<SymExpr*>(call->get(1))->var);
    } else if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr))
        if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == sym)
          continue;
      if (dynamic_cast<VarSymbol*>(sym->var))
        if (sym->var->defPoint && sym->var->defPoint->parentSymbol == fn)
          if (!defined.set_in(sym->var))
            USR_FATAL(sym, "Variable '%s' used before defined", sym->var->name);
    }
  }
}


void
check_normalized_enum(EnumType* et) {
  for_alist(DefExpr, def, et->constants) {
    if (def->init) {
      SymExpr* sym = dynamic_cast<SymExpr*>(def->init);
      if (!sym || (dynamic_cast<VarSymbol*>(sym->var)->consClass != VAR_PARAM &&
                   !dynamic_cast<VarSymbol*>(sym->var)->immediate))
        USR_FATAL(def, "Enumerator value for %s must be int parameter", def->sym->name);
    }
  }
}


void
check_normalized(void) {
  compute_sym_uses();
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      check_normalized_calls(a);
    } else if (VarSymbol* a = dynamic_cast<VarSymbol*>(ast)) {
      if (!a->immediate)
        check_normalized_vars(a);
    } else if (ArgSymbol* a = dynamic_cast<ArgSymbol*>(ast)) {
      check_normalized_vars(a);
    } else if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast)) {
      check_normalized_functions(a);
      check_normalized_def_before_use(a);
    } else if (SymExpr* a = dynamic_cast<SymExpr*>(ast)) {
      CallExpr* parent = dynamic_cast<CallExpr*>(a->parentExpr);
      if (!(parent && parent->baseExpr == a))
        if (dynamic_cast<UnresolvedSymbol*>(a->var))
          USR_FATAL_CONT(a, "Symbol '%s' is not defined", a->var->name);
    } else if (TypeSymbol* a = dynamic_cast<TypeSymbol*>(ast)) {
      if (EnumType* et = dynamic_cast<EnumType*>(a->type))
        check_normalized_enum(et);
    }
  }
}


static void
check_resolved_calls(CallExpr* call) {
}


static void
check_resolved_vars(VarSymbol* var) {
  if (var->immediate)
    return;
  if (!dynamic_cast<TypeSymbol*>(var->defPoint->parentSymbol))
    if (var->isParam())
      USR_FATAL(var,
                "Initializing parameter '%s' to value not known at compile time",
                var->name);
}


void
check_resolved(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast))
      check_resolved_calls(a);
    else if (VarSymbol* a = dynamic_cast<VarSymbol*>(ast))
      check_resolved_vars(a);
  }
}
