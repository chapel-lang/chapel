#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "baseAST.h"
#include "symbol.h"
#include "analysis.h"

class Expr;
class AInfo;

class Stmt : public BaseAST {
 public:
  Symbol* parentSymbol;
  AInfo *ainfo;
  Stmt** back;

  Stmt(astType_t astType);

  bool isNull(void);
  virtual bool canLiveAtFileScope(void);
  virtual bool topLevelExpr(Expr* testExpr);

  Stmt* copyList(CloneCallback* analysis_clone = NULL);
  Stmt* copy(CloneCallback* analysis_clone = NULL);
  Stmt* copyListInternal(CloneCallback* analysis_clone = NULL);
  Stmt* copyInternal(CloneCallback* analysis_clone = NULL);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseStmt(Traversal* traversal);

  void codegenVarDefs(FILE* outfile);
  virtual void codegenVarDef(FILE* outfile);

  void codegenVarNames(FILE* outfile, char* premod, char* postmod);
  virtual void codegenVarName(FILE* outfile, char* premod, char* postmod);

  void replace(Stmt* new_stmt);
  virtual void append(ILink* new_stmt);
  void insertBefore(Stmt* new_stmt);
  void insertAfter(Stmt* new_stmt);
  Stmt* extract(void);
};
#define forv_Stmt(_p, _v) forv_Vec(Stmt, _p, _v)

extern Stmt* nilStmt;


class NoOpStmt : public Stmt {
 public:
  NoOpStmt(void);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WithStmt : public Stmt {
 public:
  Expr* withExpr;

  WithStmt(Expr* init_withExpr);
  ClassType* getClass(void);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class VarDefStmt : public Stmt {
 public:
  VarSymbol* var;
  Expr* init;

  VarDefStmt(VarSymbol* init_var, Expr* init_expr);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenVarDef(FILE* outfile);
  void codegenVarName(FILE* outfile, char* premod, char* postmod);
};


class TypeDefStmt : public Stmt {
 public:
  Type* type;

  TypeDefStmt(Type* init_type);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  bool canLiveAtFileScope(void);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class FnDefStmt : public Stmt {
 public:
  FnSymbol* fn;

  FnDefStmt(FnSymbol* init_fn);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);
  FnDefStmt* clone(CloneCallback* clone_callback);

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
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);

  static ExprStmt* createFnCallStmt(FnSymbol* fnSym, Expr* argList = nilExpr);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* retExpr);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class BlockStmt : public Stmt {
 public:
  Stmt* body;

  SymScope* blkScope;

  BlockStmt::BlockStmt(Stmt* init_body = nilStmt);
  void addBody(Stmt* init_body);
  void setBlkScope(SymScope* init_blkScope);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WhileLoopStmt : public BlockStmt {
 public:
  bool isWhileDo;
  Expr* condition;

  WhileLoopStmt(bool init_whileDo, Expr* init_cond, Stmt* body);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

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

  SymScope* indexScope;

  ForLoopStmt(bool init_forall, VarSymbol* init_index, Expr* init_domain,
	      Stmt* body = nilStmt);
  void setIndexScope(SymScope* init_indexScope);
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

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
  virtual Stmt* copyStmt(CloneCallback* analysis_clone);

  bool topLevelExpr(Expr* testExpr);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};

#endif
