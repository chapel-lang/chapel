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
  RUN(Fixup, ""),

  // passes to normalize the basic AST
  RUN(Verify, ""), // this is a sanity check
  RUN(ExpandClassWiths, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertAnonymousDomains, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertLiteralTemps, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(BuildLValueFunctions, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ReconstructIterators, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertThisParameters, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(Flatten, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ResolveModuleUses, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ScopeResolveSymbols, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ScopeResolveGotos, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(RenameOverloadedFunctions, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertAnonymousTypes, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertIndexType, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(BuildClassConstructorsEtc, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ApplyThisParameters, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ApplyGettersSetters, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(RemoveLikeTypes, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertDefaultInitVariables, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertOutParameterInitializations, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(EliminateReturns, ""),

  // passes to collect information for analysis
  RUN(Verify, ""), // this is a sanity check
  RUN(VerifyASTType, ""),
  RUN(ComputeVisibleFunctions, ""),

  // passes to run analysis
  RUN(Verify, ""), // this is a sanity check
  RUN(Instantiate, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(SpecializeParenOpExprs, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(PreAnalysisHacks, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(RunAnalysis, ""),
 
  // passes to capture analysis information in the AST
  RUN(Verify, ""), // this is a sanity check
  RUN(RemoveDeadSymbols, ""),
  RUN(ResolveTypes, ""),
  RUN(PostAnalysisCleanup, ""),
  RUN(ResolveTupleTypes, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ResolveSymbols, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(RemoveNamedParameters, ""),
  RUN(RemoveTypeVariableActuals, ""),
  RUN(RemoveTypeVariableFormals, ""),

  //passed to canonicalize AST
  RUN(Verify,""),
  RUN(SimpleNestedFunctionMove, ""),
  RUN(Verify, ""),
  RUN(RemoveNestedFunctions, ""),

  // passes to normalize the basic AST after analysis
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertElidedLoops, ""),

  // check the program's semantics
  RUN(Verify, ""), // this is a sanity check
  RUN(CheckSemantics, ""),

  // eventually, optimizations will go here

  // passes to prepare for C code generation
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertFunctionTemps, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(TransformLetExprs, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(EliminateReturns, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(ProcessParameters, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InlineFunctions, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertVariableInitializations, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(InsertUnionChecks, ""),
  RUN(Verify, ""), // this is a sanity check
  RUN(LegalizeCNames, ""), 

  //RED: pass to resolve name conflicts due to overloading
  RUN(Verify, ""),
  RUN(FindCollidingNames, ""),
  
  // passes to generate code and compile
  RUN(Verify, ""), // this is a sanity check
  RUN(CodegenOne, ""), // types and function prototypes
  RUN(Codegen, ""),
  RUN(BuildBinary, ""),

  LAST
};

#endif
