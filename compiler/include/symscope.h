#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "gc_cpp.h"
#include "map.h"
#include "symbol.h"

enum scopeType {
  SCOPE_INTRINSIC = -4,
  SCOPE_INTERNAL_PRELUDE = -3,
  SCOPE_PRELUDE = -2,
  SCOPE_POSTPARSE = -1,
  SCOPE_MODULE = 0,
  SCOPE_PARAM,
  SCOPE_FUNCTION,
  SCOPE_LOCAL,
  SCOPE_FORLOOP,
  SCOPE_FORALLEXPR,
  SCOPE_CLASS,
};

class SymLink;

class SymScope : public gc {
 public:
  scopeType type;
  int level;

  Stmt* stmtContext;
  Symbol* symContext;
  Expr* exprContext;

  SymScope* parent;
  SymScope* child;
  SymScope* sibling;

  SymLink* firstSym;
  SymLink* lastSym;

  SymLink* useBeforeDefSyms;

  ChainHashMap<char*, StringHashFns, Symbol*> table;

  SymScope(scopeType init_type, int init_level = 0);
  void setContext(Stmt* stmt, Symbol* sym = nilSymbol, Expr* expr = nilExpr);

  bool isEmpty(void);

  void insert(Symbol* sym);
  SymScope* findEnclosingScopeType(scopeType t);

  void addUndefined(UnresolvedSymbol*);
  void addUndefinedToFile(UnresolvedSymbol*);
  void handleUndefined(void);

  void print(FILE* outfile = stdout, bool tableOrder = false);
};

#endif
