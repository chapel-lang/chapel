#ifndef _PASSES_H
#define _PASSES_H_

extern bool normalized;

//
// prototypes of functions that are called as passes (alphabetical)
//
void buildDefaultFunctions();
void checkNormalized();
void checkParsed();
void checkResolved();
void cleanup();
void codegen();
void complex2record();
void copyCollection();
void copyPropagation();
void cullOverReferences();
void deadCodeElimination();
void flattenClasses();
void flattenFunctions();
void inlineFunctions();
void insertDestructors();
void insertWideReferences();
void insertLineNumbers();
void lowerIterators();
void makeBinary();
void normalize();
void parallel();
void parse();
void prune();
void refPropagation();
void resolve();
void scalarReplace();
void scopeResolve();
void verify();

#endif
