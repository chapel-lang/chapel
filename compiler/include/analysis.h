#ifndef _analysis_H_
#define _analysis_H_

#include "ast.h"
#include "callbacks.h"
#include "chplalloc.h"
#include "alist.h"

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
class ClassType;
class VarSymbol;
class AVar;
class AType;

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
  int log_line();
  int ast_id();
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

enum AError_kind {
  AERROR_CALL_ARGUMENT,         // all types of a call argument not handled
  AERROR_DISPATCH_AMBIGUITY,    // call is ambiguous (fills "funs")
  AERROR_MEMBER,                // no member with that "name"
  AERROR_DESTRUCT,              // unable to destructure expression type
  AERROR_NOTYPE,                // unable to type variable
  AERROR_BOXING,                // primitive missmatch (e.g. integer AND float)
  AERROR_MISMATCH_OFFSETS,      // offset mismatch for field names 
  AERROR_BAD_STATE              // called with bad arguments/data
};

class AError : public gc { public:
  AError_kind kind;
  AVar *call;
  AType *type;
  AVar *avar;
  Vec<FnSymbol *> funs;
  
  void get_member_names(Vec<char *> names);

  AError(AError_kind akind, AVar *acall = NULL, AType *atype = NULL, AVar *avar = NULL);
};

int AST_to_IF1(Vec<AList<Stmt> *> &stmts);  // -1 == error(s)
Type *type_info(BaseAST *a, Symbol *s = 0);     // NULL == error(s)
Type *return_type_info(FnSymbol *fn);           // NULL == error(s)
Type *element_type_info(TypeSymbol *t);         // NULL == error(s)
int function_is_used(FnSymbol *fn);             // -1 == error(s)
int type_is_used(TypeSymbol *fn);               // -1 == error(s)
int AST_is_used(BaseAST *a, Symbol *s = 0);     // -1 == error(s)
int function_returns_void(FnSymbol *fn);        // -1 == error(s)
int constant_info(BaseAST *a, Vec<Expr *> &constants, Symbol *s = 0); // -1 == error(s)
float execution_frequency_info(Expr *);         // -1.0 == error(s)
float execution_frequency_info(FnSymbol *);     // -1.0 == error(s)
int resolve_member_access(Expr *e, int *offset, Type **type);  // -1 == error(s)
int resolve_member(ClassType *t, VarSymbol *v, int *offset, Type **type);  // -1 == error(s)
int structural_subtypes(Type *t, Vec<Type *> subtypes); // -1 == error(s)
int analysis_error(AError_kind akind, AVar *acall = NULL, AType *atype = NULL, AVar *avar = NULL);

enum { CALL_INFO_FIND_SINGLE, 
       CALL_INFO_FIND_ALL,       // for HTML output
       CALL_INFO_FIND_OPERATOR,  //  >  these find disjoint sets
       CALL_INFO_FIND_FUNCTION,  // / 
       CALL_INFO_FIND_ASSIGN,    //  >  these find disjoint sets
       CALL_INFO_FIND_NON_ASSIGN // /  
};
int call_info(Expr *a, Vec<FnSymbol *> &fns, int find_type = CALL_INFO_FIND_SINGLE);

// set by the above calls in case of errors
// must be cleared by the caller after reporting the errors
extern Vec<AError *> analysis_errors;

#endif
