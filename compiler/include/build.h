#ifndef _BUILD_H_
#define _BUILD_H_

#include "alist.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

Expr* buildDot(BaseAST* base, char* member);

Expr* buildLogicalAnd(BaseAST* left, BaseAST* right);
Expr* buildLogicalOr(BaseAST* left, BaseAST* right);

BlockStmt* build_chpl_stmt(AList* stmts);
BlockStmt* build_chpl_stmt(BaseAST* ast = NULL);
void build_tuple_var_decl(Expr* base, BlockStmt* decls, Expr* insertPoint);
DefExpr* buildLabelStmt(char* name);
ModuleSymbol* build_module(char* name, modType type, AList* stmts);
CallExpr* build_primitive_call(AList* exprs);

FnSymbol* build_if_expr(Expr* e, Expr* e1, Expr* e2 = NULL);
FnSymbol* build_let_expr(BlockStmt* decls, Expr* expr);
BlockStmt* build_while_do_block(Expr* cond, BlockStmt* body);
BlockStmt* build_do_while_block(Expr* cond, BlockStmt* body);
BlockStmt* build_serial_block(Expr* cond, BlockStmt* body);
BlockStmt* build_for_block(BlockTag tag,
                           BaseAST* indices,
                           Expr* iterator,
                           BlockStmt* body);
BlockStmt* build_for_expr(BaseAST* indices,
                          Expr* iterator,
                          Expr* expr,
                          Expr* cond = NULL);
BlockStmt* build_param_for_stmt(char* index, Expr* low, Expr* high, Expr* stride, BlockStmt* stmts);
BlockStmt* buildCompoundAssignment(char* op, Expr* lhs, Expr* rhs);
BlockStmt* buildLogicalAndAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildLogicalOrAssignment(Expr* lhs, Expr* rhs);
CondStmt* build_select(Expr* s, BlockStmt* whenstmts);
BlockStmt* build_type_select(AList* s, BlockStmt* whenstmts);
FnSymbol* build_reduce(Expr* red, Expr *data, bool scan=false);

void backPropagateInitsTypes(BlockStmt* stmts);
void setVarSymbolAttributes(BlockStmt* stmts,
                            varType vartag,
                            consType constag);

DefExpr* build_class(char* name, Type* type, AList* decls);
DefExpr*
build_arg(intentTag tag, char* ident, Expr* type, Expr* init, Expr* variable);
Expr* build_tuple_arg(FnSymbol* fn, BlockStmt* tupledefs, Expr* base);

BlockStmt* buildOnStmt(Expr* expr, Expr* stmt);

#endif
