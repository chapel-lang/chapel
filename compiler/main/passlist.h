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

  RUN(parse),
  RUN(check_parsed), // checks semantics of parsed AST

  RUN(createEntryPoint), // fold into cleanup -- builds init fn for modules
  RUN(processImportExprs), // fold into cleanup -- expands with and use
  RUN(buildClassHierarchy), // fold into cleanup -- handles inheritance

  RUN(buildDefaultFunctions), // move after normalize, the new
                              // functions will then be cleaned up,
                              // looked up, and normalized

  RUN(cleanup), // post parsing transformations

  RUN(scopeResolve), // resolve symbols by scope

  RUN(normalize), // normalization transformations

  RUN(check_normalized), // checks semantics of normalized AST

  RUN(preAnalysisHacks),    // fold into normalize
  RUN(applyGettersSetters), // fold into normalize

  // FunctionResolution instantiates types, resolves functions, and
  // computes a very basic form of type inference.  It is run with the
  // --no-infer or -b flag.  It replaces all the passes between its
  // two calls in this passlist.
  RUN(functionResolution),

  RUN(pre_instantiate), // remove, analysis should instantiate

  RUN(preAnalysisCleanup),

  // ANALYSIS
  RUN(runAnalysis),

  RUN(resolve_analyzed),

  //  RUN(removeDeadSymbols),   // fold into resolve analysis
  RUN(resolveTypes),        // fold into resolve analysis
  RUN(postAnalysisCleanup), // fold into resolve analysis
  RUN(resolveSymbols),      // fold into resolve analysis

  RUN(functionResolution),

  RUN(removeNamedParameters),     // fold into resolve analysis
  RUN(removeTypeVariableActuals), // fold into resolve analysis
  RUN(removeTypeVariableFormals), // fold into resolve analysis

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
