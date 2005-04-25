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
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(ExpandClassWiths, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertThisParameters, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(ScopeResolveSymbols, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(ScopeResolveGotos, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(RenameOverloadedFunctions, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertTemps, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertAnonymousDomains, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertAnonymousTypes, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertIndexType, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(BuildClassConstructorsEtc, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(SpecializeParenOpExprs, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(ApplyThisParameters, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(RemoveLikeTypes, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertDefaultInitVariables, ""),

  // passes to collect information for analysis
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(VerifyASTType, ""),
  RUN(ComputeVisibleFunctions, ""),

  // passes to run analysis
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(RunAnalysis, ""),

  // passes to run if --noanalysis is used
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(OmitForNoAnalysis, ""),

  // passes to capture analysis information in the AST
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(RemoveDeadSymbols, ""),
  RUN(ResolveTypes, ""),
  RUN(ResolveTupleTypes, ""),
  RUN(ResolveSymbols, ""),
  //RUN(FindUnknownTypes, ""),
  RUN(RemoveTypeVariableActuals, ""),
  RUN(RemoveTypeVariableFormals, ""),
  RUN(ResolveOverloadedOperators, ""),
  RUN(RemoveNamedParameters, ""),

  // passes to normalize the basic AST after analysis
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertWriteFns, ""),
  RUN(InsertElidedLoops, ""),

  // check the program's semantics
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(CheckSemantics, ""),

  // eventually, optimizations will go here

  // passes to prepare for C code generation
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(RemoveSeqOperations, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertFunctionTemps, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(TransformLetExprs, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(ProcessParameters, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertVariableInitializations, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(DestructureTupleAssignments, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(ExpandSeqExprAssignments, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertUnionChecks, ""),
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(LegalizeCNames, ""), 

  // passes to generate code and compile
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(Codegen, ""),
  RUN(BuildBinary, ""),

  LAST
};

#endif
