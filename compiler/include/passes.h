/*
 * Copyright 2004-2018 Cray Inc.
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

#include "symbol.h"

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
void checkParsed();
void checkResolved();
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
void processIteratorYields();
void prune();
void prune2();
void readExternC();
void refPropagation();
void removeEmptyRecords();
void removeUnnecessaryAutoCopyCalls();
void removeWrapRecords();
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
void checkNoUnresolveds();
// These checks can be applied after any pass.
void checkForDuplicateUses();
void checkArgsAndLocals();
void checkReturnTypesHaveRefTypes();

//
// utility functions in pass-containing code files
//

// buildDefaultFunctions.cpp
void buildDefaultDestructor(AggregateType* ct);
void buildNearScopeEnumFunctions(EnumType* et);
void buildFarScopeEnumFunctions(EnumType* et);

// createTaskFunctions.cpp -> implementForallIntents.cpp
extern Symbol* markPruned;
extern Symbol* markUnspecified;
void replaceVarUses(Expr* topAst, SymbolMap& vars);
void pruneOuterVars(Symbol* parent, SymbolMap& uses);

// deadCodeElimination.cpp
void deadBlockElimination();

// flattenFunctions.cpp
void flattenNestedFunction(FnSymbol* nestedFunction);

// callDestructors.cpp
void insertReferenceTemps(CallExpr* call);

// normalize.cpp
void normalize(FnSymbol* fn);
void normalize(Expr* expr);

// parallel.cpp
Type* getOrMakeRefTypeDuringCodegen(Type* type);
Type* getOrMakeWideTypeDuringCodegen(Type* refType);

// type.cpp
void initForTaskIntents();

#endif
