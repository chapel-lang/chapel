#ifndef _PASSLIST_H_
#define _PASSLIST_H_

#include "checkTypeInfo.h"
#include "findUnknownTypes.h"
#include "getstuff.h"
#include "printAST.h"
#include "printSymtab.h"
#include "testGetStuff.h"
#include "verifyASTType.h"


/* This is the default list of passes that will be run by the
   compiler.  The passes shown will be run in the order shown. */

Pass* passlist[] = {
  FIRST,

  RUN(FindUnknownTypes),

  LAST
};

#endif
