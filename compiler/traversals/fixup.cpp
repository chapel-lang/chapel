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
  exprParents.add(NULL);
  exprStmts.add(NULL);
}


void Fixup::preProcessStmt(Stmt* stmt) {
  Symbol* parent = Symboltable::getCurrentScope()->findEnclosingSymContext();

  if (!parent) {
    INT_FATAL(stmt, "Null Stmt ParentSymbol in Fixup");
  }

  // SJD: HACK for definitions of secondary methods
  DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt);
  FnSymbol* fn;
  if (def_stmt) {
    fn = def_stmt->fnDef();
  }
  if (!def_stmt || !fn || !fn->classBinding) {
    if (!verify) {
      stmt->parentSymbol = parent;
    }
    else if (stmt->parentSymbol != parent) {
      // SJD hack because of __init function
      bool ignore = false;
      if (ModuleSymbol* m = dynamic_cast<ModuleSymbol*>(stmt->parentSymbol)) {
                                if (m->initFn == parent) {
                                ignore = true;
                                }
      }
      if (!ignore) {
                                INT_FATAL(stmt, "Statement's parent is incorrect");
      }
    }
  }

  exprStmts.add(stmt);

  if (!stmt->back || *stmt->back != stmt) {
    INT_FATAL(stmt, "stmt back incorrect");
  }
}


void Fixup::postProcessStmt(Stmt* stmt) {
  exprStmts.pop();
}


void Fixup::preProcessExpr(Expr* expr) {
  Stmt* exprStmt = exprStmts.v[exprStmts.n-1];
  if (exprStmt == NULL) {
    INT_FATAL(expr, "Fixup cannot determine Expr's stmt");
  }
  if (verify) {
    if (expr->stmt != exprStmt) {
      INT_FATAL(expr, "Expr's stmt is incorrect");
    }
  } else {
    expr->stmt = exprStmt;
  }

  Expr* exprParent = exprParents.v[exprParents.n-1];
  if (verify) {
    if (expr->parent != exprParent) {
      INT_FATAL(expr, "Expr's parent is incorrect");
    }
  } else {
    expr->parent = exprParent;
  }

  if (!dynamic_cast<DefExpr*>(expr)) {
    exprParents.add(expr);
  }

  if (!expr->back || *expr->back != expr) {
    INT_FATAL(expr, "expr back incorrect");
  }
}


void Fixup::postProcessExpr(Expr* expr) {
  if (!dynamic_cast<DefExpr*>(expr)) {
    exprParents.pop();
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
    mod->startTraversal(this);
    mod = nextLink(ModuleSymbol, mod);
  }

  if (!strcmp(args, "hyper verify")) {
    HyperCopyReplace* hyper_copy_replace = new HyperCopyReplace();
    hyper_copy_replace->run(moduleList);

    ModuleSymbol* mod = moduleList;
    while (mod) {
      mod->startTraversal(this);
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
  if (typeid(*sym) == typeid(LabelSymbol)) {
    return;
  }
  if (sym->parentScope && sym->parentScope->type == SCOPE_INTRINSIC) {
    return;
  }

  Symbol* tmp = sym->defPoint->sym;
  while (tmp) {
    if (tmp == sym) {
      return;
    }
    tmp = nextLink(Symbol, tmp);
  }
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym->defPoint->sym)) {
    Symbol* formals = fn->formals;
    while (formals) {
      if (formals == sym) {
        return;
      }
      formals = nextLink(Symbol, formals);
    }
  }
  if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(sym->defPoint->sym)) {
    if (EnumType* enum_type = dynamic_cast<EnumType*>(type_sym->type)) {
      EnumSymbol* tmp = enum_type->valList;
      while (tmp) {
        if (tmp == sym) {
          return;
        }
        tmp = nextLink(EnumSymbol, tmp);
      }
    }
  }
  INT_FATAL(sym, "Incorrect defPoint for symbol '%s'", sym->name);
}


HyperCopyReplace::HyperCopyReplace(void) {
  whichModules = MODULES_USER;
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
