#ifndef _analysis_H_
#define _analysis_H_

#include "ast.h"
#include "sym.h"
#include "callbacks.h"
#include "gc_cpp.h"

class Symbol;
class UnresolvedSymbol;
class MemberAccess;
class BaseAST;
class ParenOpExpr;
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
  char *pathname();
  int line();

  int is_aggregate();
  Sym *element_type();
  
  BaseAST *xsymbol;

  ASymbol();
};

class AInfo : public AST {
 public:
  // AST interface
  char *pathname();
  int line();
  Sym *symbol();  
  AST *copy_tree(ASTCopyContext* context);
  AST *copy_node(ASTCopyContext* context);
  Vec<Fun *> *visible_functions(char *name);

  BaseAST *xast;	// pointer to shadowed BaseAST
  Code *code;		// IF1 Code
  Label *label[2];	// before and after for loops (continue,break)
  Sym *sym, *rval;	// IF1 Syms

  AInfo();
};

class CloneCallback : public gc {
 public:
  virtual void clone(BaseAST* old_ast, BaseAST* new_ast) = 0;
};

int AST_to_IF1(Vec<Stmt *> &stmts);
Type *type_info(BaseAST *a, Symbol *s = 0);
Type *return_type_info(FnSymbol *fn);
void call_info(ParenOpExpr *a, Vec<FnSymbol *> &fns);
int constant_info(BaseAST *a, Vec<Symbol *> &constants, Symbol *s = 0);
int resolve_symbol(UnresolvedSymbol *us, MemberAccess *ma, Symbol *&sym);

#endif
