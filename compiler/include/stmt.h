#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "baseAST.h"
#include "symbol.h"

#define TRAVERSABLE_STMT(name)                                                       \
 public:                                                                             \
  void traverse(name* &_this, Traversal* traversal, bool atTop = true) {             \
    if (isNull()) return;                                                            \
    if (traversal->processTop || !atTop) traversal->preProcessStmt((Stmt*&)_this);   \
    if (atTop || traversal->exploreChildStmts) _this->traverseStmt(traversal);       \
    if (traversal->processTop || !atTop) traversal->postProcessStmt((Stmt*&)_this);  \
  };                                                                                 \
  void traverseList(name* &_this, Traversal* traversal, bool atTop = true) {         \
    if (isNull()) return;                                                            \
    TRAVERSE(_this, traversal, atTop);                                               \
    TRAVERSE_LS(_this->next, traversal, atTop);                                      \
  }

class Expr;
class AInfo;

class Stmt : public BaseAST {
  TRAVERSABLE_STMT(Stmt);
 public:
  AInfo *ainfo;

  Symbol* parentSymbol;

  Stmt(astType_t astType);
  virtual Stmt* copy(void);
  Stmt* copyList(void);
  Stmt* copy(SymScope* new_scope);
  Stmt* copyList(SymScope* new_scope);

  bool isNull(void);
  virtual bool canLiveAtFileScope(void);
  virtual bool topLevelExpr(Expr* testExpr);

  virtual void traverseStmt(Traversal* traversal);

  void codegenVarDefs(FILE* outfile);
  virtual void codegenVarDef(FILE* outfile);

  static void replace(Stmt* &old_stmt, Stmt* new_stmt);
};
#define forv_Stmt(_p, _v) forv_Vec(Stmt, _p, _v)

extern Stmt* nilStmt;


class NoOpStmt : public Stmt {
  TRAVERSABLE_STMT(NoOpStmt);
 public:
  NoOpStmt(void);
  virtual Stmt* copy(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WithStmt : public Stmt {
  TRAVERSABLE_STMT(WithStmt);
 public:
  Expr* withExpr;

  WithStmt(Expr* init_withExpr);
  ClassType* getClass(void);
  virtual Stmt* copy(void);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class VarDefStmt : public Stmt {
  TRAVERSABLE_STMT(VarDefStmt);
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
  TRAVERSABLE_STMT(TypeDefStmt);
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
  TRAVERSABLE_STMT(FnDefStmt);
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
  TRAVERSABLE_STMT(ModuleDefStmt);
 public:
  ModuleSymbol* module;

  ModuleDefStmt(ModuleSymbol* init_module);

  void codegen(FILE* outfile);
};


class ExprStmt : public Stmt {
  TRAVERSABLE_STMT(ExprStmt);
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
  TRAVERSABLE_STMT(ReturnStmt);
 public:
  ReturnStmt(Expr* retExpr);
  virtual Stmt* copy(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class BlockStmt : public Stmt {
  TRAVERSABLE_STMT(BlockStmt);
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
  TRAVERSABLE_STMT(WhileLoopStmt);
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
  TRAVERSABLE_STMT(ForLoopStmt);
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
  TRAVERSABLE_STMT(CondStmt);
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
