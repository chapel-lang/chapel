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
  PREC_EXP,
  PREC_HIGHEST
};

enum ioCallType {
  IO_WRITE = 0, 
  IO_WRITELN, 
  IO_READ
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


class Expr : public BaseAST {
 public:
  Stmt* stmt;
  AInfo *ainfo;

  Expr* parent;

  Expr(astType_t astType);
  virtual Expr* copy(void);
  Expr* copyList(void);

  bool isNull(void);

  void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseExpr(Traversal* traversal);

  virtual Type* typeInfo(void);
  virtual bool isComputable(void);
  virtual long intVal(void);
  virtual int rank(void);
  virtual precedenceType precedence(void);

  virtual void codegenComplex(FILE* outfile, bool real);

  static Expr* newPlusMinus(binOpType op, Expr* l, Expr* r);
};
#define forv_Expr(_p, _v) forv_Vec(Expr, _p, _v)


extern Expr* nilExpr;


class Literal : public Expr {
 public:
  char* str;

  Literal(astType_t astType, char* init_str);
  virtual Expr* copy(void);

  bool isComputable(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class BoolLiteral : public Literal {
 public:
  bool val;

  BoolLiteral(char* init_str, bool init_val);
  virtual Expr* copy(void);

  bool boolVal(void);
  
  Type* typeInfo(void);
};


class IntLiteral : public Literal {
 public:
  long val;

  IntLiteral(char* init_str, int init_val);
  virtual Expr* copy(void);

  long intVal(void);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
};


class FloatLiteral : public Literal {
 public:
  double val;

  FloatLiteral(char* init_str, double init_val);
  virtual Expr* copy(void);
};


class ComplexLiteral : public Literal {
 public:
  double realVal;
  double imagVal;
  char* realStr;

  ComplexLiteral(char* init_str, double init_imag, double init_real = 0.0,
		 char* init_realStr = "");
  void addReal(FloatLiteral* init_real);
  virtual Expr* copy(void);

  void negateImag(void);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
  void codegenComplex(FILE* outfile, bool real);
};


class StringLiteral : public Literal {
 public:
  StringLiteral(char* init_val);
  virtual Expr* copy(void);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class Variable : public Expr {
 public:
  Symbol* var;

  Variable(Symbol* init_var);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);
  
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class UnOp : public Expr {
 public:
  unOpType type;
  Expr* operand;

  UnOp(unOpType init_type, Expr* op);
  virtual Expr* copy(void);

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
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
};


class AssignOp : public BinOp {
 public:
  getsOpType type;

  AssignOp(getsOpType init_type, Expr* l, Expr* r);
  virtual Expr* copy(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  precedenceType precedence(void);
};


class SpecialBinOp : public BinOp {
 public:
  SpecialBinOp(binOpType init_type, Expr* l, Expr* r);
  virtual Expr* copy(void);

  precedenceType precedence(void);
};


class MemberAccess : public Expr {
 public:
  Expr* base;
  Symbol* member;

  MemberAccess(Expr* init_base, Symbol* init_member);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ParenOpExpr : public Expr {
 public:
  Expr* baseExpr;
  Expr* argList;

  ParenOpExpr(Expr* init_base, Expr* init_arg = nilExpr);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);

  static ParenOpExpr* classify(Expr* base, Expr* arg);
};


class ArrayRef : public ParenOpExpr {
 public:
  ArrayRef(Expr* init_base, Expr* init_arg = nilExpr);
  virtual Expr* copy(void);

  Type* typeInfo();

  void codegen(FILE* outfile);
};


class FnCall : public ParenOpExpr {
 public:
  FnCall(Expr* init_base, Expr* init_arg = nilExpr);
  virtual Expr* copy(void);
};


class IOCall : public FnCall {
 public:
  ioCallType ioType;

  IOCall(ioCallType init_iotype, Expr* init_base, Expr* init_arg);
  virtual Expr* copy(void);

  Type* typeInfo(void);

  void codegen(FILE* outfile);
};


class Tuple : public Expr {
 public:
  Expr* exprs;

  Tuple(Expr* init_exprs);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class SizeofExpr : public Expr {
 public:
  Type* type;

  SizeofExpr(Type* init_type);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);
  
  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CastExpr : public ParenOpExpr {
 public:
  Type* castType;

  CastExpr(Type* init_castType, Expr* init_argList);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ReduceExpr : public Expr {
 public:
  Symbol* reduceType;
  Expr* redDim;
  Expr* argExpr;

  ReduceExpr(Symbol* init_reduceType, Expr* init_redDim, Expr* init_argExpr);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class SimpleSeqExpr : public Expr {
 public:
  Expr* lo;
  Expr* hi;
  Expr* str;

  SimpleSeqExpr(Expr* init_lo, Expr* init_hi, 
                Expr* init_str = new IntLiteral("1", 1));
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class FloodExpr : public Expr {
 public:
  FloodExpr(void);
  virtual Expr* copy(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CompleteDimExpr : public Expr {
 public:
  CompleteDimExpr(void);
  virtual Expr* copy(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class DomainExpr : public Expr {
 public:
  Expr* domains;
  Symbol* indices;
  Expr* forallExpr;

  DomainExpr(Expr* init_domains, Symbol* init_indices = nilVarSymbol,
	     Expr* init_forallExpr = nilExpr);
  void setForallExpr(Expr* exp);
  virtual Expr* copy(void);

  void traverseExpr(Traversal* traversal);

  Type* typeInfo(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


extern DomainExpr* unknownDomain;

void initExpr(void);

#endif
