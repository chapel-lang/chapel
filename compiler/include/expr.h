#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "primitive.h"

class Stmt;
class FnSymbol;

class Expr : public BaseAST {
 public:
  Stmt* parentStmt;
  Expr* parentExpr;

  Expr(astType_t astType = EXPR);
  COPY_DEF(Expr);
  virtual void callReplaceChild(BaseAST* new_ast);
  virtual void verify();
  virtual ASTContext getContext(void);
  virtual Type* typeInfo(void);

  virtual bool isParam(void);
  virtual bool isConst(void);

  bool isRef(void);
  Stmt* Expr::getStmt();

  void codegenCastToString(FILE* outfile);
};
#define forv_Expr(_p, _v) forv_Vec(Expr, _p, _v)

class DefExpr : public Expr {
 public:
  Symbol* sym;
  Expr* init;
  Expr* exprType;

  DefExpr(Symbol* initSym = NULL,
          Expr* initInit = NULL,
          Expr* initExprType = NULL);
  virtual void verify(); 
  COPY_DEF(DefExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class SymExpr : public Expr {
 public:
  Symbol* var;
  SymExpr(Symbol* init_var);
  SymExpr(char* init_var);
  COPY_DEF(SymExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void verify(); 

  Type* typeInfo(void);
  virtual bool isConst(void);
  virtual bool isParam(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CallExpr : public Expr {
 public:
  Expr* baseExpr;
  AList<Expr>* argList;
  PrimitiveOp* primitive;
  PartialTag partialTag;
  bool methodTag;
  bool square; // true if call made with square brackets

  CallExpr(BaseAST* base, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(PrimitiveOp *prim, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL);
  CallExpr(PrimitiveTag prim, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL);
  CallExpr(char* name, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  virtual void verify(); 
  COPY_DEF(CallExpr);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);

  void insertAtHead(BaseAST* ast);
  void insertAtTail(BaseAST* ast);

  bool isAssign(void);

  FnSymbol* isResolved(void);
  bool isNamed(char*);

  Expr* get(int index);
  FnSymbol* findFnSymbol(void);
  Type* typeInfo(void);
  bool isPrimitive(PrimitiveTag primitiveTag);
};


class NamedExpr : public Expr {
 public:
  char* name;
  Expr* actual;
  NamedExpr(char* init_name, Expr* init_actual);
  virtual void verify(); 
  COPY_DEF(NamedExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


Expr *new_IntLiteral(char *, IF1_int_type int_size=INT_SIZE_64);
Expr *new_IntLiteral(long long int i, IF1_int_type int_size=INT_SIZE_64);
Expr *new_UIntLiteral(char *ui_str, IF1_int_type uint_size=INT_SIZE_64);
Expr *new_UIntLiteral(unsigned long long u, IF1_int_type uint_size=INT_SIZE_64);
Expr *new_FloatLiteral(char *n, long double d, IF1_float_type float_size=FLOAT_SIZE_64);
Expr *new_ComplexLiteral(long double i,IF1_float_type comp_size=FLOAT_SIZE_64);
Expr *new_StringLiteral(char *str);
bool get_int(Expr *e, long *i); // false is failure
bool get_string(Expr *e, char **s); // false is failure
VarSymbol *get_constant(Expr *e);
AList<DefExpr>* exprsToIndices(AList<Expr>* indices);

#endif
