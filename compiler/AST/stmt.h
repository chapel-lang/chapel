#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "link.h"
class Expr;

class Stmt : public Link {
 public:
  char* filename;
  int lineno;

  Stmt();

  virtual void print(FILE* outfile) = 0;
  void printList(FILE* outfile);
};


class NullStmt : public Stmt {
 public:

  bool isNull(void);
  
  void print(FILE* outfile) { fprintf(outfile, "Null Statement\n"); }
};


class VarDefStmt : public Stmt {
 public:
  Symbol* var;
  Expr* init;

  VarDefStmt(Symbol* init_var, Expr* init_expr);

  void print(FILE*);
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);

  void print(FILE*);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* retExpr);

  void print(FILE*);
};


class LoopStmt : public Stmt {
 public:
  Stmt* body;

  LoopStmt::LoopStmt(Stmt* init_body);

  void print(FILE*);
};


class WhileLoopStmt : public LoopStmt {
 public:
  bool topTest;
  Expr* condition;

  WhileLoopStmt(bool init_topTest, Expr* init_cond, Stmt* body);

  void print(FILE*);
};


class ForLoopStmt : public LoopStmt {
 public:
  bool forall;
  Expr* index;
  Expr* domain;

  ForLoopStmt(bool init_forall, Expr* init_index, Expr* init_domain,
	      Stmt* body);

  void print(FILE*);
};

#endif

