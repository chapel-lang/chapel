#ifndef _analysis_H_
#define _analysis_H_

#include "ast.h"
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
class ASTCopyContext;
class Sym;
class TypeSymbol;

class ACallbacks : public Callbacks {
public:
  void finalize_functions();
  Sym *make_LUB_type(Sym *);
  Sym *instantiate(Sym *, Map<Sym *, Sym *> &substitutions);
  Sym *new_Sym(char *name = 0);
  Fun *order_wrapper(Match *);
  Fun *coercion_wrapper(Match *);
  Fun *default_wrapper(Match *);
  Fun *instantiate_generic(Match *);
};

class CloneCallback : public gc {
 public:
  virtual void clone(BaseAST* old_ast, BaseAST* new_ast) = 0;
};

class AnalysisCloneCallback : public CloneCallback {
 public:
  ASTCopyContext *context;
  void clone(BaseAST* old_ast, BaseAST* new_ast);
  AnalysisCloneCallback() : context(0) {}
};

class ASymbol : public gc {
 public:
  // Sym interface
  Sym *clone(CloneCallback *);
  char *pathname();
  int line();
  ASymbol *copy();

  BaseAST *symbol;
  Sym *sym;

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
  Vec<Fun *> *visible_functions(Sym *arg0);

  BaseAST *xast;	// pointer to shadowed BaseAST
  Code *code;		// IF1 Code
  Label *label[2];	// before and after for loops (continue,break)
  Sym *sym, *rval;	// IF1 Syms

  AInfo();
};

int AST_to_IF1(Vec<Stmt *> &stmts);
Type *type_info(BaseAST *a, Symbol *s = 0);
Type *return_type_info(FnSymbol *fn);
int function_is_used(FnSymbol *fn);
int type_is_used(TypeSymbol *fn);
void call_info(ParenOpExpr *a, Vec<FnSymbol *> &fns);
int constant_info(BaseAST *a, Vec<Symbol *> &constants, Symbol *s = 0);
int resolve_symbol(UnresolvedSymbol *us, MemberAccess *ma, Symbol *&sym);

#endif
