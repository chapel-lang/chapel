#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "baseAST.h"
#include "symbol.h"

class Expr;
class AInfo;

class Stmt : public BaseAST {
  AInfo *ainfo;
 public:
  Stmt(astType_t astType);

  void traverse(Traversal* traversal);
  virtual void traverseStmt(Traversal* traversal);

  void codegenVarDefs(FILE* outfile);
  virtual void codegenVarDef(FILE* outfile);
};


class NullStmt : public Stmt {
 public:
  NullStmt(void);

  bool isNull(void);
  
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class VarDefStmt : public Stmt {
 public:
  VarSymbol* var;
  Expr* init;

  VarDefStmt(VarSymbol* init_var, Expr* init_expr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenVarDef(FILE* outfile);
};


class TypeDefStmt : public Stmt {
 public:
  Type* type;

  TypeDefStmt(Type* init_type);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class FnDefStmt : public Stmt {
 public:
  FnSymbol* fn;

  FnDefStmt(FnSymbol* init_fn);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
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

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WhileLoopStmt : public BlockStmt {
 public:
  bool isWhileDo;
  Expr* condition;

  WhileLoopStmt(bool init_whileDo, Expr* init_cond, Stmt* body);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ForLoopStmt : public BlockStmt {
 public:
  bool forall;
  VarSymbol* index;
  Expr* domain;

  ForLoopStmt(bool init_forall, VarSymbol* init_index, Expr* init_domain,
	      Stmt* body);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class CondStmt : public Stmt {
 public:
  Expr* condExpr;
  Stmt* thenStmt;
  Stmt* elseStmt;

  CondStmt(Expr* init_condExpr, Stmt* init_thenStmt, 
	   Stmt* init_elseStmt = new NullStmt());

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};

#endif
