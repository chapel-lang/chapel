#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "gc_cpp.h"
#include "map.h"
#include "symbol.h"

enum scopeType {
  SCOPE_INTRINSIC = -1,
  SCOPE_FILE = 0,
  SCOPE_PARAM,
  SCOPE_FUNCTION,
  SCOPE_LOCAL,
  SCOPE_FORLOOP
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
  SymScope* findFileScope(void);

  void addUndefined(UseBeforeDefSymbol*);
  void addUndefinedToFile(UseBeforeDefSymbol*);
  void handleUndefined(void);

  void print(FILE* outfile = stdout, bool alphabetical = false);
};

#endif
