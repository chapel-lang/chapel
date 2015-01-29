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
void pruneOuterVars(SymbolMap* uses, CallExpr* byrefVars);
void pruneThisArg(Symbol* parent, SymbolMap* uses, bool pruneMore);

// flattenFunctions.cpp
void flattenNestedFunctions(Vec<FnSymbol*>& nestedFunctions);

// callDestructors.cpp
void insertReferenceTemps(CallExpr* call);

// parallel.cpp
bool isRefWideString(Type* t);
bool isWideString(Type* t);
Type* getOrMakeRefTypeDuringCodegen(Type* type);
Type* getOrMakeWideTypeDuringCodegen(Type* refType);

// type.cpp
void initForTaskIntents();

#endif
