#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "gc_cpp.h"
#include "map.h"
#include "symbol.h"

enum scopeType {
	//builtins at the global level
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
  SCOPE_LETEXPR,
  SCOPE_CLASS,
};

class SymLink;
class SymtabTraversal;
class ScopeLookupCache;

class SymScope : public gc {
 public:
  scopeType type;
  int level;
  
  ScopeLookupCache *lookupCache;

	//statement context
  Stmt* stmtContext;
  //symbol context
  Symbol* symContext;
  Expr* exprContext;

  SymScope* parent;
  SymScope* child;
  SymScope* sibling;

  SymLink* firstSym;
  SymLink* lastSym;

  Map<char *,Vec<FnSymbol*> *> visibleFunctions;

  ChainHashMap<char*, StringHashFns, Symbol*> table;

  SymScope(scopeType init_type, int init_level = 0);
  void setContext(Stmt* stmt, Symbol* sym = NULL, Expr* expr = NULL);

  void traverse(SymtabTraversal* traversal);

  bool isEmpty(void);
  bool isInternal(void);

  void insert(Symbol* sym);
  //void remove(Symbol* sym);
  SymScope* findEnclosingScopeType(scopeType t);

  Symbol* findEnclosingSymContext();
  Stmt* findEnclosingStmtContext();
  Expr* findEnclosingExprContext();

  void print(FILE* outfile = stdout, bool tableOrder = false);

  // these are "private"
  char* indentStr(void);
  void printHeader(FILE* outfile);
  void printSymbols(FILE* outfile, bool tableOrder);
  void printFooter(FILE* outfile);

  void codegen(FILE* outfile, char* separator);

  bool commonModuleIsFirst();
  void setVisibleFunctions(Vec<FnSymbol*>* moreVisibleFunctions);
  void printVisibleFunctions();
};

#endif
