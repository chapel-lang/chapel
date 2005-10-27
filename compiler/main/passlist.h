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
  RUN(FilesToAST, ""),
  RUN(CreateEntryPoint, ""),
  RUN(NormalizeParsedAST, ""), // handles complicated parsing transforms
  RUN(SemanticCheckI, ""), // post parsing semantic checks

  // passes to normalize the basic AST
  RUN(ProcessImportExprs, ""),
  RUN(InsertLiteralTemps, ""),
  RUN(InsertThisParameters, ""),
  RUN(Flatten, ""),
  RUN(BuildClassHierarchy, ""),

  // SCOPE RESOLUTION
  RUN(ScopeResolveSymbols, ""), // postScopeResolution = true
  RUN(ScopeResolveGotos, ""),
  RUN(ApplyThisParameters, ""),

  RUN(SemanticCheckII, ""), // post scope resolution semantic checks

  RUN(BuildDefaultFunctions, ""),

  RUN(BuildLValueFunctions, ""),
  RUN(ReconstructIterators, ""),

  RUN(InsertDefaultInitVariables, ""),
  RUN(InsertOutParameterInitializations, ""),
  RUN(EliminateReturns, ""),
  RUN(NormalizeFunctions, ""),

  RUN(SpecializeCallExprs, ""),
  RUN(PreAnalysisHacks, ""),
  RUN(ApplyGettersSetters, ""),

  // FunctionResolution instantiates types, resolves functions, and
  // computes a very basic form of type inference.  It is run with the
  // --no-infer or -b flag.  It replaces all the passes between its
  // two calls in this passlist.
  RUN(FunctionResolution, ""),

  RUN(Instantiate, ""),
  RUN(PreAnalysisCleanup, ""),

  // ANALYSIS
  RUN(RunAnalysis, ""), // postAnalysis = true

  // passes to capture analysis information in the AST
  RUN(RemoveDeadSymbols, ""),
  RUN(ResolveTypes, ""),
  RUN(PostAnalysisCleanup, ""),
  RUN(ResolveSymbols, ""),

  RUN(FunctionResolution, ""),

  RUN(RemoveNamedParameters, ""),
  RUN(RemoveTypeVariableActuals, ""),
  RUN(RemoveTypeVariableFormals, ""),


  RUN(SemanticCheckIII, ""), // post analysis semantic checks

  RUN(CreateNestedFuncIterators, ""),
  RUN(RemoveNestedFunctions, ""),

  // eventually, optimizations will go here

  // passes to prepare for C code generation
  RUN(InsertFunctionTemps, ""),
  RUN(TransformLetExprs, ""),
  RUN(EliminateReturns, ""),
  RUN(ProcessParameters, ""),
  RUN(InlineFunctions, ""),
  RUN(InsertVariableInitializations, ""),

  RUN(CopyPropagation, ""),

  // passes to generate code and compile
  RUN(CodegenOne, ""), // codegen types and function prototypes
  RUN(Codegen, ""),
  RUN(BuildBinary, ""),

  LAST
};

#endif
