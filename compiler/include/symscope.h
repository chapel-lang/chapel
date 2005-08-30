#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "chplalloc.h"
#include "map.h"
#include "symbol.h"

enum scopeType {
  // builtins at the global level
  SCOPE_INTRINSIC = -3,
  SCOPE_PRELUDE = -2,
  SCOPE_POSTPARSE = -1,
  SCOPE_MODULE = 0,           // less is all modules
  SCOPE_ARG,
  SCOPE_LOCAL,
  SCOPE_FORLOOP,
  SCOPE_FORALLEXPR,
  SCOPE_LETEXPR,
  SCOPE_CLASS,
};

class SymtabTraversal;
class ScopeLookupCache;

class SymScope : public gc {
 public:
  scopeType type;

  ScopeLookupCache *lookupCache;

  BaseAST* astParent; // back pointer to AST

  SymScope* parent;
  SymScope* child;
  SymScope* sibling;

  Vec<ModuleSymbol*> uses; // modules used by this scope

  Vec<Symbol*> symbols;

  Map<char*,Vec<FnSymbol*>*> visibleFunctions;

  ChainHashMap<char*, StringHashFns, Symbol*> table;

  SymScope(scopeType init_type);
  void setASTParent(BaseAST* ast);

  void traverse(SymtabTraversal* traversal);

  bool isEmpty(void);

  void define(Symbol* sym);
  void undefine(Symbol* sym);

  void print(FILE* outfile = stdout, bool tableOrder = false);

  int parentLength(void);

  // these are "private"
  char* indentStr(void);
  void printHeader(FILE* outfile);
  void printSymbols(FILE* outfile, bool tableOrder);
  void printFooter(FILE* outfile);

  void codegen(FILE* outfile, char* separator);
  void codegenFunctions(FILE* outfile);

  ModuleSymbol* SymScope::getModule();

  void addVisibleFunction(FnSymbol* fn);
  void removeVisibleFunction(FnSymbol* fn);
  void setVisibleFunctions(FnSymbol* fn);
  void printVisibleFunctions();

  void getVisibleFunctions(Vec<FnSymbol*>* allVisibleFunctions, char* name,
                           bool recursed = false);
}; 

void getSymbols(SymScope *scope, Vec<Symbol *> &symbols);

#endif
