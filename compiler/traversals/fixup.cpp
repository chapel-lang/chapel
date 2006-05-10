#include <string.h>
#include <typeinfo>
#include "fixup.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"


Fixup::Fixup(bool initVerifyParents) :
  verifyParents(initVerifyParents)
{
  if (!verifyParents)
    INT_FATAL("Fixup not used to Verify");
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
  ModuleSymbol* mod1 = dynamic_cast<ModuleSymbol*>(sym1);
  ModuleSymbol* mod2 = dynamic_cast<ModuleSymbol*>(sym2);
  return ((sym1 == sym2) ||
          (mod1 && mod1->initFn == sym2) ||
          (mod2 && mod2->initFn == sym1));
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

  if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
    if (!verifyParents) {
      if (blockStmt->blkScope &&
          blockStmt->blkScope->type > SCOPE_MODULE &&
          blockStmt->blkScope->type != SCOPE_CLASS) {
        INT_FATAL(blockStmt, "Unexpected scope in BlockStmt");
      }
      if (!blockStmt->blkScope) {
        Symboltable::pushScope(SCOPE_LOCAL);
      }
    }
  }

  parentStmts.add(stmt);
}


void Fixup::postProcessStmt(Stmt* stmt) {
  if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
    if (!verifyParents) {
      if (!blockStmt->blkScope) {
        blockStmt->blkScope = Symboltable::popScope();
        blockStmt->blkScope->astParent = blockStmt;
      }
    }
  }

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
    if (!verifyParents) {
      if (def_expr->sym && !def_expr->sym->isUnresolved) {
        def_expr->parentScope->define(def_expr->sym);
      }
    }
    defSymbols.add(def_expr->sym);

    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (!verifyParents) {
        if (fn->argScope) {
          INT_FATAL(fn, "Unexpected scope in FnSymbol");
        }
        Symboltable::pushScope(SCOPE_ARG);
      }
    }
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
      if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
        if (!verifyParents) {
          if (type->structScope) {
            INT_FATAL(typeSym, "Unexpected scope in FnSymbol");
          }
          Symboltable::pushScope(SCOPE_CLASS);
        }
      }
    }
  }

  parentExprs.add(expr);
}


void Fixup::postProcessExpr(Expr* expr) {
  if (dynamic_cast<DefExpr*>(expr)) {
    defSymbols.pop();
  }

  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
      if (!verifyParents) {
        fn->argScope = Symboltable::popScope();
        fn->argScope->astParent = fn;
      }
    }
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
        if (!verifyParents) {
          type->structScope = Symboltable::popScope();
          type->structScope->astParent = typeSym;
        }
      }
    }
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


void Fixup::run(Vec<ModuleSymbol*>* modules) {
  forv_Vec(ModuleSymbol, mod, *modules) {
    parentSymbols.add(mod);
    mod->startTraversal(this);
    parentSymbols.pop();
  }
}


void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Stmt* parentStmt,
                 Symbol* parentSymbol,
                 SymScope* parentScope) {
  if (Symbol* sym = dynamic_cast<Symbol*>(ast)) {
    parentSymbol = sym;
    parentExpr = NULL;
    parentStmt = NULL;
  }

  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    stmt->parentScope = parentScope;
    stmt->parentSymbol = parentSymbol;
    stmt->parentStmt = parentStmt;

    if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
      if (blockStmt->blkScope &&
          blockStmt->blkScope->type > SCOPE_MODULE &&
          blockStmt->blkScope->type != SCOPE_CLASS) {
        INT_FATAL(blockStmt, "Unexpected scope in BlockStmt");
      }
      if (!blockStmt->blkScope) {
        blockStmt->blkScope = Symboltable::pushScope(SCOPE_LOCAL, parentScope);
        blockStmt->blkScope->astParent = blockStmt;
      }
      parentScope = blockStmt->blkScope;
    }
    parentStmt = stmt;
  }

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    expr->parentScope = parentScope;
    expr->parentSymbol = parentSymbol;
    expr->parentStmt = parentStmt;
    expr->parentExpr = parentExpr;

    if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
      if (!dynamic_cast<ModuleSymbol*>(def_expr->sym)) {
        if (def_expr->sym && !def_expr->sym->isUnresolved) {
          def_expr->parentScope->define(def_expr->sym);
        }
      }
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
        if (fn->argScope) {
          INT_FATAL(fn, "Unexpected scope in FnSymbol");
        }
        fn->argScope = Symboltable::pushScope(SCOPE_ARG, parentScope);
        fn->argScope->astParent = fn;
        parentScope = fn->argScope;
      }
      if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
        if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
          if (type->structScope) {
            INT_FATAL(typeSym, "Unexpected scope in FnSymbol");
          }
          type->structScope = Symboltable::pushScope(SCOPE_CLASS, parentScope);
          type->structScope->astParent = typeSym;
          parentScope = type->structScope;
        }
      }
    }
    parentExpr = expr;
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts, 0, 1);
  forv_Vec(BaseAST, ast, asts)
    insert_help(ast, parentExpr, parentStmt, parentSymbol, parentScope);
}


void remove_help(BaseAST* ast) {
  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    stmt->parentScope = NULL;
    stmt->parentSymbol = NULL;
    stmt->parentStmt = NULL;
  }
  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    expr->parentScope = NULL;
    expr->parentSymbol = NULL;
    expr->parentStmt = NULL;
    expr->parentExpr = NULL;
  }
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
    if (!dynamic_cast<ModuleSymbol*>(defExpr->sym)) {
      if (defExpr->sym->parentScope)
        defExpr->sym->parentScope->undefine(defExpr->sym);
      defExpr->sym->parentScope = NULL;
    }
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    remove_help(ast);

  if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
    if (block->blkScope && block->blkScope->type == SCOPE_LOCAL) {
      Symboltable::removeScope(block->blkScope);
      block->blkScope = NULL;
    }
  }
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
      if (fn->argScope)
        Symboltable::removeScope(fn->argScope);
      fn->argScope = NULL;
    }
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
        if (type->structScope)
          Symboltable::removeScope(type->structScope);
        type->structScope = NULL;
      }
    }
  }
}
