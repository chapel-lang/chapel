#ifndef _BUILD_H_
#define _BUILD_H_

#include "alist.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

BlockStmt* buildPragmaStmt(Vec<const char*>*, BlockStmt*);

CallExpr* buildOneTuple(Expr* elem);
CallExpr* buildTuple(CallExpr* call);

Expr* buildSquareCallExpr(Expr* base, CallExpr* args);

Expr* buildNamedActual(const char* name, Expr* expr);
Expr* buildNamedAliasActual(const char* name, Expr* expr);

Expr* buildFormalArrayType(Expr* iterator, Expr* eltType, Expr* index = NULL);

Expr* buildIntLiteral(const char* pch);
Expr* buildRealLiteral(const char* pch);
Expr* buildImagLiteral(const char* pch);
Expr* buildStringLiteral(const char* pch);

Expr* buildDotExpr(BaseAST* base, const char* member);
Expr* buildDotExpr(const char* base, const char* member);

Expr* buildLogicalAndExpr(BaseAST* left, BaseAST* right);
Expr* buildLogicalOrExpr(BaseAST* left, BaseAST* right);

BlockStmt* buildChapelStmt(BaseAST* ast = NULL);
BlockStmt* buildUseStmt(CallExpr* modules);
BlockStmt* buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init);
BlockStmt* buildLabelStmt(const char* name, Expr* stmt);
BlockStmt* buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr = NULL);
ModuleSymbol* buildModule(const char* name, BlockStmt* block, const char* filename, char *docs);
CallExpr* buildPrimitiveExpr(CallExpr* exprs);

FnSymbol* buildIfExpr(Expr* e, Expr* e1, Expr* e2 = NULL);
CallExpr* buildLetExpr(BlockStmt* decls, Expr* expr);
BlockStmt* buildWhileDoLoopStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildDoWhileLoopStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body);
BlockStmt* buildCoforallLoopStmt(Expr* indices,
                                 Expr* iterator,
                                 CallExpr* byref_vars,
                                 BlockStmt* body,
                                 bool zippered = false);
BlockStmt* buildGotoStmt(GotoTag tag, const char* name);
BlockStmt* buildPrimitiveStmt(PrimitiveTag tag, Expr* e1 = NULL, Expr* e2 = NULL);
BlockStmt* buildForLoopStmt(Expr* indices,
                            Expr* iterator,
                            BlockStmt* body,
                            bool coforall = false,
                            bool zippered = false);
BlockStmt* buildForallLoopStmt(Expr* indices,
                               Expr* iterator,
                               BlockStmt* body,
                               bool zippered = false);
CallExpr* buildForLoopExpr(Expr* indices,
                           Expr* iterator,
                           Expr* expr,
                           Expr* cond = NULL,
                           bool maybeArrayType = false,
                           bool zippered = false);
CallExpr* buildForallLoopExpr(Expr* indices,
                              Expr* iterator,
                              Expr* expr,
                              Expr* cond = NULL,
                              bool maybeArrayType = false,
                              bool zippered = false);
CallExpr* buildForallLoopExprFromArrayType(CallExpr* buildArrRTTypeCall,
                                           bool recursiveCall = false);
BlockStmt* buildParamForLoopStmt(const char* index, Expr* range, BlockStmt* block);
BlockStmt* buildAssignment(Expr* lhs, Expr* rhs, const char* op = NULL);
BlockStmt* buildLAndAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildLOrAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildSelectStmt(Expr* s, BlockStmt* whenstmts);
BlockStmt* buildTypeSelectStmt(CallExpr* s, BlockStmt* whenstmts);

CallExpr* buildReduceExpr(Expr* op, Expr* data, bool zippered = false);
CallExpr* buildScanExpr(Expr* op, Expr* data, bool zippered = false);

BlockStmt* buildVarDecls(BlockStmt* stmts, Flag externconfig, Flag varconst, char* docs);

DefExpr* buildClassDefExpr(const char* name, Type* type, Expr* inherit, BlockStmt* decls, Flag isExtern, char *docs);
DefExpr* buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable);
DefExpr* buildTupleArgDefExpr(IntentTag tag, BlockStmt* tuple, Expr* type, Expr* init);
FnSymbol* buildFunctionFormal(FnSymbol* fn, DefExpr* def);
FnSymbol* buildLambda(FnSymbol *fn);
BlockStmt* buildFunctionDecl(FnSymbol* fn, RetTag optRetTag, Expr* optRetType,
                             Expr* optWhere, BlockStmt* optFnBody, char *docs);
BlockStmt* buildLocalStmt(Expr* stmt);
BlockStmt* buildOnStmt(Expr* expr, Expr* stmt);
BlockStmt* buildBeginStmt(CallExpr* byref_vars, Expr* stmt);
BlockStmt* buildSyncStmt(Expr* stmt);
BlockStmt* buildCobeginStmt(CallExpr* byref_vars, BlockStmt* block);
BlockStmt* buildAtomicStmt(Expr* stmt);
void createInitFn(ModuleSymbol* mod);
BlockStmt* buildExternBlockStmt(const char* c_code);
CallExpr* buildPreDecIncWarning(Expr* expr, char sign);
BlockStmt* convertTypesToExtern(BlockStmt*);
BlockStmt* handleConfigTypes(BlockStmt*);

#endif
