#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include "alist.h"
#include "analysis.h"
#include "baseAST.h"
#include "symbol.h"

class Stmt;
class AInfo;
class FnSymbol;

enum OpTag {
  OP_NONE,
  OP_UNPLUS,
  OP_UNMINUS,
  OP_LOGNOT,
  OP_BITNOT,
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
  OP_MOD,
  OP_EQUAL,
  OP_NEQUAL,
  OP_LEQUAL,
  OP_GEQUAL,
  OP_LTHAN,
  OP_GTHAN,
  OP_BITAND,
  OP_BITOR,
  OP_BITXOR,
  OP_LOGAND,
  OP_LOGOR,
  OP_EXP,
  OP_SEQCAT,
  OP_BY,
  OP_SUBTYPE,
  OP_NOTSUBTYPE,
  OP_GETSNORM,
  OP_GETSPLUS,
  OP_GETSMINUS,
  OP_GETSMULT,
  OP_GETSDIV,
  OP_GETSBITAND,
  OP_GETSBITOR,
  OP_GETSBITXOR,
  OP_GETSSEQCAT
};

#define OP_ISASSIGNOP(op) (op >= OP_GETSNORM)
#define OP_ISBINARYOP(op) (op >= OP_PLUS && op <= OP_NOTSUBTYPE)
#define OP_ISUNARYOP(op) (op >= OP_UNPLUS && op <= OP_BITNOT)

#define OP_ISLOGICAL(op) \
  ((op == OP_LOGNOT) ||  \
   (op == OP_EQUAL)  ||  \
   (op == OP_NEQUAL) ||  \
   (op == OP_LEQUAL) ||  \
   (op == OP_GEQUAL) ||  \
   (op == OP_LTHAN)  ||  \
   (op == OP_GTHAN)  ||  \
   (op == OP_LOGAND) ||  \
   (op == OP_LOGOR))

class Expr : public BaseAST {
 public:
  Symbol* parentSymbol;
  Stmt* parentStmt;
  Expr* parentExpr;
  AInfo *ainfo;

  Expr(astType_t astType = EXPR);
  COPY_DEF(Expr);
  virtual void callReplaceChild(BaseAST* new_ast);
  virtual void verify(void); 
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseExpr(Traversal* traversal);

  virtual Type* typeInfo(void);
  
  virtual bool isParam(void);
  virtual bool isConst(void);
  virtual int rank(void);

  virtual void printCfgInitString(FILE* outfile);
  FnSymbol *parentFunction();

  bool isRead(void);
  bool isWritten(void);
  bool isRef(void);
  Stmt* Expr::getStmt();

  void codegenCastToString(FILE* outfile);
};
#define forv_Expr(_p, _v) forv_Vec(Expr, _p, _v)


class Literal : public Expr {
 public:
  char* str;

  Literal(astType_t astType, char* init_str);
  virtual void verify(void); 
  COPY_DEF(Literal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);
};


class BoolLiteral : public Literal {
 public:
  bool val;

  BoolLiteral(char* initStr);
  BoolLiteral(bool initVal);
  virtual void verify(void); 
  COPY_DEF(BoolLiteral);
  bool boolVal(void);
  
  Type* typeInfo(void);
};


class IntLiteral : public Literal {
 public:
  long val;

  IntLiteral(char* initStr);
  IntLiteral(int initVal);
  virtual void verify(void); 
  COPY_DEF(IntLiteral);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
};


class FloatLiteral : public Literal {
 public:
  double val;

  FloatLiteral(char* init_str, double init_val);
  virtual void verify(void); 
  COPY_DEF(FloatLiteral);
  virtual Type* typeInfo(void);
};


class StringLiteral : public Literal {
 public:
  StringLiteral(char* init_val);
  virtual void verify(void); 
  COPY_DEF(StringLiteral);
  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void printCfgInitString(FILE* outfile);
};


class DefExpr : public Expr {
 public:
  Symbol* sym;
  Expr* init;
  Expr* exprType;
  Vec<FnSymbol *> initAssign;

