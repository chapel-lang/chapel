#ifndef _PASSLIST_H_
#define _PASSLIST_H_

#include "../passes/filesToAST.h"
#include "../passes/printProgram.h"
#include "../passes/printSymtab.h"
#include "../passes/runAnalysis.h"

#include "../traversals/checkBackLinks.h"
#include "../traversals/checkIDs.h"
#include "../traversals/checkTypeInfo.h"
#include "../traversals/cloneAllFns.h"
#include "../traversals/findUnknownTypes.h"
#include "../traversals/getstuff.h"
#include "../traversals/printAST.h"
#include "../traversals/setupStmtLinks.h"
#include "../traversals/testGetStuff.h"
#include "../traversals/verifyASTType.h"


/* This is the default list of passes that will be run by the
   compiler.  The passes shown will be run in the order shown. */

Pass* passlist[] = {
  FIRST,

  RUN(FilesToAST),
  RUN(SetupStmtLinks),
  RUN(CheckBackLinks),
  RUN(RunAnalysis),
  RUN(FindUnknownTypes),

  LAST
};

#endif
