#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "baseAST.h"
#include "symbol.h"

class Expr;
class AInfo;

class Stmt : public BaseAST {
 public:
  AInfo *ainfo;

  FnSymbol* parentFn;

  Stmt(astType_t astType);

  bool isNull(void);
  virtual bool canLiveAtFileScope(void);
  virtual bool topLevelExpr(Expr* testExpr);

  void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseStmt(Traversal* traversal);

  void codegenVarDefs(FILE* outfile);
  virtual void codegenVarDef(FILE* outfile);
};
#define forv_Stmt(_p, _v) forv_Vec(Stmt, _p, _v)


extern Stmt* nilStmt;


class NoOpStmt : public Stmt {
 public:
  NoOpStmt(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class VarDefStmt : public Stmt {
 public:
  VarSymbol* var;
  Expr* init;

  VarDefStmt(VarSymbol* init_var, Expr* init_expr);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenVarDef(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
  int getExprs(Vec<BaseAST *> &asts);
};


class TypeDefStmt : public Stmt {
 public:
  Type* type;

  TypeDefStmt(Type* init_type);

  bool canLiveAtFileScope(void);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getTypes(Vec<BaseAST *> &asts);
};


class FnDefStmt : public Stmt {
 public:
  FnSymbol* fn;

  FnDefStmt(FnSymbol* init_fn);

  bool isNull(void);
  bool canLiveAtFileScope(void);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
};

extern FnDefStmt* nilFnDefStmt;


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* retExpr);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class BlockStmt : public Stmt {
 public:
  Stmt* body;

  BlockStmt::BlockStmt(Stmt* init_body);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getStmts(Vec<BaseAST *> &asts);
};


class WhileLoopStmt : public BlockStmt {
 public:
  bool isWhileDo;
  Expr* condition;

  WhileLoopStmt(bool init_whileDo, Expr* init_cond, Stmt* body);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
};


class ForLoopStmt : public BlockStmt {
 public:
  bool forall;
  VarSymbol* index;
  Expr* domain;

  ForLoopStmt(bool init_forall, VarSymbol* init_index, Expr* init_domain,
	      Stmt* body);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
  int getExprs(Vec<BaseAST *> &asts);
};


class CondStmt : public Stmt {
 public:
  Expr* condExpr;
  Stmt* thenStmt;
  Stmt* elseStmt;

  CondStmt(Expr* init_condExpr, Stmt* init_thenStmt, 
	   Stmt* init_elseStmt = nilStmt);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
  int getStmts(Vec<BaseAST *> &asts);
};

#endif
