#include <string.h>
#include <typeinfo>
#include "fixup.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"


Fixup::Fixup(bool initVerifyParents, bool initInsertHelper) :
  verifyParents(initVerifyParents),
  insertHelper(initInsertHelper)
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
    stmt->parentScope = insertHelper ? parentScope : NULL;
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
    stmt->parentSymbol = insertHelper ? parentSymbol : NULL;
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
    stmt->parentStmt = insertHelper ? parentStmt : NULL;
  }

  if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
    if (!verifyParents) {
      if (insertHelper) {
        if (blockStmt->blkScope &&
            blockStmt->blkScope->type > SCOPE_MODULE) {
          INT_FATAL(blockStmt, "Unexpected scope in BlockStmt");
        }
        if (!blockStmt->blkScope) {
          Symboltable::pushScope(SCOPE_LOCAL);
        }
      }
    }
  }

  if (ForLoopStmt* forStmt = dynamic_cast<ForLoopStmt*>(stmt)) {
    if (!verifyParents) {
      if (insertHelper) {
        if (forStmt->indexScope) {
          INT_FATAL(forStmt, "Unexpected scope in ForStmt");
        }
        Symboltable::pushScope(SCOPE_FORLOOP);
      }
    }
  }

  parentStmts.add(stmt);
}


void Fixup::postProcessStmt(Stmt* stmt) {
  if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
    if (!verifyParents) {
      if (insertHelper) {
        if (!blockStmt->blkScope) {
          blockStmt->blkScope = Symboltable::popScope();
          blockStmt->blkScope->setContext(blockStmt);
        }
      } else {
        Symboltable::removeScope(blockStmt->blkScope);
        blockStmt->blkScope = NULL;
      }
    }
  }

  if (ForLoopStmt* forStmt = dynamic_cast<ForLoopStmt*>(stmt)) {
    if (!verifyParents) {
      if (insertHelper) {
        forStmt->indexScope = Symboltable::popScope();
        forStmt->indexScope->setContext(forStmt);
      } else {
        Symboltable::removeScope(forStmt->indexScope);
        forStmt->indexScope = NULL;
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
    expr->parentScope = insertHelper ? parentScope : NULL;
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
    expr->parentSymbol = insertHelper ? parentSymbol : NULL;
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
    expr->parentStmt = insertHelper ? parentStmt : NULL;
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
    expr->parentExpr = insertHelper ? parentExpr : NULL;
  }

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (!verifyParents) {
      if (def_expr->sym && !def_expr->sym->isUnresolved) {
        if (insertHelper) {
          def_expr->parentScope->insert(def_expr->sym);
        } else {
          def_expr->sym->parentScope->remove(def_expr->sym);
        }
      }
    }
    defSymbols.add(def_expr->sym);

    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (!verifyParents) {
        if (insertHelper) {
          if (fn->argScope) {
            INT_FATAL(fn, "Unexpected scope in FnSymbol");
          }
          Symboltable::pushScope(SCOPE_ARG);
        }
      }
    }
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
      if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
        if (!verifyParents) {
          if (insertHelper) {
            if (type->structScope) {
              INT_FATAL(typeSym, "Unexpected scope in FnSymbol");
            }
            Symboltable::pushScope(SCOPE_CLASS);
          }
        }
      }
    }
  }

  if (LetExpr* letExpr = dynamic_cast<LetExpr*>(expr)) {
    if (!verifyParents) {
      if (insertHelper) {
        if (letExpr->letScope) {
          INT_FATAL(letExpr, "Unexpected scope in Let Expr");
        }
        Symboltable::pushScope(SCOPE_LETEXPR);
      }
    }
  }

  if (ForallExpr* forallExpr = dynamic_cast<ForallExpr*>(expr)) {
    if (!verifyParents) {
      if (insertHelper) {
        if (forallExpr->indexScope) {
          INT_FATAL(forallExpr, "Unexpected scope in Forall Expr");
        }
        Symboltable::pushScope(SCOPE_FORALLEXPR);
      }
    }
  }

  parentExprs.add(expr);
}


void Fixup::postProcessExpr(Expr* expr) {
  if (dynamic_cast<DefExpr*>(expr)) {
    defSymbols.pop();
  }

  if (LetExpr* letExpr = dynamic_cast<LetExpr*>(expr)) {
    if (!verifyParents) {
      if (insertHelper) {
        letExpr->letScope = Symboltable::popScope();
        letExpr->letScope->setContext(NULL, NULL, letExpr);
      } else {
        Symboltable::removeScope(letExpr->letScope);
        letExpr->letScope = NULL;
      }
    }
  }

  if (ForallExpr* forallExpr = dynamic_cast<ForallExpr*>(expr)) {
    if (!verifyParents) {
      if (insertHelper) {
        forallExpr->indexScope = Symboltable::popScope();
        forallExpr->indexScope->setContext(NULL, NULL, forallExpr);
      } else {
        Symboltable::removeScope(forallExpr->indexScope);
        forallExpr->indexScope = NULL;
      }
    }
  }

  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
      if (!verifyParents) {
        if (insertHelper) {
          fn->argScope = Symboltable::popScope();
          fn->argScope->setContext(NULL, fn, NULL);
        } else {
          Symboltable::removeScope(fn->argScope);
          fn->argScope = NULL;
        }
      }
    }
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
        if (!verifyParents) {
          if (insertHelper) {
            type->structScope = Symboltable::popScope();
            type->structScope->setContext(NULL, typeSym, NULL);
          } else {
            Symboltable::removeScope(type->structScope);
            type->structScope = NULL;
          }
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
  if (!verifyParents)
    findInternalTypes();
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
    if (ast->parentScope == NULL) {
      ast->parentScope = Symboltable::getCurrentScope();
    }
    TRAVERSE(ast, fixup, true);
  }
}


void insertHelper(BaseAST* ast, ASTContext context) {
  if (context.parentSymbol) {
    Fixup* fixup = new Fixup();
    fixup->parentSymbols.add(context.parentSymbol);
    fixup->parentStmts.add(context.parentStmt);
    fixup->parentExprs.add(context.parentExpr);
    ast->parentScope = context.parentScope;
    if (ast->parentScope == NULL) {
      ast->parentScope = Symboltable::getCurrentScope();
    }
    TRAVERSE(ast, fixup, true);
  }  
}


void removeHelper(BaseAST* ast) {
  Symbol* parentSymbol = NULL;
  Stmt* parentStmt = NULL;
  Expr* parentExpr = NULL;
  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    parentStmt = stmt->parentStmt;
    parentSymbol = stmt->parentSymbol;
  } else if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    parentExpr = expr->parentExpr;
    parentStmt = expr->parentStmt;
    parentSymbol = expr->parentSymbol;
  }
  if (parentSymbol) {
    Fixup* fixup = new Fixup(false, false);
    fixup->parentSymbols.add(parentSymbol);
    fixup->parentStmts.add(parentStmt);
    fixup->parentExprs.add(parentExpr);
    if (!ast->parentScope) {
      INT_FATAL(ast, "Removing AST without scope");
    }
    TRAVERSE(ast, fixup, true);
  }
}
