#include "typeCheck.h"

BaseAST* getScope(BaseAST* ast) {
  if (Expr* expr = toExpr(ast)) {
    BlockStmt* block = toBlockStmt(expr->parentExpr);
    if (block && block->blockTag != BLOCK_SCOPELESS) {
      return block;
    } else if (expr->parentExpr) {
      return getScope(expr->parentExpr);
    } else if (FnSymbol* fn = toFnSymbol(expr->parentSymbol)) {
      return fn;
    } else if (TypeSymbol* ts = toTypeSymbol(expr->parentSymbol)) {
      if (isEnumType(ts->type) || isClassType(ts->type)) {
        return ts;
      }
    }
    if (expr->parentSymbol == rootModule)
      return NULL;
    else
      return getScope(expr->parentSymbol->defPoint);
  } else if (Symbol* sym = toSymbol(ast)) {
    if (sym == rootModule)
      return NULL;
    else
      return getScope(sym->defPoint);
  }
  INT_FATAL(ast, "getScope expects an Expr or a Symbol");
  return NULL;
}
