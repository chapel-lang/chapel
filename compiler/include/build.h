/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include <vector>

#include "flags.h"
#include "stmt.h"
#include "vec.h"

class BaseAST;
class BlockStmt;
class CallExpr;
class DefExpr;
class Expr;
class FnSymbol;
class ForwardingStmt;
class ImportStmt;
class ModuleSymbol;
class Type;

struct PotentialRename {
  enum { SINGLE, DOUBLE } tag;
  union {
    Expr* elem;
    std::pair<Expr*, Expr*>* renamed;
  };
};

Expr* lookupConfigVal(VarSymbol* var);

BlockStmt* buildPragmaStmt(Vec<const char*>*, BlockStmt*);
DefExpr* buildPragmaDefExpr(Vec<const char*>*, DefExpr*);

CallExpr* buildOneTuple(Expr* elem);
CallExpr* buildTuple(CallExpr* call);

Expr* buildSquareCallExpr(Expr* base, CallExpr* args);

Expr* buildNamedActual(const char* name, Expr* expr);

Expr* buildFormalArrayType(Expr* iterator, Expr* eltType, Expr* index = NULL);

SymExpr* buildIntLiteral(const char* pch, const char* file = NULL, int line = -1);
SymExpr* buildRealLiteral(const char* pch);
SymExpr* buildImagLiteral(const char* pch);
SymExpr* buildStringLiteral(const char* pch);
SymExpr* buildBytesLiteral(const char* pch);
SymExpr* buildCStringLiteral(const char* pch);

Expr* buildDotExpr(BaseAST* base, const char* member);
Expr* buildDotExpr(const char* base, const char* member);

BlockStmt* buildChapelStmt(Expr* expr = NULL);
BlockStmt* buildErrorStandin();

DefExpr* buildDeprecated(DefExpr* def);
DefExpr* buildDeprecated(DefExpr* def, const char* msg);
BlockStmt* buildDeprecated(BlockStmt* block);
BlockStmt* buildDeprecated(BlockStmt* block, const char* msg);

DefExpr* buildUnstable(DefExpr* def);
DefExpr* buildUnstable(DefExpr* def, const char* msg);
BlockStmt* buildUnstable(BlockStmt* block);
BlockStmt* buildUnstable(BlockStmt* block, const char* msg);

BlockStmt* buildUseStmt(std::vector<PotentialRename*>* args, bool privateUse);
BlockStmt* buildUseStmt(Expr* mod, const char* rename,
                        std::vector<PotentialRename*>* names, bool except,
                        bool privateUse);
BlockStmt* buildUseStmt(Expr* mod, Expr* rename,
                        std::vector<PotentialRename*>* names, bool except,
                        bool privateUse);
ImportStmt* buildImportStmt(Expr* mod);
ImportStmt* buildImportStmt(Expr* mod, const char* rename);
ImportStmt* buildImportStmt(Expr* mod, std::vector<PotentialRename*>* names);
void setImportPrivacy(BlockStmt* list, bool isPrivate);
bool processStringInRequireStmt(const char* str, bool parseTime,
                                const char* modFilename);
BlockStmt* buildRequireStmt(CallExpr* args);
DefExpr* buildQueriedExpr(const char *expr);
BlockStmt* buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init);
BlockStmt* buildLabelStmt(const char* name, Expr* stmt);
BlockStmt* buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr = NULL);
CallExpr*  buildIfVar(const char* name, Expr* rhs, bool isConst);

ModuleSymbol* buildModule(const char* name,
                          ModTag      modTag,
                          BlockStmt*  block,
                          const char* filename,
                          bool        priv,
                          bool        prototype);

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
BlockStmt* buildDeleteStmt(CallExpr* exprlist);
Expr* buildForLoopExpr(Expr* indices,
                       Expr* iterator,
                       Expr* expr,
                       Expr* cond = NULL,
                       bool maybeArrayType = false,
                       bool zippered = false);
Expr* buildForallLoopExpr(Expr* indices,
                          Expr* iterator,
                          Expr* expr,
                          Expr* cond = NULL,
                          bool maybeArrayType = false,
                          bool zippered = false);
Expr* buildForallLoopExprFromArrayType(CallExpr* buildArrRTTypeCall,
                                           bool recursiveCall = false);
BlockStmt* buildParamForLoopStmt(VarSymbol* indexVar, Expr* range, BlockStmt* block);
BlockStmt* buildAssignment(Expr* lhs, Expr* rhs, const char* op);
BlockStmt* buildAssignment(Expr* lhs, Expr* rhs, PrimitiveTag op);
BlockStmt* buildLAndAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildLOrAssignment(Expr* lhs, Expr* rhs);
BlockStmt* buildSelectStmt(Expr* s, BlockStmt* whenstmts);

