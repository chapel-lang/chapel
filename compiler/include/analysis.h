#ifndef _analysis_H_
#define _analysis_H_

#include "ast.h"
#include "callbacks.h"
#include "chplalloc.h"

class Symbol;
class UnresolvedSymbol;
class MemberAccess;
class BaseAST;
class Expr;
class Label;
class Code;
class Type;
class BaseAST;
class FnSymbol;
class Stmt;
class ASTCopyContext;
class Sym;
class TypeSymbol;
class StructuralType;
class VarSymbol;

class ACallbacks : public Callbacks {
public:
  void finalize_functions();
  Sym *make_LUB_type(Sym *);
  Sym *instantiate(Sym *, Map<Sym *, Sym *> &substitutions);
  Sym *formal_to_generic(Sym*);
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

  BaseAST *xast;        // pointer to shadowed BaseAST
  Code *code;           // IF1 Code (including children)
  Code *send;           // used for 0-arity parenops
  Label *label[2];      // before and after for loops (continue,break)
  Sym *sym, *rval;      // IF1 Syms

  AInfo();
};

int AST_to_IF1(Vec<Stmt *> &stmts);
Type *type_info(BaseAST *a, Symbol *s = 0);
Type *return_type_info(FnSymbol *fn);
Type *element_type_info(TypeSymbol *t);
int function_is_used(FnSymbol *fn);
int type_is_used(TypeSymbol *fn);
int AST_is_used(BaseAST *a, Symbol *s = 0);  
int function_returns_void(FnSymbol *fn);
enum { CALL_INFO_FIND_SINGLE, CALL_INFO_FIND_OPERATOR, CALL_INFO_FIND_FUNCTION };
int call_info(Expr *a, Vec<FnSymbol *> &fns, int find_type = CALL_INFO_FIND_SINGLE);
int constant_info(BaseAST *a, Vec<Symbol *> &constants, Symbol *s = 0);
void resolve_member_access(Expr *e, int *offset, Type **type); 
void resolve_member(StructuralType *t, VarSymbol *v, int *offset, Type **type); 
void structural_subtypes(Type *t, Vec<Type *> subtypes);

#endif
