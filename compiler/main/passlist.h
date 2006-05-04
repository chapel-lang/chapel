#ifndef _PASSLIST_H_
#define _PASSLIST_H_

void parse();
void check_parsed();
void parallel();
void cleanup();
void scopeResolve();
void normalize();
void check_normalized();
void build_default_functions();
void functionResolution();
void runInterpreter();
void runAnalysis();
void resolve_analyzed();
void check_resolved();
void flattenFunctions();
void inlineFunctions();
void copy_propagation();
void codegen();

PassInfo passlist[] = {
  FIRST,
  RUN(parse),                   // parse files and create AST
  RUN(check_parsed),            // checks semantics of parsed AST
  RUN(parallel),                // parallel passes
  RUN(cleanup),                 // post parsing transformations
  RUN(scopeResolve),            // resolve symbols by scope
  RUN(normalize),               // normalization transformations
  RUN(check_normalized),        // check semantics of normalized AST
  RUN(build_default_functions), // build default functions

  RUN(functionResolution), // --no-infer function resolution

  RUN(runInterpreter), // INTERPRETER
                       //   Enabled by -i or --interpreter.
                       //   Terminates after running if enabled.

  RUN(runAnalysis),      // analysis
  RUN(resolve_analyzed), // resolve functions/types with analysis information

  RUN(check_resolved), // checks semantics of resolved AST

  RUN(flattenFunctions),

  RUN(inlineFunctions), // function inlining
  RUN(copy_propagation), // not implemented yet
  RUN(codegen),
  LAST
};

#endif