CallExpr* buildReduceExpr(Expr* op, Expr* data, bool zippered = false);
CallExpr* buildScanExpr(Expr* op, Expr* data, bool zippered = false);


std::set<Flag>* buildVarDeclFlags(Flag flag1 = FLAG_UNKNOWN,
                                  Flag flag2 = FLAG_UNKNOWN);
BlockStmt* buildVarDecls(BlockStmt* stmts,
                         std::set<Flag>* flags = NULL, Expr* cnameExpr = NULL);

DefExpr*  buildClassDefExpr(const char*   name,
                            const char*   cname,
                            AggregateTag  tag,
                            Expr*         inherit,
                            BlockStmt*    decls,
                            Flag          isExtern);

void setupTypeIntentArg(ArgSymbol* arg);

DefExpr*  buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable);
DefExpr*  buildTupleArgDefExpr(IntentTag tag, BlockStmt* tuple, Expr* type, Expr* init);
FnSymbol* buildFunctionFormal(FnSymbol* fn, DefExpr* def);

void setupExternExportFunctionDecl(Flag externOrExport, Expr* paramCNameExpr,
                                   FnSymbol* fn);

BlockStmt* buildExternExportFunctionDecl(Flag externOrExport, Expr* paramCNameExpr, BlockStmt* blockFnDef);

void setupFunctionDecl(FnSymbol*   fn,
                       RetTag      optRetTag,
                       Expr*       optRetType,
                       bool        optThrowsError,
                       Expr*       optWhere,
                       Expr*       optLifetimeConstraints,
                       BlockStmt*  optFnBody);

BlockStmt* buildFunctionDecl(FnSymbol*   fn,
                             RetTag      optRetTag,
                             Expr*       optRetType,
                             bool        optThrowsError,
                             Expr*       optWhere,
                             Expr*       optLifetimeConstraints,
                             BlockStmt*  optFnBody);
void applyPrivateToBlock(BlockStmt* block);
ForwardingStmt* buildForwardingStmt(DefExpr* fnDef);
ForwardingStmt* buildForwardingStmt(DefExpr* fnDef,
                                    std::vector<PotentialRename*>* names,
                                    bool except);
BlockStmt* buildConditionalLocalStmt(Expr* condExpr, Expr* stmt);
BlockStmt* buildLocalStmt(Expr* stmt);
BlockStmt* buildManagerBlock(Expr* managerExpr, std::set<Flag>* flags,
                             const char* resourceName);
BlockStmt* buildManageStmt(BlockStmt* managers, BlockStmt* block);
BlockStmt* buildOnStmt(Expr* expr, Expr* stmt);
BlockStmt* buildBeginStmt(CallExpr* byref_vars, Expr* stmt);
BlockStmt* buildSyncStmt(Expr* stmt);
BlockStmt* buildCobeginStmt(CallExpr* byref_vars, BlockStmt* block);
BlockStmt* buildExternBlockStmt(const char* c_code);
CallExpr*  buildPreDecIncWarning(Expr* expr, char sign);
BlockStmt* convertTypesToExtern(BlockStmt*, const char* cname=nullptr);
BlockStmt* handleConfigTypes(BlockStmt*);

// In the following routines 'open[high|low]' are used to indicate
// whether an open-range is being created, like `lo..<hi`.  At
// present, Chapel only supports open intervals on the high bound,
// so those that say that the low bound is open are unused, but
// here if we decide to add `lo<..<hi` and/or `lo<..hi` later.
CallExpr* buildBoundedRange(Expr* low, Expr* high,
                            bool openlow=false, bool openhigh=false);
CallExpr* buildLowBoundedRange(Expr* low, bool open=false);
CallExpr* buildHighBoundedRange(Expr* high, bool open=false);
CallExpr* buildUnboundedRange();

Expr* tryExpr(Expr*);
Expr* tryBangExpr(Expr*);

// Intended to help issue better compile errors
// Converts a misuse of 'if a=b' into 'if a==b' and warns.
Expr* convertAssignmentAndWarn(Expr* a, const char* op, Expr* b);

// Emits an error for an attempt to redefine an internal type.
// The string name will be used in the error message.
void redefiningReservedTypeError(const char* name);
void redefiningReservedWordError(const char* name);

// Emits an error for an attempt to provide an unexpected this intent tag
// when defining an explicit operator.
void updateOpThisTagOrErr(FnSymbol* fn);

BlockStmt* foreachNotImplementedError();

#endif
