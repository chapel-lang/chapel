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

  RUN(FilesToAST, ""),
  RUN(CreateEntryPoint, ""),
  RUN(Fixup, ""),
  RUN(Cleanup, ""),
  RUN(Fixup, ""),
  RUN(RunAnalysis, ""),
  RUN(ResolveSymbols, ""),
  RUN(FindUnknownTypes, ""),
  RUN(MethodsToFunctions, ""),
  RUN(Codegen, ""),
  RUN(BuildBinary, ""),

  LAST
};

#endif
