#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "alist.h"
#include "analysis.h"
#include "baseAST.h"
#include "pragma.h"
#include "symbol.h"

extern bool printCppLineno;

class Expr;
class DefExpr;
class AInfo;

class Stmt : public BaseAST {
 public:
  Symbol* parentSymbol;
  Stmt* parentStmt;
  AInfo *ainfo;
  AList<Pragma>* pragmas;

  Stmt(astType_t astType = STMT);

  FnSymbol *parentFunction();

  Stmt* copy(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, Vec<BaseAST*>* update_list = NULL);
  Stmt* copyInternal(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  void codegen(FILE* outfile);
  virtual void codegenStmt(FILE* outfile);
  virtual void callReplaceChild(BaseAST* new_ast);
  virtual void verify(void);
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseStmt(Traversal* traversal);

  bool hasPragma(char* str);
  void addPragma(char* str);
};
#define forv_Stmt(_p, _v) forv_Vec(Stmt, _p, _v)


class NoOpStmt : public Stmt {
 public:
  NoOpStmt(void);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class DefStmt : public Stmt {
public:
  AList<DefExpr>* defExprls;

  DefStmt(DefExpr* init_defExprls);
  DefStmt(AList<DefExpr>* init_defExprls);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);

  VarSymbol* varDef();
  FnSymbol* fnDef();

  bool definesTypes();
  bool definesFunctions();
  bool definesVariables();
  Vec<VarSymbol*>* varDefSet();
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegenStmt(FILE* outfile);
};


class ReturnStmt : public ExprStmt {
 public:
  ReturnStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class WithStmt : public ExprStmt {
 public:
  WithStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);

  StructuralType* getStruct(void);
};


class UseStmt : public ExprStmt {
 public:
  UseStmt(Expr* initExpr);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);

  ModuleSymbol* getModule(void);
};


enum blockStmtType {
  BLOCK_NORMAL = 0,
  BLOCK_ATOMIC,
  BLOCK_COBEGIN
};


class BlockStmt : public Stmt {
 public:
  blockStmtType blockType;
  AList<Stmt>* body;

  SymScope* blkScope;

  BlockStmt::BlockStmt(AList<Stmt>* init_body = new AList<Stmt>(), 
                       SymScope* init_scope = NULL, 
                       blockStmtType init_blockType = BLOCK_NORMAL);
  void addBody(AList<Stmt>* init_body);
  void setBlkScope(SymScope* init_blkScope);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class WhileLoopStmt : public BlockStmt {
 public:
  bool isWhileDo;
  Expr* condition;

  WhileLoopStmt(bool init_whileDo, Expr* init_cond, AList<Stmt>* body);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class ForLoopStmt : public BlockStmt {
 public:
  bool forall;
  AList<DefExpr>* indices; // DefExpr* containing local index variables
  Expr* domain;

  SymScope* indexScope;

  ForLoopStmt(bool init_forall, AList<DefExpr>* init_indices, Expr* init_domain,
              AList<Stmt>* body = new AList<Stmt>());
  void setIndexScope(SymScope* init_indexScope);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class CondStmt : public Stmt {
 public:
  Expr* condExpr;
  BlockStmt* thenStmt;
  BlockStmt* elseStmt;

  CondStmt(Expr* init_condExpr, BlockStmt* init_thenStmt, 
           BlockStmt* init_elseStmt = NULL);
  void addElseStmt(BlockStmt* init_elseStmt);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void verify(void);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class WhenStmt : public Stmt {
 public:
  AList<Expr>* caseExprs;
  BlockStmt* doStmt;

  WhenStmt(AList<Expr>* init_caseExprs = NULL, BlockStmt* init_doStmt = NULL);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class SelectStmt : public Stmt {
 public:
  Expr* caseExpr;
  AList<WhenStmt>* whenStmts;

  SelectStmt(Expr* init_caseExpr = NULL, AList<WhenStmt>* init_whenStmts = NULL);
  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class LabelStmt : public Stmt {
 public:
  LabelSymbol* label;
  BlockStmt* stmt;
  
  LabelStmt(LabelSymbol* init_label, BlockStmt* init_stmt);

  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
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

  virtual Stmt* copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map);

  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


#endif
