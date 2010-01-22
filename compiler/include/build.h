#ifndef _BUILD_H_
#define _BUILD_H_

#include "alist.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

BlockStmt* buildPragmaStmt(BlockStmt*, Vec<const char*>*, BlockStmt*);

Expr* buildParenExpr(CallExpr* call);

Expr* buildSquareCallExpr(Expr* base, CallExpr* args);

Expr* buildIntLiteral(const char* pch);
Expr* buildRealLiteral(const char* pch);
Expr* buildImagLiteral(const char* pch);
Expr* buildStringLiteral(const char* pch);

Expr* buildDotExpr(BaseAST* base, const char* member);
Expr* buildDotExpr(const char* base, const char* member);

Expr* buildLogicalAndExpr(BaseAST* left, BaseAST* right);
Expr* buildLogicalOrExpr(BaseAST* left, BaseAST* right);

BlockStmt* buildChapelStmt(BaseAST* ast = NULL);
BlockStmt* buildUseList(BaseAST* module, BlockStmt* list = NULL);
BlockStmt* buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init);
BlockStmt* buildLabelStmt(const char* name, Expr* stmt);
BlockStmt* buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr = NULL);
ModuleSymbol* buildModule(const char* name, BlockStmt* block, const char* filename);
CallExpr* buildPrimitiveExpr(CallExpr* exprs);

FnSymbol* buildIfExpr(Expr* e, Expr* e1, Expr* e2 = NULL);
CallExpr* buildLetExpr(BlockStmt* decls, Expr* expr);
BlockStmt* buildWhileDoLoopStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildDoWhileLoopStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildCoforallLoopStmt(Expr* indices,
                                 Expr* iterator,
                                 BlockStmt* body);
BlockStmt* buildForLoopStmt(Expr* indices,
                            Expr* iterator,
                            BlockStmt* body,
                            bool coforall = false);
BlockStmt* buildForallLoopStmt(Expr* indices,
                               Expr* iterator,
                               BlockStmt* body);
CallExpr* buildForLoopExpr(Expr* indices,
                           Expr* iterator,
                           Expr* expr,
                           Expr* cond = NULL);
CallExpr* buildForallLoopExpr(Expr* indices,
                              Expr* iterator,
                              Expr* expr,
                              Expr* cond = NULL,
                              bool maybeArrayType = false);
BlockStmt* buildParamForLoopStmt(const char* index, Expr* range, BlockStmt* block);
BlockStmt* buildCompoundAssignment(const char* op, Expr* lhs, Expr* rhs);
BlockStmt* buildLogicalAndExprAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildLogicalOrExprAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildSelectStmt(Expr* s, BlockStmt* whenstmts);
BlockStmt* buildTypeSelectStmt(CallExpr* s, BlockStmt* whenstmts);

CallExpr* buildReduceExpr(Expr* op, Expr* data);
CallExpr* buildScanExpr(Expr* op, Expr* data);

BlockStmt* buildVarDecls(BlockStmt* stmts, bool isConfig, bool isParam, bool isConst);

DefExpr* buildClassDefExpr(const char* name, Type* type, Expr* inherit, BlockStmt* decls, bool isExtern);
DefExpr* buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable);
Expr* buildTupleArg(FnSymbol* fn, BlockStmt* tupledefs, Expr* base);

BlockStmt* buildLocalStmt(Expr* stmt);
BlockStmt* buildOnStmt(Expr* expr, Expr* stmt);
BlockStmt* buildBeginStmt(Expr* stmt);
BlockStmt* buildSyncStmt(Expr* stmt);
BlockStmt* buildCobeginStmt(BlockStmt* block);
BlockStmt* buildAtomicStmt(Expr* stmt);
void createInitFn(ModuleSymbol* mod);
CallExpr* buildPreDecIncWarning(Expr* expr, char sign);

#endif
