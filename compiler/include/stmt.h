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

  Symbol* parentSymbol;

  Stmt(astType_t astType);
  virtual Stmt* copy(void);
  Stmt* copyList(void);

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
  virtual Stmt* copy(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WithStmt : public Stmt {
 public:
  Expr* withExpr;

  WithStmt(Expr* init_withExpr);
  ClassType* getClass(void);
  virtual Stmt* copy(void);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
};


class VarDefStmt : public Stmt {
 public:
  VarSymbol* var;
  Expr* init;

  VarDefStmt(VarSymbol* init_var, Expr* init_expr);
  virtual Stmt* copy(void);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenVarDef(FILE* outfile);
};


class TypeDefStmt : public Stmt {
 public:
  Type* type;

  TypeDefStmt(Type* init_type);
  virtual Stmt* copy(void);

  bool canLiveAtFileScope(void);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class FnDefStmt : public Stmt {
 public:
  FnSymbol* fn;

  FnDefStmt(FnSymbol* init_fn);
  virtual Stmt* copy(void);
  Stmt* clone(void);

  bool isNull(void);
  bool canLiveAtFileScope(void);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};

extern FnDefStmt* nilFnDefStmt;


class ModuleDefStmt : public Stmt {
 public:
  ModuleSymbol* module;

  ModuleDefStmt(ModuleSymbol* init_module);

  void codegen(FILE* outfile);
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);
  virtual Stmt* copy(void);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);

  static ExprStmt* createFnCallStmt(FnSymbol* fnSym, Expr* argList = nilExpr);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* retExpr);
  virtual Stmt* copy(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class BlockStmt : public Stmt {
 public:
  Stmt* body;

  BlockStmt::BlockStmt(Stmt* init_body = nilStmt);
  void addBody(Stmt* body);
  virtual Stmt* copy(void);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WhileLoopStmt : public BlockStmt {
 public:
  bool isWhileDo;
  Expr* condition;

  WhileLoopStmt(bool init_whileDo, Expr* init_cond, Stmt* body);
  virtual Stmt* copy(void);

  bool topLevelExpr(Expr* testExpr);

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
	      Stmt* body = nilStmt);
  virtual Stmt* copy(void);

  bool topLevelExpr(Expr* testExpr);

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
	   Stmt* init_elseStmt = nilStmt);
  virtual Stmt* copy(void);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};

#endif
