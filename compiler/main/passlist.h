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
  RUN(Fixup, ""), // postFixup = true

  // passes to normalize the basic AST
  RUN(ProcessImportExprs, ""),
  RUN(InsertAnonymousTypes, ""),
  RUN(InsertLiteralTemps, ""),
  RUN(BuildLValueFunctions, ""),
  RUN(ReconstructIterators, ""),
  RUN(InsertThisParameters, ""),
  RUN(Flatten, ""),
  RUN(BuildClassHierarchy, ""),
  RUN(ScopeResolveSymbols, ""), // postScopeResolution = true
  RUN(ScopeResolveGotos, ""),
  RUN(RenameOverloadedFunctions, ""),
  RUN(BuildClassConstructorsEtc, ""),
  RUN(ApplyThisParameters, ""),
  RUN(RemoveLikeTypes, ""),
  RUN(InsertDefaultInitVariables, ""),
  RUN(InsertOutParameterInitializations, ""),
  RUN(EliminateReturns, ""),

  // passes to run analysis
  RUN(Instantiate, ""),
  RUN(SpecializeCallExprs, ""),
  RUN(PreAnalysisHacks, ""),
  RUN(ApplyGettersSetters, ""),
  RUN(PreAnalysisCleanup, ""),
  RUN(RunAnalysis, ""),
 
  // passes to capture analysis information in the AST
  RUN(RemoveDeadSymbols, ""),
  RUN(ResolveTypes, ""),
  RUN(PostAnalysisCleanup, ""),
  RUN(ResolveSymbols, ""),
  RUN(RemoveNamedParameters, ""),
  RUN(RemoveTypeVariableActuals, ""),
  RUN(RemoveTypeVariableFormals, ""), // postAnalysis = true

  //passed to canonicalize AST
  RUN(CreateNestedFuncIterators, ""),
  
  RUN(SimpleNestedFunctionMove, ""),
  RUN(RemoveNestedFunctions, ""),

  // check the program's semantics
  RUN(CheckSemantics, ""),

  // eventually, optimizations will go here

  // passes to prepare for C code generation
  RUN(InsertFunctionTemps, ""),
  RUN(TransformLetExprs, ""),
  RUN(EliminateReturns, ""),
  RUN(ProcessParameters, ""),
  RUN(InlineFunctions, ""),
  RUN(InsertVariableInitializations, ""),
  RUN(InsertUnionChecks, ""),
  RUN(LegalizeCNames, ""), 

  //RED: pass to resolve name conflicts due to overloading
  RUN(FindCollidingNames, ""),
  
  // passes to generate code and compile
  RUN(CodegenOne, ""), // codegen types and function prototypes
  RUN(Codegen, ""),
  RUN(BuildBinary, ""),

  LAST
};

#endif
