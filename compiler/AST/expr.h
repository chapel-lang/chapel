#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include "link.h"
#include "symbol.h"

class Stmt;

class Expr : public ILink {
 public:
  Stmt* stmt;

  Expr* parent;

  Expr(void);

  virtual void print(FILE* outfile) = 0;
};


class NullExpr : public Expr {
 public:
  bool isNull(void);
  
  void print(FILE* outfile) {}
};


class Literal : public Expr {
 public:
  char* str;

  Literal(char* init_str);

  void print(FILE* outfile);
};


class IntLiteral : public Literal {
 public:
  long val;

  IntLiteral(char* init_str, int init_val);
};


class FloatLiteral : public Literal {
 public:
  double val;

  FloatLiteral(char* init_str, double init_val);
};


class StringLiteral : public Literal {
 public:
  StringLiteral(char* init_val);

  void print(FILE* outfile);
};


class Variable : public Expr {
 public:
  Symbol* var;

  Variable(Symbol* init_var);
  
  void print(FILE* outfile);
};


enum unOpType {
  UNOP_PLUS = 0,
  UNOP_MINUS,
  UNOP_LOGNOT,
  UNOP_BITNOT,

  NUM_UNOPS
};


class UnOp : public Expr {
 public:
  unOpType type;
  Expr* operand;

  UnOp(unOpType init_type, Expr* op);

  void print(FILE* outfile);
};


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
  BINOP_NEQUALS,
  BINOP_BITAND,
  BINOP_BITOR,
  BINOP_BITXOR,
  BINOP_LOGAND,
  BINOP_LOGOR,
  BINOP_EXP,

  BINOP_BY,
  BINOP_DOT,

  BINOP_OTHER,

  NUM_BINOPS
};


class BinOp : public Expr {
 public:
  binOpType type;
  Expr* left;
  Expr* right;

  BinOp(binOpType init_type, Expr* l, Expr* r);

  void print(FILE* outfile);
};


class SpecialBinOp : public BinOp {
 public:
  SpecialBinOp(binOpType init_type, Expr* l, Expr* r);
};


class AssignOp : public BinOp {
 public:

  AssignOp(Expr* l, Expr* r);

  void print(FILE* outfile);
};


class SimpleSeqExpr : public Expr {
 public:
  Expr* lo;
  Expr* hi;
  Expr* str;

  SimpleSeqExpr(Expr* init_lo, Expr* init_hi, Expr* init_str = new NullExpr());

  void print(FILE* outfile);
};


class FloodExpr : public Expr {
 public:
  void print(FILE* outfile);
};


class CompleteDimExpr : public Expr {
 public:
  void print(FILE* outfile);
};


class DomainExpr : public Expr {
 public:
  Expr* domains;
  VarSymbol* indices;
  Expr* forallExpr;

  DomainExpr(Expr* init_domains, VarSymbol* init_indices = new NullVarSymbol());

  void setForallExpr(Expr* exp);

  void print(FILE* outfile);
};


class ParenOpExpr : public Expr {
 public:
  Expr* baseExpr;
  Expr* argList;

  static ParenOpExpr* classify(Expr* base, Expr* arg);

  ParenOpExpr(Expr* init_base, Expr* init_arg = new NullExpr());

  virtual void print(FILE* outfile);
};


class CastExpr : public ParenOpExpr {
 public:
  Type* castType;

  CastExpr(Type* init_castType, Expr* init_argList);

  void print(FILE* outfile);
};


class FnCall : public ParenOpExpr {
 public:
  FnCall(Expr* init_base, Expr* init_arg = new NullExpr());
};


class ArrayRef : public ParenOpExpr {
 public:
  ArrayRef(Expr* init_base, Expr* init_arg = new NullExpr());
};


class ReduceExpr : public Expr {
 public:
  Symbol* reduceType;
  Expr* redDim;
  Expr* argExpr;

  ReduceExpr(Symbol* init_reduceType, Expr* init_redDim, Expr* init_argExpr);

  void print(FILE* outfile);
};


extern DomainExpr* unknownDomain;

void initExpr(void);

#endif
