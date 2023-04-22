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

#ifndef _PASSES_H_
#define _PASSES_H_

#include "FnSymbol.h"
#include "symbol.h"
#include "PassManager.h"
#include "pass-manager-passes.h"
#include <unordered_set>

extern bool parsed;
extern bool normalized;
extern bool resolved;
extern bool intentsResolved;
extern bool iteratorsLowered;


//
// prototypes of functions that are called as passes (alphabetical)
//
void addInitCalls();
void buildDefaultFunctions();
void bulkCopyRecords();
void callDestructors();
void checkNormalized();
void checkResolved();
void checkUast();
void cleanup();
void codegen();
void copyPropagation();
void createTaskFunctions();
void cullOverReferences();
void deadCodeElimination();
void denormalize();
void docs();
void expandExternArrayCalls();
void flattenClasses();
void flattenFunctions();
void inlineFunctions();
void insertLineNumbers();
void insertWideReferences();
void localizeGlobals();
void loopInvariantCodeMotion();
void lowerErrorHandling();
void lowerIterators();
void makeBinary();
void normalize();
void optimizeOnClauses();
void parallel();
void prune();
void prune2();
void readExternC();
void refPropagation();
void removeEmptyRecords();
void removeUnnecessaryAutoCopyCalls();
void replaceArrayAccessesWithRefTemps();
void resolve();
void resolveIntents();
void returnStarTuplesByRefArgs();
void scalarReplace();
void scopeResolve();
void verify();

//
// prototypes for functions called as post-pass checks.
//
void checkInvariants(char log_tag);
void checkPrimitives();                 // constrains primitive use
void checkPostResolution();
// These checks can be applied after any pass.
void checkForDuplicateUses();
void checkArgsAndLocals();
void checkReturnTypesHaveRefTypes();

//
// utility functions in pass-containing code files
//

// buildDefaultFunctions.cpp
void buildDefaultDestructor(AggregateType* ct);
void buildEnumFunctions(EnumType* et);
FnSymbol* build_accessor(AggregateType* ct, Symbol* field,
                         bool setter, bool typeMethod);

// createTaskFunctions.cpp
void initForTaskIntents();
void removeTiMarks();
bool isTiMark(Symbol* sym);

// deadCodeElimination.cpp
void deadBlockElimination();

// flattenFunctions.cpp
void flattenNestedFunction(FnSymbol* nestedFunction);
void flattenNestedFunctions(Vec<FnSymbol*>& nestedFunctions);

// foralls.cpp
void checkTypeParamTaskIntent(SymExpr* outerSE);

// inlineFunctions.cpp
BlockStmt* copyFnBodyForInlining(CallExpr* call, FnSymbol* fn, Expr* anchor);

// interfaces.cpp, interfaceResolution.cpp
void  introduceConstrainedTypes(FnSymbol* fn);
void  insertResolutionPoint(Expr* ref, Symbol* sym);
TypeSymbol* desugarInterfaceAsType(FnSymbol* parentFn, ArgSymbol* arg,
                                   Expr* ref, InterfaceSymbol* isym);
Type* desugarInterfaceAsType(ArgSymbol* arg, SymExpr* se,
                             InterfaceSymbol* isym);
void  markImplStmtWrapFnAsFailure(FnSymbol* wrapFn);
void  wrapImplementsStatements();
FnSymbol* wrapOneImplementsStatement(ImplementsStmt* istm);
void  handleCallsToOtherCGfuns(FnSymbol* origFn, InterfaceInfo* ifcInfo,
                               SymbolMap& copyMap, FnSymbol* newFn);

// iterator.cpp
CallExpr* setIteratorRecordShape(Expr* ref, Symbol* ir, Symbol* shapeSpec,
                                 bool fromForExpr);
void setIteratorRecordShape(CallExpr* call);
bool checkIteratorFromForExpr(Expr* ref, Symbol* shape);

// LoopExpr.cpp
bool isOuterVarLoop(Symbol* sym, Expr* enclosingExpr);

// lowerIterators.cpp, lowerForalls.cpp
void lowerForallStmtsInline();
void handleChplPropagateErrorCall(CallExpr* call, bool allowForall = false);
void fixupErrorHandlingExits(BlockStmt* body, bool& adjustCaller);
void addDummyErrorArgumentToCall(CallExpr* call);
bool isVirtualIterator(FnSymbol* iterFn);

// normalize.cpp
void normalize(FnSymbol* fn);
void normalize(Expr* expr);
void checkUseBeforeDefs(FnSymbol* fn);
void addMentionToEndOfStatement(Expr* node, CallExpr* existingEndOfStatement);
Expr* partOfNonNormalizableExpr(Expr* expr);

// parallel.cpp
Type* getOrMakeRefTypeDuringCodegen(Type* type);
Type* getOrMakeWideTypeDuringCodegen(Type* refType);
CallExpr* findDownEndCount(FnSymbol* fn);

// resolution
Expr*     resolveExpr(Expr* expr);
void      resolveBlockStmt(BlockStmt* blockStmt);

#endif
