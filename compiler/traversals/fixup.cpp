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
  Symbol* parentSymbol = parentSymbols.v[parentSymbols.n-1];
  if (verify) {
    if (!EQparentSymbol(stmt->parentSymbol, parentSymbol)) {
      INT_FATAL(stmt, "Stmt's parentSymbol is incorrect");
    }
  } else {
    stmt->parentSymbol = parentSymbol;
  }

  Stmt* parentStmt = parentStmts.v[parentStmts.n-1];
  if (verify) {
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
  Symbol* parentSymbol = parentSymbols.v[parentSymbols.n-1];
  if (verify) {
    if (!EQparentSymbol(expr->parentSymbol, parentSymbol)) {
      INT_FATAL(expr, "Expr's parentSymbol is incorrect");
    }
  } else {
    expr->parentSymbol = parentSymbol;
  }

  Stmt* parentStmt = parentStmts.v[parentStmts.n-1];
  if (verify) {
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
  if (verify) {
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
    /*** Verify only one symbol per DefExpr ***/
    if (def_expr->sym && def_expr->sym->next) {
      INT_FATAL(def_expr, "Multiple symbols in DefExpr");
    }
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
  if (verify) {
    verifyParentScope(sym);
    verifyDefPoint(sym);
    if (dynamic_cast<TypeSymbol*>(sym)) {
      if (sym->type->symbol != sym) {
 	INT_FATAL(sym, "(TypeSymbol)sym->type->symbol != sym!");
      }
    }
  }
}


void Fixup::run(ModuleSymbol* moduleList) {
  verify = !strcmp(args, "verify");
  ModuleSymbol* mod = moduleList;
  while (mod) {
    parentSymbols.add(mod);
    mod->startTraversal(this);
    parentSymbols.pop();

    mod = nextLink(ModuleSymbol, mod);
  }
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


void fixup_symbol(Symbol* symbol) {
  Fixup* fixup = new Fixup();
  fixup->parentSymbols.add(symbol);
  TRAVERSE_DEF(symbol, fixup, true);
}


void fixup_stmt(Stmt* stmt) {
  Fixup* fixup = new Fixup();
  fixup->parentSymbols.add(stmt->parentSymbol);
  fixup->parentStmts.add(stmt->parentStmt);
  TRAVERSE(stmt, fixup, true);
}


void fixup_expr(Expr* expr) {
  Fixup* fixup = new Fixup();
  fixup->parentSymbols.add(expr->parentSymbol);
  fixup->parentStmts.add(expr->parentStmt);
  fixup->parentExprs.add(expr->parentExpr);
  TRAVERSE(expr, fixup, true);
}


void call_fixup(BaseAST* ast) {
  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    if (stmt->parentStmt) {
      fixup_stmt(stmt->parentStmt);
    } else if (stmt->parentSymbol) {
      fixup_symbol(stmt->parentSymbol);
    } else {
      INT_FATAL(ast, "Stmt has no parentStmt|Symbol in call_fixup");
    }
  } else if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    if (expr->parentExpr) {
      fixup_expr(expr->parentExpr);
    } else if (expr->parentStmt) {
      fixup_stmt(expr->parentStmt);
    } else if (expr->parentSymbol) {
      fixup_symbol(expr->parentSymbol);
    } else {
      INT_FATAL(ast, "Expr has no parentExpr|Stmt|Symbol in call_fixup");
    }
  } else {
    INT_FATAL(ast, "Stmt or Expr expected as argument to call_fixup");
  }
}
