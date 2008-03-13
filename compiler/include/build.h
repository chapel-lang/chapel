#ifndef _BUILD_H_
#define _BUILD_H_

#include "alist.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

Expr* buildDotExpr(BaseAST* base, const char* member);
Expr* buildDotExpr(const char* base, const char* member);

Expr* buildLogicalAndExpr(BaseAST* left, BaseAST* right);
Expr* buildLogicalOrExpr(BaseAST* left, BaseAST* right);

BlockStmt* buildChapelStmt(AList* stmts);
BlockStmt* buildChapelStmt(BaseAST* ast = NULL);
BlockStmt* buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init);
BlockStmt* buildLabelStmt(const char* name, Expr* stmt);
BlockStmt* buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr = NULL);
ModuleSymbol* buildModule(const char* name, ModTag type, BlockStmt* block);
CallExpr* buildPrimitiveExpr(AList* exprs);

FnSymbol* buildIfExpr(Expr* e, Expr* e1, Expr* e2 = NULL);
CallExpr* buildLetExpr(BlockStmt* decls, Expr* expr);
BlockStmt* buildWhileDoLoopStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildDoWhileLoopStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildCoforallLoopStmt(BaseAST* indices,
                                 Expr* iterator,
                                 BlockStmt* body);
BlockStmt* buildForLoopStmt(BlockTag tag,
                            BaseAST* indices,
                            Expr* iterator,
                            BlockStmt* body);
BlockStmt* buildForLoopExpr(BaseAST* indices,
                            Expr* iterator,
                            Expr* expr,
                            Expr* cond = NULL);
BlockStmt* buildParamForLoopStmt(const char* index, Expr* range, BlockStmt* block);
BlockStmt* buildCompoundAssignment(const char* op, Expr* lhs, Expr* rhs);
BlockStmt* buildLogicalAndExprAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildLogicalOrExprAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildSelectStmt(Expr* s, BlockStmt* whenstmts);
BlockStmt* buildTypeSelectStmt(AList* s, BlockStmt* whenstmts);

CallExpr* buildReduceScanExpr(Expr* op, Expr* data, bool isScan=false);

void backPropagateInitsTypes(BlockStmt* stmts);
void setVarSymbolAttributes(BlockStmt* stmts,
                            bool isConfig, bool isParam, bool isConst);

DefExpr* buildClassDefExpr(const char* name, Type* type, BlockStmt* decls);
DefExpr* buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable);
Expr* buildTupleArg(FnSymbol* fn, BlockStmt* tupledefs, Expr* base);

BlockStmt* buildOnStmt(Expr* expr, Expr* stmt);
BlockStmt* buildBeginStmt(Expr* stmt, bool allocateOnHeap = true);
BlockStmt* buildSyncStmt(Expr* stmt);
BlockStmt* buildCobeginStmt(Expr* stmt);
BlockStmt* buildAtomicStmt(Expr* stmt);
void createInitFn(ModuleSymbol* mod);
CallExpr* buildPreDecIncWarning(Expr* expr, char sign);

#endif
