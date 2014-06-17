/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _PASSES_H
#define _PASSES_H_

#include "symbol.h"

extern bool normalized;
extern bool resolved;


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
void copyPropagation();
void cullOverReferences();
void deadCodeElimination();
void docs();
void flattenClasses();
void flattenFunctions();
void gpuFlattenArgs();
void inlineFunctions();
void insertLineNumbers();
void insertWideReferences();
void localizeGlobals();
void lowerIterators();
void makeBinary();
void normalize();
void optimizeOnClauses();
void parallel();
void parse();
void prune();
void prune2();
void readExternC();
void refPropagation();
void removeEmptyRecords();
void removeUnnecessaryAutoCopyCalls();
void removeWrapRecords();
void repositionDefExpressions();
void returnStarTuplesByRefArgs();
void resolve();
void scalarReplace();
void scopeResolve();
void verify();

//
// utility functions in pass-containing code files
//

// parallel.cpp
bool passingWideStringToExtern(Type* t);
Type* getOrMakeRefTypeDuringCodegen(Type* type);
Type* getOrMakeWideTypeDuringCodegen(Type* refType);

#endif
