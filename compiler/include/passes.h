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

#ifndef _PASSES_H_
#define _PASSES_H_

#include "symbol.h"

extern bool parsed;
extern bool normalized;
extern bool resolved;
extern bool intentsResolved;


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
void complex2record();
void copyPropagation();
void createTaskFunctions();
void cullOverReferences();
void deadCodeElimination();
void docs();
void expandExternArrayCalls();
void flattenClasses();
void flattenFunctions();
void inlineFunctions();
void insertAutoCopyAutoDestroy();
void insertLineNumbers();
void insertWideReferences();
void narrowWideReferences();
void localizeGlobals();
void loopInvariantCodeMotion();
void lowerIterators();
void makeBinary();
void normalize();
void optimizeOnClauses();
void parallel();
void parse();
void processIteratorYields();
void prune();
void prune2();
void readExternC();
void refPropagation();
void removeEmptyRecords();
void removeUnnecessaryAutoCopyCalls();
void removeWrapRecords();
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
void checkReturnTypesHaveRefTypes();

//
// utility functions in pass-containing code files
//

// buildDefaultFunctions.cpp
void buildDefaultDestructor(AggregateType* ct);

// createTaskFunctions.cpp -> implementForallIntents.cpp
extern Symbol* markPruned;
extern Symbol* markUnspecified;
void markOuterVarsWithIntents(CallExpr* byrefVars, SymbolMap& uses);
void replaceVarUses(Expr* topAst, SymbolMap& vars);
void pruneThisArg(Symbol* parent, SymbolMap& uses);

// deadCodeElimination.cpp
void deadBlockElimination();

// flattenFunctions.cpp
void flattenNestedFunctions(Vec<FnSymbol*>& nestedFunctions);

// callDestructors.cpp
void insertReferenceTemps(CallExpr* call);
void insertReferenceTemps();

void insertDerefTemps(CallExpr* call);
void insertDerefTemps(FnSymbol* fn);
void insertDerefTemps();

// parallel.cpp
Type* getOrMakeRefTypeDuringCodegen(Type* type);
Type* getOrMakeWideTypeDuringCodegen(Type* refType);

// type.cpp
void initForTaskIntents();

#endif
