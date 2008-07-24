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
  Vec<Symbol*>     actualSyms;  // symbols of actuals
  Vec<Type*>       actualTypes; // types of actuals
  Vec<const char*> actualNames; // named arguments
  CallInfo(CallExpr* icall);
};

#endif
