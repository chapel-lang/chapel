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
  virtual ~Expr() { }
  COPY_DEF(Expr);
  virtual void callReplaceChild(BaseAST* new_ast);
  virtual void verify();
  virtual ASTContext getContext(void);
  virtual bool inTree();
  virtual Type* typeInfo(void);

  virtual bool isParam(void);
  virtual bool isConst(void);

  virtual bool isRef(void);
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
          BaseAST* initInit = NULL,
          BaseAST* initExprType = NULL);
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
  virtual bool isRef(void);
};


class CallExpr : public Expr {
 public:
  Expr* baseExpr;
  AList* argList;
  PrimitiveOp* primitive;
  bool partialTag;
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
  CallExpr(BaseAST* base, AList* args);
  CallExpr(PrimitiveOp *prim, AList* args);
  CallExpr(PrimitiveTag prim, AList* args);
  CallExpr(char* name, AList* args);
  ~CallExpr();
  virtual void verify(); 
  COPY_DEF(CallExpr);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);

  void insertAtHead(BaseAST* ast);
  void insertAtTail(BaseAST* ast);
  void insertAtHead(AList* ast);
  void insertAtTail(AList* ast);

  FnSymbol* isResolved(void);
  bool isNamed(char*);

  Expr* get(int index);
  FnSymbol* findFnSymbol(void);
  Type* typeInfo(void);
  bool isPrimitive(PrimitiveTag primitiveTag);
  virtual bool isRef(void);
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

bool get_int(Expr *e, long *i); // false is failure
bool get_string(Expr *e, char **s); // false is failure
VarSymbol *get_constant(Expr *e);

#endif
