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
  RUN(Cleanup, ""),
  RUN(Fixup, ""),

  // passes to run analysis
  RUN(Fixup, "verify"),  // this is a sanity check
  RUN(RunAnalysis, ""),

  // passes to rewrite AST after analysis
  RUN(ResolveSymbols, ""),
  RUN(FindUnknownTypes, ""),
  RUN(MethodsToFunctions, ""),

  // passes to generate code and compile
  //  RUN(Fixup, "verify"),  // BLC: We want this, but it doesn't work yet
  RUN(Codegen, ""),
  RUN(BuildBinary, ""),

  LAST
};

#endif
