#ifndef _PASSLIST_H_
#define _PASSLIST_H_

/* This is the default list of passes that will be run by the
   compiler.  The passes shown will be run in the order shown.

   This file may be saved and fed to the compiler using the
   --passlist argument to specify a different set of passes
   dynamically.
*/

PassInfo passlist[] = {
  FIRST,

  // passes to create the basic AST
  RUN(parse),
  RUN(check_parsed), // checks semantics of parsed AST
  RUN(createEntryPoint),

  RUN(processImportExprs),

  RUN(buildClassHierarchy),

  RUN(buildDefaultFunctions),

  RUN(cleanup), // post parsing transformations

  RUN(scopeResolve), // resolve symbols by scope

  RUN(normalize), // normalization transformations

  RUN(check_normalized), // checks semantics of normalized AST

  RUN(normalizeFunctions),

  RUN(specializeCallExprs),
  RUN(preAnalysisHacks),
  RUN(applyGettersSetters),

  // FunctionResolution instantiates types, resolves functions, and
  // computes a very basic form of type inference.  It is run with the
  // --no-infer or -b flag.  It replaces all the passes between its
  // two calls in this passlist.
  RUN(functionResolution),

  RUN(pre_instantiate),
  RUN(preAnalysisCleanup),

  // ANALYSIS
  RUN(runAnalysis),

  // passes to capture analysis information in the AST
  RUN(removeDeadSymbols),
  RUN(resolveTypes),
  RUN(postAnalysisCleanup),
  RUN(resolveSymbols),

  RUN(functionResolution),

  RUN(removeNamedParameters),
  RUN(removeTypeVariableActuals),
  RUN(removeTypeVariableFormals),


  RUN(check_resolved), // checks semantics of resolved AST

  RUN(createNestedFuncIterators),
  RUN(removeNestedFunctions),

  // eventually, optimizations will go here

  // passes to prepare for C code generation
  RUN(insertFunctionTemps),
  RUN(transformLetExprs),
  RUN(processParameters),
  RUN(inlineFunctions),
  RUN(insertVariableInitializations),

  RUN(copyPropagation),

  // passes to generate code and compile
  RUN(codegenOne), // codegen types and function prototypes
  RUN(codegen),

  LAST
};

#endif
