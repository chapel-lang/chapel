// checks.h
#ifndef CHECKS_H
#define CHECKS_H

// These are the entry points for per-pass checks.
void check_parse();
void check_checkParsed();
void check_readExternC();
void check_cleanup();
void check_scopeResolve();
void check_flattenClasses();
void check_docs();
void check_normalize();
void check_checkNormalized();
void check_buildDefaultFunctions();
void check_createTaskFunctions();
void check_resolve();
void check_resolveIntents();
void check_checkResolved();
void check_processIteratorYields();
void check_flattenFunctions();
void check_cullOverReferences();
void check_callDestructors();
void check_lowerIterators();
void check_parallel();
void check_prune();
void check_complex2record();
void check_removeUnnecessaryAutoCopyCalls();
void check_inlineFunctions();
void check_scalarReplace();
void check_refPropagation();
void check_copyPropagation();
void check_deadCodeElimination();
void check_removeWrapRecords();
void check_removeEmptyRecords();
void check_localizeGlobals();
void check_loopInvariantCodeMotion();
void check_prune2();
void check_returnStarTuplesByRefArgs();
void check_insertWideReferences();
void check_optimizeOnClauses();
void check_addInitCalls();
void check_insertLineNumbers();
void check_codegen();
void check_makeBinary();

#endif
