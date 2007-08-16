#include "astutil.h"
#include "expr.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"


static void
verify_ast(BaseAST* ast,
           Expr* parentExpr,
           Symbol* parentSymbol,
           SymScope* parentScope) {
  ast->verify();

  if (Symbol* sym = toSymbol(ast)) {
    if (sym->parentScope != parentScope)
      INT_FATAL(sym, "Symbol's parentScope is incorrect");
    parentSymbol = sym;
    parentExpr = NULL;
    if (FnSymbol* fn = toFnSymbol(sym))
      parentScope = fn->argScope;
    if (TypeSymbol* type = toTypeSymbol(sym))
      if (ClassType* ct = toClassType(type->type))
        parentScope = ct->structScope;
  }

  if (Expr* expr = toExpr(ast)) {
    if (expr->parentScope != parentScope)
      INT_FATAL(expr, "Expr's parentScope is incorrect");
    if (!expr->parentSymbol)
      INT_FATAL(expr, "Expr has no parentSymbol");
    if (expr->parentSymbol != parentSymbol)
      INT_FATAL(expr, "Expr's parentSymbol is incorrect");
    if (expr->parentExpr != parentExpr)
      INT_FATAL(expr, "Expr's parentExpr is incorrect");
    if (parentExpr && parentExpr->parentSymbol != expr->parentSymbol)
      INT_FATAL(expr, "Expr's parentExpr's parentSymbol is incorrect");
    if (BlockStmt* blockStmt = toBlockStmt(expr))
      if (blockStmt->blockTag != BLOCK_SCOPELESS)
        parentScope = blockStmt->blkScope;
    parentExpr = expr;
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, child, asts)
    verify_ast(child, parentExpr, parentSymbol, parentScope);
}


void
verify() {
  verify_ast(theProgram, NULL, NULL, rootScope);
}
