#ifndef _PASSLIST_H_
#define _PASSLIST_H_

//
// prototypes of functions that are called as passes (alphabetical)
//
void buildDefaultFunctions();
void checkNormalized();
void checkParsed();
void checkResolved();
void cleanup();
void codegen();
void copyPropagation();
void cullOverReferences();
void flattenClasses();
void flattenFunctions();
void inlineFunctions();
void insertLineNumbers();
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
  RUN(parse),       // parse files and create AST
  RUN(checkParsed), // checks semantics of parsed AST

  // Scope resolution and normalization
  RUN(parallel1),       // parallel transforms, part 1
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
  RUN(cullOverReferences), // remove excess references
  RUN(parallel2),          // parallel transforms, part 2

  // Optimizations
  RUN(inlineFunctions), // function inlining
  RUN(copyPropagation), // copy propagation and other low-level optimizations
  RUN(memoryManage),    // insert memory frees, garbage collection

  // AST to C
  RUN(insertLineNumbers), // insert line numbers for error messages
  RUN(codegen),           // generate C code and build it
  LAST
};

#endif
