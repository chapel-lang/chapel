#include <string.h>
#include <typeinfo>
#include "fixup.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"


static void verifySymbolDefPoint(Symbol* sym);
static void verifySymbolScope(Symbol* sym);


class HyperCopyReplace : public Traversal {
 public:
  HyperCopyReplace(void);
  void postProcessExpr(Expr* expr);
  void postProcessStmt(Stmt* stmt);
};


Fixup::Fixup(void) {
  exprParent.add(NULL);
  stmtParent.add(NULL);
}


void Fixup::preProcessStmt(Stmt* stmt) {
  Symbol* parent;

  SymScope* tmp = Symboltable::getCurrentScope();
  while (tmp && tmp->symContext->isNull()) {
    tmp = tmp->parent;
  }
  //  Symbol* tmp = stmtParent.v[stmtParent.n-1];
  if (tmp == NULL) {
    INT_FATAL(stmt, "NULL in Fixup()");
  }
  parent = tmp->symContext;

  // SJD: HACK for FnDefStmts of secondary methods
  bool ignore = false;
  if (FnDefStmt* fn_def_stmt = dynamic_cast<FnDefStmt*>(stmt)) {
    if (!fn_def_stmt->fn->classBinding->isNull()) {
      ignore = true;
    }
  }

  if (!ignore) {
    if (!verify) {
      stmt->parentSymbol = parent;
    }
    else if (stmt->parentSymbol != parent) {
      INT_FATAL(stmt, "Error during verification fixup: statement's parent is incorrect");
    }
  }
  /*
  if (FnDefStmt* fstmt = dynamic_cast<FnDefStmt*>(stmt)) {
    stmtParent.add(fstmt->fn);
  }

  if (TypeDefStmt* tstmt = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (ClassType* ctype = dynamic_cast<ClassType*>(tstmt->type)) {
      stmtParent.add(ctype->name);
    }
  }
  */
  exprParent.add(stmt);

  if (!stmt->back || *stmt->back != stmt) {
    INT_FATAL(stmt, "stmt back incorrect");
  }
}


void Fixup::postProcessStmt(Stmt* stmt) {
  /*
  if (dynamic_cast<FnDefStmt*>(stmt)) {
    stmtParent.pop();
  }

  if (TypeDefStmt* tstmt = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (dynamic_cast<ClassType*>(tstmt->type)) {
      stmtParent.pop();
    }
  }
  */
  exprParent.pop();
}


void Fixup::preProcessExpr(Expr* expr) {
  Stmt* tmp = exprParent.v[exprParent.n-1];
  if (tmp == NULL) {
    INT_FATAL(expr, "NULL in Fixup()");
  }
  if (!verify) {
    expr->stmt = tmp;
  }
  else if (expr->stmt != tmp) {
    INT_FATAL(expr, "Error during verification fixup: expression's statement is incorrect");
  }

  if (!expr->back || *expr->back != expr) {
    INT_FATAL(expr, "expr back incorrect");
  }
}


void Fixup::preProcessSymbol(Symbol* sym) {
  if (verify) {
    verifySymbolScope(sym);
    verifySymbolDefPoint(sym);
  }
}


void Fixup::run(ModuleSymbol* moduleList) {
  verify = !strcmp(args, "verify");

  if (!strcmp(args, "hyper verify")) {
    verify = 1;
  }

  ModuleSymbol* mod = moduleList;
  while (mod) {
    stmtParent.add(mod);
    mod->startTraversal(this);
    if (mod != stmtParent.pop()) {
      INT_FATAL(mod, "Major error in Fixup traversal");
    }
    mod = nextLink(ModuleSymbol, mod);
  }

  if (!strcmp(args, "hyper verify")) {
    HyperCopyReplace* hyper_copy_replace = new HyperCopyReplace();
    hyper_copy_replace->run(moduleList);

    ModuleSymbol* mod = moduleList;
    while (mod) {
      stmtParent.add(mod);
      mod->startTraversal(this);
      if (mod != stmtParent.pop()) {
	INT_FATAL(mod, "Major error in Fixup traversal");
      }
      mod = nextLink(ModuleSymbol, mod);
    }
  }
}


