// oldCollectors.h
//
// It is desired to deprecate the collect...() functions below in favor of
// their STL-based counter parts that still appear in astutil.h
// 
// Please do not include this .h file nor use these functions in new code.
//

#ifdef _OLDCOLLECTORS_H
#error Multiple inclusion
#else
#define _OLDCOLLECTORS_H

void collectFnCalls(BaseAST* ast, Vec<CallExpr*>& calls);
void collect_asts(BaseAST* ast, Vec<BaseAST*>& asts);
void collect_asts_postorder(BaseAST*, Vec<BaseAST*>& asts);
void collect_top_asts(BaseAST* ast, Vec<BaseAST*>& asts);
void collect_stmts(BaseAST* ast, Vec<Expr*>& stmts);
void collectDefExprs(BaseAST* ast, Vec<DefExpr*>& defExprs);
void collectCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs);
void collectMyCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs, FnSymbol* fn);
void collectGotoStmts(BaseAST* ast, Vec<GotoStmt*>& gotoStmts);
void collectSymExprs(BaseAST* ast, Vec<SymExpr*>& symExprs);
void collectMySymExprs(Symbol* me, Vec<SymExpr*>& symExprs);
void collectSymbols(BaseAST* ast, Vec<Symbol*>& symbols);


#endif
