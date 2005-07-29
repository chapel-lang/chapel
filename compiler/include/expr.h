#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include "alist.h"
#include "analysis.h"
#include "baseAST.h"
#include "symbol.h"

class Stmt;
class AInfo;
class UserInitExpr;

extern char* cUnOp[];
extern char* cBinOp[];
extern char* cGetsOp[];

enum precedenceType {
  PREC_LOWEST = 0,
  PREC_LOGOR,
  PREC_LOGAND,
  PREC_BITOR,
  PREC_BITXOR,
  PREC_BITAND,
  PREC_EQUALITY,
  PREC_COMPARE,
  PREC_BITS,
  PREC_PLUSMINUS,
  PREC_MULTDIV,
  PREC_UNOP, 
  PREC_EXP,
  PREC_HIGHEST
};


/************* IF CHANGING THIS, change cUnOp as well... *****************/
enum unOpType {
  UNOP_PLUS = 0,
  UNOP_MINUS,
  UNOP_LOGNOT,
  UNOP_BITNOT,

  NUM_UNOPS
};
/************* IF CHANGING THIS, change cUnOp as well... *****************/


/************* IF CHANGING THIS, change cBinOp as well... *****************/
enum binOpType {
  BINOP_PLUS = 0,
  BINOP_MINUS,
  BINOP_MULT,
  BINOP_DIV,
  BINOP_MOD,
  BINOP_EQUAL,
  BINOP_LEQUAL,
  BINOP_GEQUAL,
  BINOP_GTHAN,
  BINOP_LTHAN,
  BINOP_NEQUAL,
  BINOP_BITAND,
  BINOP_BITOR,
  BINOP_BITXOR,
  BINOP_LOGAND,
  BINOP_LOGOR,
  BINOP_EXP,

  BINOP_SEQCAT,
  BINOP_BY,

  BINOP_SUBTYPE,
  BINOP_NOTSUBTYPE,

  BINOP_OTHER,
  
  NUM_BINOPS
};
/************* IF CHANGING THIS, change cBinOp as well... *****************/


/************* IF CHANGING THIS, change cGetsOp as well... *****************/
enum getsOpType {
  GETS_NORM = 0,
  GETS_PLUS,
  GETS_MINUS,
  GETS_MULT,
  GETS_DIV,
  GETS_BITAND,
  GETS_BITOR,
  GETS_BITXOR,
  GETS_SEQCAT,

  NUM_GETS_OPS
};
/************* IF CHANGING THIS, change cGetsOp as well... *****************/


class Expr : public BaseAST {
 public:
  Symbol* parentSymbol;
  Stmt* parentStmt;
  Expr* parentExpr;
  AInfo *ainfo;
  FnSymbol *resolved;

  Expr(astType_t astType = EXPR);
  COPY_DEF(Expr);
  virtual void callReplaceChild(BaseAST* new_ast);
  virtual void verify(void); 
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseExpr(Traversal* traversal);

  virtual Type* typeInfo(void);
  virtual bool isComputable(void);
  
  virtual bool isParam(void);
  
  virtual bool isConst(void);
  virtual long intVal(void);
  virtual int rank(void);
  virtual precedenceType precedence(void);

  virtual void printCfgInitString(FILE* outfile);

