#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include "baseAST.h"
#include "symbol.h"

class Stmt;
class AInfo;

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
  PREC_HIGHEST
};


class Expr : public BaseAST {
 public:
  Stmt* stmt;
  AInfo *ainfo;

  Expr* parent;

  Expr(astType_t astType);

  bool isNull(void);

  void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseExpr(Traversal* traversal);

  virtual Type* typeInfo(void);
  virtual bool isComputable(void);
  virtual long intVal(void);
  virtual int rank(void);
  virtual precedenceType precedence(void);

  int getTypes(Vec<BaseAST *> &asts);
};
#define forv_Expr(_p, _v) forv_Vec(Expr, _p, _v)


extern Expr* nilExpr;


class Literal : public Expr {
 public:
  char* str;

  Literal(astType_t astType, char* init_str);

  bool isComputable(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class IntLiteral : public Literal {
 public:
  long val;

  IntLiteral(char* init_str, int init_val);

  long intVal(void);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
};


class FloatLiteral : public Literal {
 public:
  double val;

  FloatLiteral(char* init_str, double init_val);
};


class StringLiteral : public Literal {
 public:
  StringLiteral(char* init_val);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class Variable : public Expr {
 public:
  Symbol* var;

  Variable(Symbol* init_var);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);
  
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
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


class UnOp : public Expr {
 public:
  unOpType type;
  Expr* operand;

  UnOp(unOpType init_type, Expr* op);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
  int getExprs(Vec<BaseAST *> &asts);
};

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
  BINOP_BITSL,
  BINOP_BITSR,
  BINOP_LOGAND,
  BINOP_LOGOR,
  BINOP_EXP,

  BINOP_BY,

  BINOP_OTHER,

  NUM_BINOPS
};
/************* IF CHANGING THIS, change cBinOp as well... *****************/


class BinOp : public Expr {
 public:
  binOpType type;
  Expr* left;
  Expr* right;

  BinOp(binOpType init_type, Expr* l, Expr* r);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
  int getExprs(Vec<BaseAST *> &asts);
};


class MemberAccess : public Expr {
 public:
  Expr* base;
  Symbol* member;

  MemberAccess(Expr* init_base, Symbol* init_member);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
  int getSymbols(Vec<BaseAST *> &asts);
};


class SpecialBinOp : public BinOp {
 public:
  SpecialBinOp(binOpType init_type, Expr* l, Expr* r);

  precedenceType precedence(void);
};


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
  GETS_BITSL,
  GETS_BITSR,

  NUM_GETS_OPS
};
/************* IF CHANGING THIS, change cGetsOp as well... *****************/

class AssignOp : public BinOp {
 public:
  getsOpType type;

  AssignOp(getsOpType init_type, Expr* l, Expr* r);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
};


class SimpleSeqExpr : public Expr {
 public:
  Expr* lo;
  Expr* hi;
  Expr* str;

  SimpleSeqExpr(Expr* init_lo, Expr* init_hi, 
                Expr* init_str = new IntLiteral("1", 1));

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
};


class FloodExpr : public Expr {
 public:
  FloodExpr(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CompleteDimExpr : public Expr {
 public:
  CompleteDimExpr(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class DomainExpr : public Expr {
 public:
  Expr* domains;
  VarSymbol* indices;
  Expr* forallExpr;

  DomainExpr(Expr* init_domains, VarSymbol* init_indices = nilVarSymbol);
  void setForallExpr(Expr* exp);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
  int getSymbols(Vec<BaseAST *> &asts);
};


class SizeofExpr : public Expr {
 public:
  Type* type;

  SizeofExpr(Type* init_type);

  void traverseExpr(Traversal* traversal);
  
  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ParenOpExpr : public Expr {
 public:
  Expr* baseExpr;
  Expr* argList;

  static ParenOpExpr* classify(Expr* base, Expr* arg);

  ParenOpExpr(Expr* init_base, Expr* init_arg = nilExpr);

  void traverseExpr(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
};


class CastExpr : public ParenOpExpr {
 public:
  Type* castType;

  CastExpr(Type* init_castType, Expr* init_argList);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getTypes(Vec<BaseAST *> &asts);
};


class FnCall : public ParenOpExpr {
 public:
  FnCall(Expr* init_base, Expr* init_arg = nilExpr);
};


class WriteCall : public FnCall {
 public:
  bool writeln;

  WriteCall(bool init_writeln, Expr* init_base, Expr* init_arg);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
};


class ArrayRef : public ParenOpExpr {
 public:
  ArrayRef(Expr* init_base, Expr* init_arg = nilExpr);

  Type* typeInfo();

  void codegen(FILE* outfile);
};


class ReduceExpr : public Expr {
 public:
  Symbol* reduceType;
  Expr* redDim;
  Expr* argExpr;

  ReduceExpr(Symbol* init_reduceType, Expr* init_redDim, Expr* init_argExpr);

  void traverseExpr(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
  int getExprs(Vec<BaseAST *> &asts);
};


class Tuple : public Expr {
 public:
  Expr* exprs;

  Tuple(Expr* init_exprs);

  void traverseExpr(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
};


extern DomainExpr* unknownDomain;

void initExpr(void);

#endif
