#include "analysis.h"
#include "expr.h"
#include "findUnknownTypes.h"
#include "stmt.h"


class FindReturn : public Traversal {
 public:
  bool found;
  FindReturn(void);
  void preProcessStmt(Stmt* stmt);
};

FindReturn::FindReturn() {
  found = false;
}

void FindReturn::preProcessStmt(Stmt* stmt) {
  if (dynamic_cast<ReturnStmt*>(stmt)) {
    found = true;
  }
}


void RemoveTypeVariableActuals::preProcessExpr(Expr* expr) {
  if (FnCall* call = dynamic_cast<FnCall*>(expr)) {
    Expr* arg = call->argList;
    while (arg) {
      Expr* next_arg = nextLink(Expr, arg);
      if (Variable* var_arg = dynamic_cast<Variable*>(arg)) {
	if (dynamic_cast<TypeSymbol*>(var_arg->var)) {
	  arg->extract();
	}
      }
      arg = next_arg;
    }
  }
}

void RemoveTypeVariableFormals::preProcessSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    Symbol* old_formals = fn->formals;
    Symbol* new_formals = NULL;
    while (old_formals) {
      Symbol* next_old_formals = nextLink(Symbol, old_formals);
      old_formals->next = NULL;
      old_formals->prev = NULL;
      if (dynamic_cast<ParamSymbol*>(old_formals)) {
	new_formals = appendLink(new_formals, old_formals);
      }
      old_formals = next_old_formals;
    }
    fn->formals = new_formals;
  }
}


FindUnknownTypes::FindUnknownTypes(void) {
  processInternalModules = false;
}

void FindUnknownTypes::preProcessSymbol(Symbol* sym) {
  if (sym->type == dtUnknown) {
    sym->type = type_info(sym);
  }
  FnSymbol* fn = dynamic_cast<FnSymbol*>(sym);
  if (fn) {
    if (fn->retType == dtUnknown) {
      if (analyzeAST) {
	fn->retType = return_type_info(fn);
      }
      else {
	FindReturn* traversal = new FindReturn();
	TRAVERSE_LS(fn->body, traversal, true);
	if (traversal->found) {
	  INT_FATAL(sym, "Analysis required to determine return type");
	}
	else {
	  fn->retType = dtVoid;
	}
      }
    }
  }
}