  static Expr* newPlusMinus(binOpType op, Expr* l, Expr* r);

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
  COPY_DEF(Literal);
  bool isComputable(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class BoolLiteral : public Literal {
 public:
  bool val;

  BoolLiteral(char* init_str, bool init_val);
  COPY_DEF(BoolLiteral);
  bool boolVal(void);
  
  Type* typeInfo(void);
};


class IntLiteral : public Literal {
 public:
  long val;

  IntLiteral(char* init_str, int init_val);
  COPY_DEF(IntLiteral);
  long intVal(void);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
};


class FloatLiteral : public Literal {
 public:
  double val;

  FloatLiteral(char* init_str, double init_val);
  COPY_DEF(FloatLiteral);
  virtual Type* typeInfo(void);
};


class ComplexLiteral : public Literal {
 public:
  double realVal;
  double imagVal;
  char* realStr;

  ComplexLiteral(char* init_str, double init_imag, double init_real = 0.0,
                 char* init_realStr = "");
  COPY_DEF(ComplexLiteral);
  void addReal(FloatLiteral* init_real);

  void negateImag(void);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class StringLiteral : public Literal {
 public:
  StringLiteral(char* init_val);
  COPY_DEF(StringLiteral);
  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void printCfgInitString(FILE* outfile);
};


class UnOp : public Expr {
 public:
  unOpType type;
  Expr* operand;

  UnOp(unOpType init_type, Expr* op);
  COPY_DEF(UnOp);
  bool isComputable(void);
  long intVal(void);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
};


class BinOp : public Expr {
 public:
  binOpType type;
  Expr* left;
  Expr* right;

  BinOp(binOpType init_type, Expr* l, Expr* r);
  COPY_DEF(BinOp);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);
  bool isComputable(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
};


class AssignOp : public BinOp {
 public:
  getsOpType type;

  AssignOp(getsOpType init_type, Expr* l, Expr* r);
  COPY_DEF(AssignOp);
  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
};


class DefExpr : public Expr {
 public:
  Symbol* sym;
  UserInitExpr* init;
  Expr* exprType;

  DefExpr(Symbol* initSym = NULL,
          UserInitExpr* initInit = NULL,
          Expr* initExprType = NULL);
  COPY_DEF(DefExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void verify(void); 
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class Variable : public Expr {
 public:
  Symbol* var;
  ForwardingSymbol* forward; // was this include by a use statement?
                             // if so, it might be renamed.
  Variable(Symbol* init_var, ForwardingSymbol* init_forward = NULL);
  COPY_DEF(Variable);
  virtual void verify(void); 
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);
  virtual bool isConst(void);
  virtual bool isParam(void);
  virtual bool isComputable();
  virtual long intVal(void);
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
  COPY_DEF(MemberAccess);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  bool isConst(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class Tuple : public Expr {
 public:
  AList<Expr>* exprs;

  Tuple(AList<Expr>* init_exprs);
  COPY_DEF(Tuple);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ParenOpExpr : public Expr {
 public:
  Expr* baseExpr;
  AList<Expr>* argList;

  ParenOpExpr(Expr* init_base, AList<Expr>* init_arg = new AList<Expr>);
  COPY_DEF(ParenOpExpr);
  virtual void verify(void); 
  void setArgs(AList<Expr>* init_arg);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);
};


class FnCall : public ParenOpExpr {
 public:
  FnCall(Expr* init_base, AList<Expr>* init_arg = new AList<Expr>());
  COPY_DEF(FnCall);
  Type* typeInfo(void);

  FnSymbol* findFnSymbol(void);

  void codegen(FILE* outfile);
};


class SizeofExpr : public Expr {
 public:
  Variable* variable;

  SizeofExpr(Variable* init_variable);
  COPY_DEF(SizeofExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  
  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CastExpr : public Expr {
 public:
  Expr* expr;
  Expr* newType;
  Type* type;

  CastExpr(Expr* initExpr, Expr* initNewType, Type* initType);
  COPY_DEF(CastExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CastLikeExpr : public Expr {
 public:
  Variable* variable; // cast to type of this variable
  Expr* expr;

  CastLikeExpr(Variable* init_variable, Expr* init_expr);
  COPY_DEF(CastLikeExpr);
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
  COPY_DEF(ReduceExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class SeqExpr : public Expr {
 public:
  AList<Expr>* exprls;

  SeqExpr(AList<Expr>* init_exprls);
  COPY_DEF(SeqExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
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
  COPY_DEF(VarInitExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class UserInitExpr : public Expr {
 public:
  Expr* expr;
  UserInitExpr(Expr* init_expr);
  COPY_DEF(UserInitExpr);
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
  COPY_DEF(WithExpr);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseExpr(Traversal* traversal);
  Type* typeInfo(void);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  StructuralType* getStruct(void);
};


#endif