  DefExpr(Symbol* initSym = NULL,
          Expr* initInit = NULL,
          Expr* initExprType = NULL);
  virtual void verify(void); 
  COPY_DEF(DefExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  bool noCodegen() { return true; }
  void codegen(FILE* outfile);
};


class Variable : public Expr {
 public:
  Symbol* var;
  Variable(Symbol* init_var);
  COPY_DEF(Variable);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void verify(void); 
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);
  virtual bool isConst(void);
  virtual bool isParam(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class MemberAccess : public Expr {
 public:
  Expr* base;
  Symbol* member;
  Type* member_type;
  int member_offset;

  MemberAccess(Expr* init_base, Symbol* init_member);
  virtual void verify(void); 
  COPY_DEF(MemberAccess);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  bool isConst(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CallExpr : public Expr {
 public:
  Expr* baseExpr;
  AList<Expr>* argList;
  OpTag opTag;

  CallExpr(Expr* initBase, AList<Expr>* initArgs = new AList<Expr>);
  CallExpr(OpTag initOpTag, Expr* arg1, Expr* arg2 = NULL);
  virtual void verify(void); 
  COPY_DEF(CallExpr);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);

  Expr* CallExpr::get(int index);
  FnSymbol* findFnSymbol(void);
  Type* typeInfo(void);
  bool isPrimitive(void);
};


class CastExpr : public Expr {
 public:
  Expr* expr;
  Expr* newType;
  Type* type;

  CastExpr(Expr* initExpr, Expr* initNewType, Type* initType = dtUnknown);
  virtual void verify(void); 
  COPY_DEF(CastExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ReduceExpr : public Expr {
 public:
  Symbol* reduceType;
  AList<Expr>* redDim;
  Expr* argExpr;
  bool isScan;

  ReduceExpr(Symbol* init_reduceType, Expr* init_argExpr, 
             bool init_isScan, AList<Expr>* init_redDim = new AList<Expr>());
  virtual void verify(void); 
  COPY_DEF(ReduceExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ForallExpr : public Expr {
 public:
  AList<DefExpr>* indices;
  AList<Expr>* iterators;
  Expr* innerExpr;
  SymScope* indexScope;

  ForallExpr(AList<DefExpr>* initIndices,
             AList<Expr>* initIterators,
             Expr* initInnerExpr = NULL,
             SymScope* initIndexScope = NULL);
  virtual void verify(void); 
  COPY_DEF(ForallExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  Type* typeInfo(void);
};

void initExpr(void);

class LetExpr : public Expr {
 public:
  AList<DefExpr>* symDefs;
  Expr* innerExpr;

  SymScope* letScope;

  LetExpr(AList<DefExpr>* init_symDefs = new AList<DefExpr>(), 
          Expr* init_innerExpr = NULL);
  virtual void verify(void); 
  COPY_DEF(LetExpr);
  void setInnerExpr(Expr* expr);
  void setSymDefs(AList<Stmt>* stmts);
  void setLetScope(SymScope* init_letScope);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CondExpr : public Expr {
 public:
  Expr* condExpr;
  Expr* thenExpr;
  Expr* elseExpr;

  CondExpr(Expr* initCondExpr, Expr* initThenExpr, Expr* initElseExpr = NULL);
  virtual void verify(void); 
  COPY_DEF(CondExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class NamedExpr : public Expr {
 public:
  char* name;
  Expr* actual;
  NamedExpr(char* init_name, Expr* init_actual);
  virtual void verify(void); 
  COPY_DEF(NamedExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class VarInitExpr : public Expr {
 public:
  Expr* expr;
  VarInitExpr(Expr* init_expr);
  virtual void verify(void); 
  COPY_DEF(VarInitExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class UseExpr : public Expr {
 public:
  Expr* expr;
  UseExpr(Expr* init_expr);
  virtual void verify(void); 
  COPY_DEF(UseExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  ModuleSymbol* getModule(void);
};


class WithExpr : public Expr {
 public:
  Expr* expr;
  WithExpr(Expr* init_expr);
  virtual void verify(void); 
  COPY_DEF(WithExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  StructuralType* getStruct(void);
};


#endif
