#ifndef _analysis_H_
#define _analysis_H_

#include "ast.h"
#include "sym.h"
#include "callbacks.h"

class Symbol;
class BaseAST;
class Label;
class Code;
class Type;
class BaseAST;
class FnSymbol;
class Stmt;

class ACallbacks : public Callbacks {
public:
  void new_LUB_type(Sym *);
  Sym *new_Sym(char *name = 0);
};

class ASymbol : public Sym {
 public:
  // Sym interface
  Sym *copy();
  
  BaseAST *xsymbol;

  ASymbol();
};

class AInfo : public AST {
 public:
  // AST interface
  char *pathname();
  int line();
  Sym *symbol();
  AST *copy(Map<PNode *, PNode*> *nmap = 0);

  BaseAST *xast;	// pointer to shadowed BaseAST
  Code *code;		// IF1 Code
  Label *label[2];	// before and after for loops (continue,break)
  Sym *sym, *rval;	// IF1 Syms

  AInfo();
};

int AST_to_IF1(Vec<Stmt *> &stmts);
Type *type_info(BaseAST *a, Symbol *s = 0);
void call_info(FnSymbol *f, BaseAST *a, Vec<FnSymbol *> &fns);
int constant_info(BaseAST *a, Vec<Symbol *> &constants, Symbol *s = 0);

#endif
