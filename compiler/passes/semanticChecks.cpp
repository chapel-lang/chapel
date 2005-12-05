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
  bool returns_void = rets.v[0]->expr == NULL;
  forv_Vec(ReturnStmt, ret, rets) {
    if ((ret->expr && returns_void) || (!ret->expr && !returns_void))
      USR_FATAL(fn, "Not all function returns return a value");
  }
}


void
check_parsed(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (Symbol* sym = dynamic_cast<Symbol*>(ast))
      check_redefinition(sym);
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast))
      check_returns(fn);
  }
}


static void
check_normalized_calls(CallExpr* call) {
  if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
    if (dynamic_cast<ModuleSymbol*>(base->var)) {
      USR_FATAL(call, "Illegal call of module %s", base->var->name);
    }
  }
}


static void
check_normalized_functions(FnSymbol* fn) {
  if (fn->noParens && !fn->typeBinding)
    USR_FATAL(fn, "Non-member functions must have parenthesized argument lists");
}


void
check_normalized(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      check_normalized_calls(a);
    } else if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast)) {
      check_normalized_functions(a);
    }
  }
}


static void
check_resolved_calls(CallExpr* call) {
  if (call->opTag == OP_GETS) {
    if (call->get(1)->isConst() || call->get(1)->isParam()) {
      USR_FATAL(call, "Assigning to a constant expression");
    }
  }
}


static void
check_resolved_vars(VarSymbol* var) {
  if (var->isParam()) {
    if (var->defPoint->init && !get_constant(var->defPoint->init)) {
      USR_FATAL(var, "Initializing param to a variable expression.");
    }
    if (!var->defPoint->init &&
        dynamic_cast<FnSymbol*>(var->defPoint->parentSymbol) &&
        var->defPoint->parentScope->type != SCOPE_ARG) {
      USR_FATAL(var, "Top-level params must be initialized.");
    }
  }
}


void
check_resolved(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      check_resolved_calls(a);
    } else if (VarSymbol* a = dynamic_cast<VarSymbol*>(ast)) {
      check_resolved_vars(a);
    }
  }
}