static void verifySymbolScope(Symbol* sym) {
  if (typeid(*sym) == typeid(UnresolvedSymbol)) {
    return;
  }

  SymScope* parentScope = sym->parentScope;
  if (parentScope) {
    Symbol* match = Symboltable::lookupInScope(sym->name, parentScope);
    if (match != sym) {
      bool error = true;
      FnSymbol* fnMatch = dynamic_cast<FnSymbol*>(match);
      if (fnMatch) {
	while (fnMatch) {
	  if (fnMatch == sym) {
	    error = false;
	  }
	  fnMatch = fnMatch->overload;
	}
      }
      if (error) {
	INT_FATAL(sym, "Symbol '%s' and Scope don't refer to each other",
		  sym->name);
      }
    }
  }
}


static void verifySymbolDefPoint(Symbol* sym) {
  if (typeid(*sym) == typeid(UnresolvedSymbol)) {
    return;
  }

  BaseAST* defPoint = sym->defPoint;
  if (defPoint) {
    if (VarDefStmt* stmt = dynamic_cast<VarDefStmt*>(defPoint)) {
      Symbol* tmp = stmt->var;
      while (tmp) {
	if (tmp == sym) {
	  return;
	}
	tmp = nextLink(Symbol, tmp);
      }
      INT_FATAL(sym, "Incorrect VarDefStmt defPoint "
		"for symbol '%s'", sym->name);
    }
    else if (TypeDefStmt* stmt = dynamic_cast<TypeDefStmt*>(defPoint)) {
      if (stmt->type->symbol == sym) {
	return;
      }
      if (EnumType* enum_type = dynamic_cast<EnumType*>(stmt->type)) {
	EnumSymbol* tmp = enum_type->valList;
	while (tmp) {
	  if (tmp == sym) {
	    return;
	  }
	  tmp = nextLink(EnumSymbol, tmp);
	}
      }
      INT_FATAL(sym, "Incorrect TypeDefStmt defPoint "
		"for symbol '%s'", sym->name);
    }
    else if (FnDefStmt* stmt = dynamic_cast<FnDefStmt*>(defPoint)) {
      if (stmt->fn != sym) {
	Symbol* formals = stmt->fn->formals;
	while (formals) {
	  if (formals == sym) {
	    return;
	  }
	  formals = nextLink(Symbol, formals);
	}
	INT_FATAL(sym, "Incorrect FnDefStmt defPoint "
		  "for symbol '%s'", sym->name);
      }
    }
    else if (ModuleDefStmt* stmt = dynamic_cast<ModuleDefStmt*>(defPoint)) {
      if (stmt->module != sym) {
	INT_FATAL(sym, "Incorrect ModuleDefStmt defPoint "
		  "for symbol '%s'", sym->name);
      }
    }
    else if (ForLoopStmt* stmt = dynamic_cast<ForLoopStmt*>(defPoint)) {
      Symbol* tmp = stmt->index;
      while (tmp) {
	if (tmp == sym) {
	  return;
	}
	tmp = nextLink(Symbol, tmp);
      }
      INT_FATAL(sym, "Incorrect ForLoopStmt defPoint "
		"for symbol '%s'", sym->name);
    }
    else if (ForallExpr* expr = dynamic_cast<ForallExpr*>(defPoint)) {
      Symbol* tmp = expr->indices;
      while (tmp) {
	if (tmp == sym) {
	  return;
	}
	tmp = nextLink(Symbol, tmp);
      }
      INT_FATAL(sym, "Incorrect ForallExpr defPoint "
		"for symbol '%s'", sym->name);
    }
    else if (LetExpr* expr = dynamic_cast<LetExpr*>(defPoint)) {
      Symbol* tmp = expr->syms;
      while (tmp) {
	if (tmp == sym) {
	  return;
	}
	tmp = nextLink(Symbol, tmp);
      }
      INT_FATAL(sym, "Incorrect LetExpr defPoint "
		"for symbol '%s'", sym->name);
    }
    else if (defPoint->isNull()) {
      if (sym->parentScope->type != SCOPE_INTRINSIC) {
	INT_FATAL(sym, "Nil defPoint for symbol '%s'", sym->name);
      }
    }
    else {
      INT_FATAL(sym, "Incorrect defPoint for symbol '%s'", sym->name);
    }
  }
}


HyperCopyReplace::HyperCopyReplace(void) {
  processInternalModules = false;
}


void HyperCopyReplace::postProcessExpr(Expr* expr) {
  if (strcmp(expr->stmt->parentSymbol->name, "main")) {
    Expr* expr_copy = expr->copy();
    expr->replace(expr_copy);
  }
}


void HyperCopyReplace::postProcessStmt(Stmt* stmt) {
  if (strcmp(stmt->parentSymbol->name, "main")) {
    Stmt* stmt_copy = stmt->copy();
    stmt->replace(stmt_copy);
  }
}
