#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "baseAST.h"
#include "symbol.h"
#include "analysis.h"
#include "pragma.h"

class Expr;
class DefExpr;
class AInfo;

class Stmt : public BaseAST {
 public:
  Symbol* parentSymbol;
  Stmt* parentStmt;
  AInfo *ainfo;
  Pragma *pragmas;

  Stmt(astType_t astType);

  FnSymbol *parentFunction();

  Stmt* copyList(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL, Vec<BaseAST*>* update_list = NULL);
  Stmt* copy(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL, Vec<BaseAST*>* update_list = NULL);
  Stmt* copyListInternal(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL);
  Stmt* copyInternal(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void verify(void);
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseStmt(Traversal* traversal);

  Stmt* head(void);
  Stmt* tail(void);
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


class DefStmt : public Stmt {
public:
  DefExpr* defExprls;

  DefStmt(DefExpr* init_defExprls);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);

  VarSymbol* varDef();
  FnSymbol* fnDef();
  TypeSymbol* typeDef();
  ModuleSymbol* moduleDef();

  Vec<VarSymbol*>* varDefSet();
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegen(FILE* outfile);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class WithStmt : public ExprStmt {
 public:
  WithStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void print(FILE* outfile);
  void codegen(FILE* outfile);

  StructuralType* getStruct(void);
};


class UseStmt : public ExprStmt {
 public:
  UseStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void print(FILE* outfile);
  void codegen(FILE* outfile);

  ModuleSymbol* getModule(void);
};


class BlockStmt : public Stmt {
 public:
  Stmt* body;

  SymScope* blkScope;

  BlockStmt::BlockStmt(Stmt* init_body = NULL, SymScope* init_scope = NULL);
  void addBody(Stmt* init_body);
  void setBlkScope(SymScope* init_blkScope);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
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

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class ForLoopStmt : public BlockStmt {
 public:
  bool forall;
  Expr* indices; // DefExpr* containing local index variables
  Expr* domain;

  SymScope* indexScope;

  ForLoopStmt(bool init_forall, Expr* init_indices, Expr* init_domain,
              Stmt* body = NULL);
  void setIndexScope(SymScope* init_indexScope);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
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
  void addElseStmt(Stmt* init_elseStmt);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void verify(void);
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

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
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
