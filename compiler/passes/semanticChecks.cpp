#include "semanticChecks.h"
#include "astutil.h"
#include "expr.h"
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


void SemanticCheckI::run(Vec<ModuleSymbol*>* modules) {
  Vec<Symbol*> syms;
  collect_symbols(&syms);

  forv_Vec(Symbol, sym, syms) {
    check_redefinition(sym);
  }
}


static void
check_calls_ii(CallExpr* call) {
  if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
    if (dynamic_cast<ModuleSymbol*>(base->var)) {
      USR_FATAL(call, "Illegal call of module %s", base->var->name);
    }
  }
}


void SemanticCheckII::run(Vec<ModuleSymbol*>* modules) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);

  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      check_calls_ii(a);
    }
  }
}


static void
check_calls_iii(CallExpr* call) {
  if (OP_ISASSIGNOP(call->opTag)) {
    if (call->get(1)->isConst() || call->get(1)->isParam()) {
      USR_FATAL(call, "Assigning to a constant expression");
    }
  }
}


static void
check_vars_iii(VarSymbol* var) {
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


void SemanticCheckIII::run(Vec<ModuleSymbol*>* modules) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);

  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      check_calls_iii(a);
    } else if (VarSymbol* a = dynamic_cast<VarSymbol*>(ast)) {
      check_vars_iii(a);
    }
  }
}
