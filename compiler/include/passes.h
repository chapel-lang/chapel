#ifndef _PASSES_H
#define _PASSES_H_

#include "symbol.h"

extern bool normalized;
extern bool resolved;


//
// prototypes of functions that are called as passes (alphabetical)
//
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
void flattenClasses();
void flattenFunctions();
void gpuFlattenArgs();
void inlineFunctions();
void insertLineNumbers();
void insertWideReferences();
void optimizeOnClauses();
void localizeGlobals();
void lowerIterators();
void makeBinary();
void normalize();
void parallel();
void parse();
void prune();
void refPropagation();
void removeEmptyRecords();
void removeUnnecessaryAutoCopyCalls();
void removeWrapRecords();
void returnStarTuplesByRefArgs();
void resolve();
void scalarReplace();
void scopeResolve();
void verify();

#endif
