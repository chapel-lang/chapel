#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "gc_cpp.h"
#include "map.h"
#include "symbol.h"

enum scopeType {
  SCOPE_INTRINSIC = -1,
  SCOPE_INTERNAL_PRELUDE = 0,
  SCOPE_PRELUDE = 1,
  SCOPE_FILE = 2,
  SCOPE_PARAM,
  SCOPE_FUNCTION,
  SCOPE_LOCAL,
  SCOPE_FORLOOP,
  SCOPE_CLASS
};

class SymLink;

class SymScope : public gc {
 public:
  scopeType type;
  int level;

  Stmt* stmtContext;
  Symbol* symContext;

  SymScope* parent;
  SymScope* child;
  SymScope* sibling;

  SymLink* firstSym;
  SymLink* lastSym;

  SymLink* useBeforeDefSyms;

  ChainHashMap<char*, StringHashFns, Symbol*> table;

  SymScope(scopeType init_type, int init_level);
  void setContext(Stmt* stmt, Symbol* sym = nilSymbol);

  void insert(Symbol* sym);
  SymScope* findEnclosingScopeType(scopeType t);

  void addUndefined(UseBeforeDefSymbol*);
  void addUndefinedToFile(UseBeforeDefSymbol*);
  void handleUndefined(void);

  void print(FILE* outfile = stdout, bool tableOrder = false);
};

#endif
