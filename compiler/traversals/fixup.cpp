#include <string.h>
#include <typeinfo>
#include "fixup.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"


static void verifyDefPoint(Symbol* sym);
static void verifyParentScope(Symbol* sym);


Fixup::Fixup(void) :
  verify(false)
{
  exprParents.add(NULL);
  exprStmts.add(NULL);
  stmtParents.add(NULL);
}


void Fixup::preProcessStmt(Stmt* stmt) {
  Symbol* parent = Symboltable::getCurrentScope()->findEnclosingSymContext();

  if (!parent) {
    INT_FATAL(stmt, "Null Stmt ParentSymbol in Fixup");
  }

  // SJD: HACK for definitions of secondary methods
  DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt);
  FnSymbol* fn = (def_stmt) ? def_stmt->fnDef() : NULL;
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
      } else if (ModuleSymbol* m = dynamic_cast<ModuleSymbol*>(parent)) {
        if (m->initFn == stmt->parentSymbol) {
          ignore = true;
        }
      }
      if (!ignore) {
        INT_FATAL(stmt, "Statement's parentSymbol is incorrect");
      }
    }
  }

  exprStmts.add(stmt);

  Stmt* stmtParent = stmtParents.v[stmtParents.n-1];
  if (verify) {
    if (stmt->parentStmt != stmtParent) {
      INT_FATAL(stmt, "Stmt's parentStmt is incorrect");
    }
    if (stmt->parentStmt && stmt->parentStmt->parentSymbol != stmt->parentSymbol) {
      //INT_FATAL(stmt, "Stmt's parentStmt's parentSymbol is incorrect");
    }
  } else {
    stmt->parentStmt = stmtParent;
  }

  if (!dynamic_cast<DefStmt*>(stmt)) {
    stmtParents.add(stmt);
  }

  if (!stmt->back || *stmt->back != stmt) {
    INT_FATAL(stmt, "stmt back incorrect");
  }
}


void Fixup::postProcessStmt(Stmt* stmt) {
  exprStmts.pop();
  if (!dynamic_cast<DefStmt*>(stmt)) {
    stmtParents.pop();
  }
}


void Fixup::preProcessExpr(Expr* expr) {
  Stmt* exprStmt = exprStmts.v[exprStmts.n-1];
  if (exprStmt == NULL) {
    INT_FATAL(expr, "Fixup cannot determine Expr's stmt");
  }
  if (verify) {
    if (expr->parentStmt != exprStmt) {
      INT_FATAL(expr, "Expr's parentStmt is incorrect");
    }
  } else {
    expr->parentStmt = exprStmt;
  }

  Expr* exprParent = exprParents.v[exprParents.n-1];
  if (verify) {
    if (expr->parentExpr != exprParent) {
      INT_FATAL(expr, "Expr's parentExpr is incorrect");
    }
    if (expr->parentExpr && expr->parentExpr->parentStmt != expr->parentStmt) {
      //INT_FATAL(expr, "Expr's parentExpr's parentStmt is incorrect");
    }
  } else {
    expr->parentExpr = exprParent;
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


void Fixup::postProcessSymbol(Symbol* sym) {
  if (verify) {
    verifyParentScope(sym);
    verifyDefPoint(sym);
  }
}


void Fixup::run(ModuleSymbol* moduleList) {
  verify = !strcmp(args, "verify");
  Traversal::run(moduleList);
}


/**
 **  Verify that Symbol::parentScope back pointer is correct
 **/
static void verifyParentScope(Symbol* sym) {
  /**
   **  Unresolved symbols have no scope
   **/
  if (dynamic_cast<UnresolvedSymbol*>(sym)) {
    return;
  }

  if (!sym->parentScope) {
    return;
  }

  Symbol* match = Symboltable::lookupInScope(sym->name, sym->parentScope);

  /**
   **  Symbol is match found in scope
   **/
  if (sym == match) {
    return;
  }

  /**
   **  Symbol is function overloaded to match found in scope
   **/
  FnSymbol* fn_match = dynamic_cast<FnSymbol*>(match);
  while (fn_match) {
    if (sym == fn_match) {
      return;
    }
    fn_match = fn_match->overload;
  }

  /**
   **  Symbol is getter of match found in scope
   **/
  FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(sym);
  if (fn_sym && fn_sym->_getter == match) {
    return;
  }

  INT_FATAL(sym, "Incorrect parentScope for symbol '%s'", sym->name);
}


/**
 **  Verify that Symbol::defPoint back pointer is correct
 **/
static void verifyDefPoint(Symbol* sym) {
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
