#ifndef _PASSLIST_H_
#define _PASSLIST_H_

/* This is the default list of passes that will be run by the
   compiler.  The passes shown will be run in the order shown,
   and their arguments will be set to the quoted string that
   follows.

   This file may be saved and fed to the compiler using the
   --passlist argument to specify a different set of passes
   dynamically.
*/

PassInfo passlist[] = {
  FIRST,

  // passes to create the basic AST
  RUN(parse),
  RUN(createEntryPoint),
  RUN(normalizeParsedAST), // handles complicated parsing transforms
  RUN(semanticCheckI), // post parsing semantic checks

  // passes to normalize the basic AST
  RUN(processImportExprs),
  RUN(flatten),
  RUN(buildClassHierarchy),

  RUN(buildDefaultFunctions),

  RUN(insertThisParameters),

  // SCOPE RESOLUTION
  RUN(scopeResolve),

  RUN(semanticCheckII), // post scope resolution semantic checks

  RUN(buildLValueFunctions),
  RUN(reconstructIterators),

  RUN(insertDefaultInitVariables),
  RUN(insertOutParameterInitializations),
  RUN(eliminateReturns),
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


  RUN(semanticCheckIII), // post analysis semantic checks

  RUN(createNestedFuncIterators),
  RUN(removeNestedFunctions),

  // eventually, optimizations will go here

  // passes to prepare for C code generation
  RUN(insertFunctionTemps),
  RUN(transformLetExprs),
  RUN(eliminateReturns),
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
