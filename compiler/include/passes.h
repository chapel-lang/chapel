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
void callDestructors();
void checkNormalized();
void checkParsed();
void checkResolved();
void cleanup();
void codegen();
void complex2record();
void createTaskFunctions();
void copyPropagation();
void cullOverReferences();
void deadCodeElimination();
void docs();
void flattenClasses();
void flattenFunctions();
void inlineFunctions();
void insertLineNumbers();
void insertWideReferences();
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
void returnStarTuplesByRefArgs();
void resolve();
void resolveIntents();
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

//
// utility functions in pass-containing code files
//

// parallel.cpp
bool isRefWideString(Type* t);
bool isWideString(Type* t);
Type* getOrMakeRefTypeDuringCodegen(Type* type);
Type* getOrMakeWideTypeDuringCodegen(Type* refType);

#endif
