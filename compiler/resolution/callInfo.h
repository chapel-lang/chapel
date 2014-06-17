/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _CALLINFO_H_
#define _CALLINFO_H_

#include "chpl.h"

class BlockStmt;
class CallExpr;
class Symbol;
class Type;

class CallInfo {
 public:
  CallExpr*        call;        // call expression
  BlockStmt*       scope;       // module scope as in M.call
  const char*      name;        // function name
  Vec<Symbol*>     actuals;     // actual symbols
  Vec<const char*> actualNames; // named arguments
  CallInfo(CallExpr* icall);
};

#endif
