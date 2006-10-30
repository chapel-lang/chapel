#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"


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


static void
verify_ast(BaseAST* ast,
               Expr* parentExpr,
               Symbol* parentSymbol,
               SymScope* parentScope) {
  ast->verify();

  if (Symbol* sym = dynamic_cast<Symbol*>(ast)) {
    parentSymbol = sym;
    parentExpr = NULL;
  }

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    if (expr->parentScope != parentScope)
      INT_FATAL(expr, "Expr's parentScope is incorrect");
    if (!expr->parentSymbol)
      INT_FATAL(expr, "Expr has no parentSymbol");
    if (!EQparentSymbol(expr->parentSymbol, parentSymbol))
      INT_FATAL(expr, "Expr's parentSymbol is incorrect");
    if (expr->parentExpr != parentExpr)
      INT_FATAL(expr, "Expr's parentExpr is incorrect");
    if (expr->parentExpr &&
        !EQparentSymbol(expr->parentExpr->parentSymbol, expr->parentSymbol))
      INT_FATAL(expr, "Expr's parentExpr's parentSymbol is incorrect");
    if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(expr))
      if (blockStmt->blockTag != BLOCK_SCOPELESS)
        parentScope = blockStmt->blkScope;
    if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym))
        parentScope = fn->argScope;
      if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(def_expr->sym))
        if (ClassType* type = dynamic_cast<ClassType*>(typeSym->type))
          parentScope = type->structScope;
    }
    parentExpr = expr;
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    verify_ast(ast, parentExpr, parentSymbol, parentScope);
}


void
verify() {
  forv_Vec(ModuleSymbol, mod, allModules)
    verify_ast(mod, NULL, mod, mod->modScope);
}
