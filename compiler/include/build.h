/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BUILD_H_
#define _BUILD_H_

#include <set>

#include "flags.h"
#include "stmt.h"
#include "vec.h"

class BaseAST;
class BlockStmt;
class CallExpr;
class DefExpr;
class Expr;
class FnSymbol;
class ModuleSymbol;
class Type;

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

BlockStmt* buildChapelStmt(Expr* expr = NULL);
BlockStmt* buildUseStmt(CallExpr* modules);
BlockStmt* buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init);
BlockStmt* buildLabelStmt(const char* name, Expr* stmt);
BlockStmt* buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr = NULL);
ModuleSymbol* buildModule(const char* name, BlockStmt* block, const char* filename, const char* docs);
CallExpr* buildPrimitiveExpr(CallExpr* exprs);

FnSymbol* buildIfExpr(Expr* e, Expr* e1, Expr* e2 = NULL);
CallExpr* buildLetExpr(BlockStmt* decls, Expr* expr);
BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body);
void       checkIndices(BaseAST* indices);
void       destructureIndices(BlockStmt* block,
                              BaseAST*   indices,
                              Expr*      init,
                              bool       coforall);
BlockStmt* buildCoforallLoopStmt(Expr* indices,
                                 Expr* iterator,
                                 CallExpr* byref_vars,
                                 BlockStmt* body,
                                 bool zippered = false);
BlockStmt* buildGotoStmt(GotoTag tag, const char* name);
BlockStmt* buildPrimitiveStmt(PrimitiveTag tag, Expr* e1 = NULL, Expr* e2 = NULL);
BlockStmt* buildForallLoopStmt(Expr* indices,
                               Expr* iterator,
                               CallExpr* byref_vars,
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

CallExpr* buildReduceExpr(Expr* op, Expr* data, bool zippered = false);
CallExpr* buildScanExpr(Expr* op, Expr* data, bool zippered = false);


BlockStmt* buildVarDecls(BlockStmt* stmts, std::set<Flag> flags, const char* docs);

DefExpr*  buildClassDefExpr(const char* name,
                            const char* cname,
                            Type*       type,
                            Expr*       inherit,
                            BlockStmt*  decls,
                            Flag        isExtern,
                            const char* docs);
void setupTypeIntentArg(ArgSymbol* arg);
DefExpr*  buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable);
DefExpr*  buildTupleArgDefExpr(IntentTag tag, BlockStmt* tuple, Expr* type, Expr* init);
FnSymbol* buildFunctionFormal(FnSymbol* fn, DefExpr* def);
FnSymbol* buildLambda(FnSymbol* fn);
FnSymbol* buildFunctionSymbol(FnSymbol*   fn,
                              const char* name,
                              IntentTag   thisTag,
                              const char* class_name);
BlockStmt* buildFunctionDecl(FnSymbol*   fn,
                             RetTag      optRetTag,
                             Expr*       optRetType,
                             Expr*       optWhere,
                             BlockStmt*  optFnBody,
                             const char* docs);
BlockStmt* buildLocalStmt(Expr* stmt);
BlockStmt* buildOnStmt(Expr* expr, Expr* stmt);
BlockStmt* buildBeginStmt(CallExpr* byref_vars, Expr* stmt);
BlockStmt* buildSyncStmt(Expr* stmt);
BlockStmt* buildCobeginStmt(CallExpr* byref_vars, BlockStmt* block);
BlockStmt* buildAtomicStmt(Expr* stmt);
BlockStmt* buildExternBlockStmt(const char* c_code);
CallExpr*  buildPreDecIncWarning(Expr* expr, char sign);
BlockStmt* convertTypesToExtern(BlockStmt*);
BlockStmt* handleConfigTypes(BlockStmt*);

#endif
