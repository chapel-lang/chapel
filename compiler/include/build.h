#ifndef _BUILD_H_
#define _BUILD_H_

#include "alist.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

BlockStmt* build_chpl_stmt(BaseAST* ast = NULL);
ExprStmt* buildLabelStmt(char* name);
ModuleSymbol* build_module(char* name, modType type, AList<Stmt>* stmts);
CallExpr* build_primitive_call(AList<Expr>* exprs);

FnSymbol* build_if_expr(Expr* e, Expr* e1, Expr* e2 = NULL);
FnSymbol* build_let_expr(AList<Stmt>* decls, Expr* expr);
AList<Stmt>* build_while_do_block(Expr* cond, BlockStmt* body);
AList<Stmt>* build_do_while_block(Expr* cond, BlockStmt* body);
AList<Stmt>* build_for_block(BlockTag tag,
                             AList<DefExpr>* indices,
                             AList<Expr>* iterators,
                             BlockStmt* body, bool isSquare = false, int only_once = 0);
AList<Stmt>* build_for_expr(AList<DefExpr>* indices,
                            AList<Expr>* iterators,
                            Expr* expr,
                            bool isSquare = false,
                            Expr* cond = NULL);
AList<Stmt>* build_param_for(char* index, Expr* low, Expr* high, BlockStmt* stmts);
AList<Stmt>* build_assignplus(Expr* lhs, Expr* rhs);
CondStmt* build_select(Expr* s, AList<WhenStmt>* whenstmts);
AList<Stmt>* build_type_select(AList<Expr>* s, AList<WhenStmt>* whenstmts);
Expr* build_reduce_expr(Expr* red, Expr *seq);

void backPropagateInitsTypes(AList<Stmt>* stmts);
void setVarSymbolAttributes(AList<Stmt>* stmts,
                            varType vartag,
                            consType constag);

DefExpr* build_class(char* name, Type* type, AList<Stmt>* decls);
DefExpr*
build_arg(intentTag tag, char* ident, Expr* type, Expr* init, Expr* variable);

AList<DefExpr>* exprsToIndices(AList<Expr>* indices);
#endif
