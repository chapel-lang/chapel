#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "primitive.h"

class Stmt;
class FnSymbol;

#define IS_EXPR(e)                                          \
  ((e)->astType == EXPR_CALL || (e)->astType == EXPR_SYM || \
   (e)->astType == EXPR_DEF || (e)->astType == EXPR_NAMED)

#define IS_STMT(e)                                              \
  ((e)->astType == STMT_BLOCK ||                                \
   (e)->astType == STMT_COND || (e)->astType == STMT_GOTO)

class Expr : public BaseAST {
 public:
  Expr* prev;       // alist previous pointer
  Expr* next;       // alist next pointer
  AList* list;      // alist pointer
  Expr* parentExpr;
  Symbol* parentSymbol;

  Expr(astType_t astType = EXPR);
  virtual ~Expr() { }
  COPY_DEF(Expr);
  virtual void callReplaceChild(Expr* new_ast);
  virtual void verify();
  virtual ASTContext getContext(void);
  virtual bool inTree();
  virtual Type* typeInfo(void);

  virtual bool isParam(void);
  virtual bool isConst(void);

  Expr* getStmtExpr();

  virtual void replaceChild(Expr* old_ast, Expr* new_ast);

  Expr* remove(void);
  void replace(Expr* new_ast);
  virtual void insertBefore(Expr* new_ast);
  virtual void insertAfter(Expr* new_ast);
};


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
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
};


class SymExpr : public Expr {
 public:
  Symbol* var;
  SymExpr(Symbol* init_var);
  SymExpr(const char* init_var);
  COPY_DEF(SymExpr);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void verify(); 

  Type* typeInfo(void);
  virtual bool isConst(void);
  virtual bool isParam(void);
  void codegen(FILE* outfile);
};


class CallExpr : public Expr {
 public:
  Expr* baseExpr;         // function expression
  AList argList;          // function actuals
  PrimitiveOp* primitive; // primitive expression (baseExpr == NULL)
  bool partialTag;
  bool methodTag;
  bool square; // true if call made with square brackets

  CallExpr(BaseAST* base, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(PrimitiveOp *prim, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(PrimitiveTag prim, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(const char* name, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(BaseAST* base, AList* args);
  CallExpr(PrimitiveOp *prim, AList* args);
  CallExpr(PrimitiveTag prim, AList* args);
  CallExpr(const char* name, AList* args);
  ~CallExpr();
  virtual void verify(); 
  COPY_DEF(CallExpr);

  virtual void replaceChild(Expr* old_ast, Expr* new_ast);

  virtual void codegen(FILE* outfile);

  void insertAtHead(BaseAST* ast);
  void insertAtTail(BaseAST* ast);
  void insertAtHead(AList* ast);
  void insertAtTail(AList* ast);

  FnSymbol* isResolved(void);
  bool isNamed(const char*);

  int numActuals();
  Expr* get(int index);
  FnSymbol* findFnSymbol(void);
  Type* typeInfo(void);
  bool isPrimitive(PrimitiveTag primitiveTag);
};


class NamedExpr : public Expr {
 public:
  const char* name;
  Expr* actual;
  NamedExpr(const char* init_name, Expr* init_actual);
  virtual void verify(); 
  COPY_DEF(NamedExpr);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  Type* typeInfo(void);
  void codegen(FILE* outfile);
};

bool get_int(Expr *e, long *i); // false is failure
bool get_string(Expr *e, const char **s); // false is failure
const char* get_string(Expr* e); // fatal on failure
VarSymbol *get_constant(Expr *e);

#define for_exprs_postorder(e, expr)                            \
  for (Expr* e = getFirstExpr(expr); e; e = getNextExpr(e))

Expr* getFirstExpr(Expr* expr);
Expr* getNextExpr(Expr* expr);

#endif
