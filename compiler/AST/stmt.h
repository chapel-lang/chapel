#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "link.h"
#include "symbol.h"
class Expr;

class Stmt : public Link {
 public:
};


class NullStmt : public Stmt {
 public:

  bool isNull(void);
  
  void print(FILE* outfile) { fprintf(outfile, "Null Statement\n"); }
};


class VarDefStmt : public Stmt {
 public:
  VarSymbol* var;
  Expr* init;

  VarDefStmt(VarSymbol* init_var, Expr* init_expr);

  void print(FILE* outfile);
};


class TypeDefStmt : public Stmt {
 public:
  Type* type;

  TypeDefStmt(Type* init_type);

  void print(FILE* outfile);
};


class FnDefStmt : public Stmt {
 public:
  FunSymbol* fun;

  FnDefStmt(FunSymbol* init_fun);

  void print(FILE* outfile);
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);

  void print(FILE* outfile);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* retExpr);

  void print(FILE* outfile);
};


class BlockStmt : public Stmt {
 public:
  Stmt* body;

  BlockStmt::BlockStmt(Stmt* init_body);

  void print(FILE* outfile);
};


enum whileLoopType {
  LOOP_WHILEDO = 0,
  LOOP_DOWHILE,
  LOOP_REPEAT
};


class WhileLoopStmt : public BlockStmt {
 public:
  whileLoopType type;
  Expr* condition;

  WhileLoopStmt(whileLoopType init_type, Expr* init_cond, Stmt* body);

  void print(FILE* outfile);
};


class ForLoopStmt : public BlockStmt {
 public:
  bool forall;
  VarSymbol* index;
  Expr* domain;

  ForLoopStmt(bool init_forall, VarSymbol* init_index, Expr* init_domain,
	      Stmt* body);

  void print(FILE* outfile);
};


class CondStmt : public Stmt {
 public:
  Expr* condExpr;
  Stmt* thenStmt;
  Stmt* elseStmt;

  CondStmt(Expr* init_condExpr, Stmt* init_thenStmt, 
	   Stmt* init_elseStmt = new NullStmt());

  void print(FILE* outfile);
};

#endif
