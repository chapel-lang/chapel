#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

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

class SymScope {
 public:
  scopeType type;
  int level;

  SymScope* parent;
  SymScope* children;
  SymScope* sibling;

  SymLink* firstSym;
  SymLink* lastSym;

  SymLink* useBeforeDefSyms;

  ChainHashMap<char*, StringHashFns, Symbol*> table;

  SymScope(scopeType init_type, int init_level);

  void insert(Symbol* sym);
  SymScope* findFileScope(void);

  void addUndefined(UseBeforeDefSymbol*);
  void addUndefinedToFile(UseBeforeDefSymbol*);
  void handleUndefined(void);

  void print(FILE* outfile = stdout, bool alphabetical = false);
};

#endif
