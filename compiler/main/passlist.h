#ifndef _PASSLIST_H_
#define _PASSLIST_H_

//
// prototypes of functions that are called as passes (alphabetical)
//
void build_default_functions();
void check_normalized();
void check_parsed();
void check_resolved();
void cleanup();
void codegen();
void copyPropagation();
void cullOverReferences();
void flattenFunctions();
void inlineFunctions();
void memoryManage();
void normalize();
void parallel1();
void parallel2();
void parse();
void resolve();
void scopeResolve();

//
// passlist: contains passes in the order that they are called
//
PassInfo passlist[] = {
  FIRST,
  // Chapel to AST
  RUN(parse),        // parse files and create AST
  RUN(check_parsed), // checks semantics of parsed AST

  // Scope resolution and normalization
  RUN(parallel1),        // parallel transforms, part 1
  RUN(cleanup),          // post parsing transformations
  RUN(scopeResolve),     // resolve symbols by scope
  RUN(normalize),        // normalization transformations
  RUN(check_normalized), // check semantics of normalized AST

  // Creation of default functions
  RUN(build_default_functions), // build default functions

  // Function resolution and shallow type inference
  RUN(resolve),        // resolves function calls and types
  RUN(check_resolved), // checks semantics of resolved AST

  // Post-resolution cleanup
  RUN(flattenFunctions),   // denest nested functions
  RUN(cullOverReferences), // remove excess references
  RUN(parallel2),          // parallel transforms, part 2

  // Optimizations
  RUN(inlineFunctions), // function inlining
  RUN(copyPropagation), // copy propagation and other low-level optimizations
  RUN(memoryManage),    // insert memory frees, garbage collection

  // AST to C
  RUN(codegen), // generate C code and build it
  LAST
};

#endif
