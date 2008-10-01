#ifndef _PASSLIST_H_
#define _PASSLIST_H_

#include "passes.h"

//
// passlist: contains passes in the order that they are called
//
PassInfo passlist[] = {
  FIRST,
  // Chapel to AST
  RUN(parse),       // parse files and create AST
  RUN(checkParsed), // checks semantics of parsed AST

  // Scope resolution and normalization
  RUN(cleanup),         // post parsing transformations
  RUN(scopeResolve),    // resolve symbols by scope
  RUN(flattenClasses),  // denest nested classes
  RUN(normalize),       // normalization transformations
  RUN(checkNormalized), // check semantics of normalized AST

  // Creation of default functions
  RUN(buildDefaultFunctions), // build default functions

  // Function resolution and shallow type inference
  RUN(resolve),       // resolves function calls and types
  RUN(checkResolved), // checks semantics of resolved AST

  // Post-resolution cleanup
  RUN(flattenFunctions),   // denest nested functions
  RUN(lowerIterators),     // lowers iterators into functions/classes
  RUN(cullOverReferences), // remove excess references
  RUN(parallel),           // parallel transforms
  RUN(insertDestructors),  // insert calls to destructors when variables go out of scope
  RUN(prune),              // prune AST of dead functions and types

  // Optimizations
  RUN(complex2record),      // change complex numbers into records
  RUN(inlineFunctions),     // function inlining
  RUN(scalarReplace),       // scalar replace all tuples
  RUN(refPropagation),      // reference propagation
  RUN(copyPropagation),     // copy propagation
  RUN(deadCodeElimination), // eliminate dead code

  RUN(insertWideReferences), // inserts wide references for on clauses

  // AST to C
  RUN(copyCollection),    // insert code needed for copying garbage collection
  RUN(insertLineNumbers), // insert line numbers for error messages
  RUN(codegen),           // generate C code
  RUN(makeBinary),        // invoke underlying C compiler
  LAST
};

#endif
