#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include "link.h"
#include "symbol.h"

class Stmt;

class Expr : public Link {
 public:
  Stmt* stmt;

  Expr* parent;

  Expr(void);

  virtual void print(FILE* outfile) = 0;
  void printList(FILE* outfile);
};


class NullExpr : public Expr {
 public:
  bool isNull(void);
  
  void print(FILE* outfile) {}
};


class Literal : public Expr {
 public:
};


class IntLiteral : public Literal {
 public:
  long val;

  IntLiteral(int init_val);

  void print(FILE* outfile);
};


class FloatLiteral : public Literal {
 public:
  double val;

  FloatLiteral(double init_val);

  void print(FILE* outfile);
};


class StringLiteral : public Literal {
 public:
  char* val;

  StringLiteral(char* init_val);

  void print(FILE* outfile);
};


class Variable : public Expr {
 public:
  Symbol* var;

  Variable(Symbol* init_var);
  
  void print(FILE* outfile);
};


class UnOp : public Expr {
 public:
  Expr* operand;

  UnOp(Expr* op);

  void print(FILE* outfile);
};


enum binOpType {
  BINOP_PLUS,
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
  BINOP_OTHER
};


class BinOp : public Expr {
 public:
  binOpType type;
  Expr* left;
  Expr* right;

  BinOp(binOpType init_type, Expr* l, Expr* r);

  void print(FILE* outfile);
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

  SimpleSeqExpr(Expr* init_lo, Expr* init_hi, Expr* init_str = NULL);

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
  Expr* indices;
  Expr* forallExpr;

  DomainExpr(Expr* init_domains, Expr* init_indices = NULL);

  void setForallExpr(Expr* exp);

  void print(FILE* outfile);
};


/*
class FunCall : public ParenOpExpr {
 public:
};


class ArrayRef : public ParenOpExpr {
 public:
};
*/


class ParenOpExpr : public Expr {
 public:
  Expr* baseExpr;
  Expr* argList;

  ParenOpExpr(Expr* init_base, Expr* init_arg = NULL);

  void print(FILE* outfile);
};


class CastExpr : public ParenOpExpr {
 public:
  Type* castType;

  CastExpr(Type* init_castType, Expr* init_argList);
};

#endif
