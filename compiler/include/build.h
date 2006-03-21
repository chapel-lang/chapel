#ifndef _BUILD_H_
#define _BUILD_H_

#include "alist.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

FnSymbol* build_if_expr(Expr* e, Expr* e1, Expr* e2 = NULL);
FnSymbol* build_let_expr(AList<Stmt>* decls, Expr* expr);
AList<Stmt>* build_while_do_block(Expr* cond, BlockStmt* body);
AList<Stmt>* build_do_while_block(Expr* cond, BlockStmt* body);
AList<Stmt>* build_for_block(BlockTag tag,
                             AList<DefExpr>* indices,
                             AList<Expr>* iterators,
                             BlockStmt* body);
AList<Stmt>* build_param_for(char* index, Expr* low, Expr* high, AList<Stmt>* stmts);
AList<Stmt>* build_assignplus(Expr* lhs, Expr* rhs);
AList<Stmt>* build_type_select(AList<Expr>* s, AList<WhenStmt>* whenstmts);
Expr* build_reduce_expr(Expr* red, Expr *seq);

#endif
