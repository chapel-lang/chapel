#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "baseAST.h"
#include "symbol.h"
#include "analysis.h"
#include "pragma.h"

class Expr;
class AInfo;

class Stmt : public BaseAST {
 public:
  Symbol* parentSymbol;
  AInfo *ainfo;
  Stmt** back;
  Pragma *pragmas;

  Stmt(astType_t astType);

  FnSymbol *parentFunction();

  Stmt* copyList(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL, Vec<BaseAST*>* update_list = NULL);
  Stmt* copy(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL, Vec<BaseAST*>* update_list = NULL);
  Stmt* copyListInternal(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL);
  Stmt* copyInternal(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseStmt(Traversal* traversal);

  void replace(Stmt* new_stmt);
  virtual void append(ILink* new_stmt);
  void insertBefore(Stmt* new_stmt);
  void insertAfter(Stmt* new_stmt);
  Stmt* extract(void);
};
#define forv_Stmt(_p, _v) forv_Vec(Stmt, _p, _v)


class NoOpStmt : public Stmt {
 public:
  NoOpStmt(void);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WithStmt : public Stmt {
 public:
  Expr* withExpr;

  WithStmt(Expr* init_withExpr);
  ClassType* getClass(void);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class DefStmt : public Stmt {
public:
  Symbol* def_sym;

  DefStmt(Symbol* init_def_sym);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);

  bool isVarDef();
  bool isFnDef();
  bool isTypeDef();
  bool isModuleDef();
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseStmt(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);

  static ExprStmt* createFnCallStmt(FnSymbol* fnSym, Expr* argList = NULL);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* retExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class BlockStmt : public Stmt {
 public:
  Stmt* body;

  SymScope* blkScope;

  BlockStmt::BlockStmt(Stmt* init_body = NULL);
  void addBody(Stmt* init_body);
  void setBlkScope(SymScope* init_blkScope);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WhileLoopStmt : public BlockStmt {
 public:
  bool isWhileDo;
  Expr* condition;

  WhileLoopStmt(bool init_whileDo, Expr* init_cond, Stmt* body);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

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
	      Stmt* body = NULL);
  void setIndexScope(SymScope* init_indexScope);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

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
	   Stmt* init_elseStmt = NULL);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class LabelStmt : public Stmt {
 public:
  LabelSymbol* label;
  Stmt* stmt;
  
  LabelStmt(LabelSymbol* init_label, Stmt* init_stmt);

  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


enum gotoType {
  goto_normal = 0,
  goto_break,
  goto_continue
};


class GotoStmt : public Stmt {
 public:
  Symbol* label;
  gotoType goto_type;

  GotoStmt(gotoType init_goto_type);
  GotoStmt(gotoType init_goto_type, char* init_label);
  GotoStmt(gotoType init_goto_type, Symbol* init_label);

  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


#endif
