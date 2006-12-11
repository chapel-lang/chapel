#ifndef _PASSLIST_H_
#define _PASSLIST_H_

void parse();
void check_parsed();
void parallel1();
void cleanup();
void scopeResolve();
void normalize();
void check_normalized();
void build_default_functions();
void resolve();
void check_resolved();
void flattenFunctions();
void parallel2();
void convertReturnsToArgs();
void destructureRecords();
void inlineFunctions();
void cullOverReferences();
void copyPropagation();
void memoryManage();
void codegen();

PassInfo passlist[] = {
  FIRST,
  RUN(parse),              // parse files and create AST
  RUN(check_parsed),       // checks semantics of parsed AST

  RUN(parallel1),          // parallel transforms, part 1

  RUN(cleanup),            // post parsing transformations
  RUN(scopeResolve),       // resolve symbols by scope
  RUN(normalize),          // normalization transformations
  RUN(check_normalized),   // check semantics of normalized AST

  RUN(build_default_functions), // build default functions

  RUN(resolve),            // resolves function calls and types
  RUN(check_resolved),     // checks semantics of resolved AST

  RUN(flattenFunctions),   // denest nested functions
  RUN(parallel2),          // parallel transforms, part 2
  RUN(convertReturnsToArgs), // pass records instead of returning them
  RUN(destructureRecords),   // destructure records into components
  RUN(inlineFunctions),    // function inlining

  RUN(cullOverReferences),

  RUN(copyPropagation),
  RUN(memoryManage),        // insert memory frees, garbage collection
  RUN(codegen),
  LAST
};

#endif
