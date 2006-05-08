#ifndef _analysis_H_
#define _analysis_H_

#include "chpl.h"
#include "alist.h"
#include "../ifa/ifa.h"
#include "../ifa/num.h"

class Symbol;
class BaseAST;
class Expr;
class SymExpr;
class Type;
class BaseAST;
class FnSymbol;
class Stmt;
class Sym;
class TypeSymbol;
class ClassType;
class VarSymbol;
class AVar;
class AType;

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
  
  void get_member_names(Vec<char *> &names);
  void get_types(Vec<Type *> &types);
  static AType *get_type(AVar *aavar);
  static BaseAST *get_BaseAST(AVar *aavar);      // Symbol or Types
  static BaseAST *get_def_BaseAST(AVar *acall);  // calling or defining Expr or Stmt
  static void get_callers(AVar *acall, Vec<AVar *> &callers);

  AError(AError_kind akind, AVar *acall = NULL, AType *atype = NULL, AVar *avar = NULL);
};

void init_chapel_ifa();
char *cannonicalize_string(char *);
int AST_to_IF1(Vec<BaseAST*>& syms);      // -1 == error(s)
Type *type_info(BaseAST *a, Symbol *s = 0);     // NULL == error(s)
Type *return_type_info(FnSymbol *fn);           // NULL == error(s)
Type *element_type_info(TypeSymbol *t);         // NULL == error(s)
int function_is_used(FnSymbol *fn);             // -1 == error(s)
int type_is_used(TypeSymbol *fn);               // -1 == error(s)
int AST_is_used(BaseAST *a, Symbol *s = 0);     // -1 == error(s)
int constant_info(BaseAST *a, Vec<SymExpr *> &constants, Symbol *s = 0); // -1 == error(s)
float execution_frequency_info(Expr *);         // -1.0 == error(s)
float execution_frequency_info(FnSymbol *);     // -1.0 == error(s)
int resolve_member_access(Expr *e, int *offset, Type **type);  // -1 == error(s)
int resolve_member(ClassType *t, VarSymbol *v, int *offset, Type **type);  // -1 == error(s)
int resolve_member(ClassType *t, int index, int *offset, Type **type);  // -1 == error(s)
int structural_subtypes(Type *t, Vec<Type *> subtypes); // -1 == error(s)
int analysis_error(AError_kind akind, AVar *acall = NULL, AType *atype = NULL, AVar *avar = NULL);
int call_info(Expr *a, Vec<FnSymbol *> &fns, 
              Vec<Vec<Vec<Type *> *> *> *dispatch = 0, Vec<Vec<int> *> *contexts = 0);
int context_info(Expr *a); // calling context for dispatch on visibility

// set by the above calls in case of errors
// must be cleared by the caller after reporting the errors
extern Vec<AError *> analysis_errors;

#endif
