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
  if (var->isParam() && !var->immediate) {
    if (!var->defPoint->init &&
        (dynamic_cast<FnSymbol*>(var->defPoint->parentSymbol) ||
         dynamic_cast<ModuleSymbol*>(var->defPoint->parentSymbol)) &&
        var->defPoint->parentScope->type != SCOPE_ARG) {
      USR_FATAL(var, "Top-level params must be initialized.");
    }
  }
}


void
check_parsed(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
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
  CallExpr* move;
  forv_Vec(SymExpr*, sym, *var->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == sym) {
        num_moves++;
        move = call;
      }
    }
  }
  if (num_moves >= 2) {
    USR_FATAL_CONT(move, "Assigning to a constant expression");
  }
  if (num_moves >= 1 && dynamic_cast<ArgSymbol*>(var)) {
    USR_FATAL_CONT(move, "Assigning to a constant expression");
  }
}


static void
check_normalized_functions(FnSymbol* fn) {
  if (fn->noParens && !fn->typeBinding)
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
            // hack for for loop statements which are not very normal
            if (!dynamic_cast<ForLoopStmt*>(sym->var->defPoint->parentStmt))
              USR_FATAL(sym, "Variable '%s' used before defined", sym->var->name);
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
