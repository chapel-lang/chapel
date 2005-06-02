#include <string.h>
#include <typeinfo>
#include "fixup.h"
#include "expr.h"
#include "moduleList.h"
#include "stmt.h"
#include "symtab.h"


Fixup::Fixup(bool init_verifyParents) :
  verifyParents(init_verifyParents)
{
  parentExprs.add(NULL);
  parentStmts.add(NULL);
  parentSymbols.add(NULL);
  defSymbols.add(NULL);
}


/**
 **  Hack for __init function
 **  Call parentSymbols equal if they are or if
 **   the one is the init function and the other is the module
 **/
static bool EQparentSymbol(Symbol* sym1, Symbol* sym2) {
  if (sym1 == sym2) {
    return true;
  }
  ModuleSymbol* mod1 = dynamic_cast<ModuleSymbol*>(sym1);
  if (mod1 && mod1->initFn == sym2) {
    return true;
  }
  ModuleSymbol* mod2 = dynamic_cast<ModuleSymbol*>(sym2);
  if (mod2 && mod2->initFn == sym1) {
    return true;
  }
  return false;
}


void Fixup::preProcessStmt(Stmt* stmt) {
  SymScope* parentScope = Symboltable::getCurrentScope();
  if (verifyParents) {
    if (stmt->parentScope != parentScope) {
      INT_FATAL(stmt, "Stmt's parentScope is incorrect");
    }
  } else {
    stmt->parentScope = parentScope;
  }

  if (verifyParents && !stmt->parentSymbol) {
    INT_FATAL(stmt, "Stmt has no parentSymbol");
  }

  Symbol* parentSymbol = parentSymbols.v[parentSymbols.n-1];
  if (verifyParents) {
    if (!EQparentSymbol(stmt->parentSymbol, parentSymbol)) {
      INT_FATAL(stmt, "Stmt's parentSymbol is incorrect");
    }
  } else {
    stmt->parentSymbol = parentSymbol;
  }

  Stmt* parentStmt = parentStmts.v[parentStmts.n-1];
  if (verifyParents) {
    if (stmt->parentStmt != parentStmt) {
      INT_FATAL(stmt, "Stmt's parentStmt is incorrect");
    }
    if (stmt->parentStmt &&
        !EQparentSymbol(stmt->parentStmt->parentSymbol, stmt->parentSymbol)) {
      INT_FATAL(stmt, "Stmt's parentStmt's parentSymbol is incorrect");
    }
  } else {
    stmt->parentStmt = parentStmt;
  }

  parentStmts.add(stmt);
}


void Fixup::postProcessStmt(Stmt* stmt) {
  parentStmts.pop();
}


void Fixup::preProcessExpr(Expr* expr) {
  SymScope* parentScope = Symboltable::getCurrentScope();
  if (verifyParents) {
    if (expr->parentScope != parentScope) {
      INT_FATAL(expr, "Expr's parentScope is incorrect");
    }
  } else {
    expr->parentScope = parentScope;
  }

  if (verifyParents && !expr->parentSymbol) {
    INT_FATAL(expr, "Expr has no parentSymbol");
  }

  Symbol* parentSymbol = parentSymbols.v[parentSymbols.n-1];
  if (verifyParents) {
    if (!EQparentSymbol(expr->parentSymbol, parentSymbol)) {
      INT_FATAL(expr, "Expr's parentSymbol is incorrect");
    }
  } else {
    expr->parentSymbol = parentSymbol;
  }

  Stmt* parentStmt = parentStmts.v[parentStmts.n-1];
  if (verifyParents) {
    if (expr->parentStmt != parentStmt) {
      INT_FATAL(expr, "Expr's parentStmt is incorrect");
    }
    if (expr->parentStmt &&
        !EQparentSymbol(expr->parentStmt->parentSymbol, expr->parentSymbol)) {
      INT_FATAL(expr, "Expr's parentStmt's parentSymbol is incorrect");
    }
  } else {
    expr->parentStmt = parentStmt;
  }

  Expr* parentExpr = parentExprs.v[parentExprs.n-1];
  if (verifyParents) {
    if (expr->parentExpr != parentExpr) {
      INT_FATAL(expr, "Expr's parentExpr is incorrect");
    }
    if (expr->parentExpr && expr->parentExpr->parentStmt != expr->parentStmt) {
      INT_FATAL(expr, "Expr's parentExpr's parentStmt is incorrect");
    }
    if (expr->parentExpr &&
        !EQparentSymbol(expr->parentExpr->parentSymbol, expr->parentSymbol)) {
      INT_FATAL(expr, "Expr's parentExpr's parentSymbol is incorrect");
    }
  } else {
    expr->parentExpr = parentExpr;
  }

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    defSymbols.add(def_expr->sym);
  }

  parentExprs.add(expr);
}


void Fixup::postProcessExpr(Expr* expr) {
  if (dynamic_cast<DefExpr*>(expr)) {
    defSymbols.pop();
  }
  parentExprs.pop();
}


void Fixup::preProcessSymbol(Symbol* sym) {
  if (sym == defSymbols.v[defSymbols.n-1]) {
    parentExprs.add(NULL);
    parentStmts.add(NULL);
    parentSymbols.add(sym);
  }
}


void Fixup::postProcessSymbol(Symbol* sym) {
  if (sym == defSymbols.v[defSymbols.n-1]) {
    parentExprs.pop();
    parentStmts.pop();
    parentSymbols.pop();
  }
}


void Fixup::run(ModuleList* moduleList) {
  ModuleSymbol* mod = moduleList->first();
  while (mod) {
    parentSymbols.add(mod);
    mod->startTraversal(this);
    parentSymbols.pop();
    mod = moduleList->next();
  }
}


void fixup(BaseAST* ast) {
  fixup(ast, ast);
}


void fixup(BaseAST* ast, BaseAST* ref) {
  Symbol* parentSymbol = NULL;
  Stmt* parentStmt = NULL;
  Expr* parentExpr = NULL;
  if (Stmt* stmt = dynamic_cast<Stmt*>(ref)) {
    parentStmt = stmt->parentStmt;
    parentSymbol = stmt->parentSymbol;
  } else if (Expr* expr = dynamic_cast<Expr*>(ref)) {
    parentExpr = expr->parentExpr;
    parentStmt = expr->parentStmt;
    parentSymbol = expr->parentSymbol;
  }
  if (parentSymbol) {
    Fixup* fixup = new Fixup();
    fixup->parentSymbols.add(parentSymbol);
    fixup->parentStmts.add(parentStmt);
    fixup->parentExprs.add(parentExpr);
    ast->parentScope = ref->parentScope;
    TRAVERSE(ast, fixup, true);
  }
}
