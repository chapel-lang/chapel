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
  RUN(ExpandClassWiths, ""),
  RUN(InsertThisParameters, ""),
  RUN(ScopeResolveSymbols, ""),
  RUN(RenameOverloadedFunctions, ""),
  RUN(BuildClassConstructorsEtc, ""),
  RUN(SpecializeParenOpExprs, ""),
  RUN(ApplyThisParameters, ""),
  RUN(InsertAnonymousDomains, ""),
  RUN(InsertAnonymousTypes, ""),

  // passes to run analysis
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(RunAnalysis, ""),

  // passes to capture analysis information in the AST
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(RemoveDeadSymbols, ""),
  RUN(ResolveSymbols, ""),
  RUN(FindUnknownTypes, ""),
  RUN(RemoveTypeVariableActuals, ""),
  RUN(RemoveTypeVariableFormals, ""),
  RUN(ResolveOverloadedOperators, ""),
  RUN(RemoveNamedParameters, ""),

  // passes to normalize the basic AST after analysis
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(InsertFunctionTemps, ""),
  RUN(TransformLetExprs, ""),
  RUN(InsertElidedLoops, ""),

  // check the program's semantics
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(CheckSemantics, ""),

  // eventually, optimizations will go here

  // passes to prepare for C code generation
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(MethodsToFunctions, ""),
  RUN(ProcessParameters, ""),
  RUN(InsertVariableInitializations, ""),
  RUN(DestructureTupleAssignments, ""),
  RUN(InsertUnionChecks, ""),
  RUN(LegalizeCNames, ""), 

  // passes to generate code and compile
  RUN(Fixup, "verify"), // this is a sanity check
  RUN(Codegen, ""),
  RUN(BuildBinary, ""),

  LAST
};

#endif
