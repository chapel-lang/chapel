#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "alist.h"
#include "analysis.h"
#include "baseAST.h"
#include "symbol.h"


extern bool printCppLineno;
extern bool printChplLineno;
extern bool inFunction;
extern bool inUserModule;
extern bool justStartedGeneratingFunction;

class Expr;
class DefExpr;
class AInfo;

class Stmt : public BaseAST {
 public:
  Symbol* parentSymbol;
  Stmt* parentStmt;
  AInfo *ainfo;

  Stmt(astType_t astType = STMT);
  COPY_DEF(Stmt);
  void codegen(FILE* outfile);
  virtual void codegenStmt(FILE* outfile);
  virtual void callReplaceChild(BaseAST* new_ast);
  virtual void verify(void);
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseStmt(Traversal* traversal);
  FnSymbol* parentFunction();
};
#define forv_Stmt(_p, _v) forv_Vec(Stmt, _p, _v)


class NoOpStmt : public Stmt {
 public:
  NoOpStmt(void);
  COPY_DEF(NoOpStmt);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class ExprStmt : public Stmt {
 public:
  Expr* expr;

  ExprStmt(Expr* initExpr);
  COPY_DEF(ExprStmt);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  virtual void print(FILE* outfile);
  virtual void codegenStmt(FILE* outfile);
};


class ReturnStmt : public ExprStmt {
 public:
  bool yield;
  ReturnStmt(Expr* initExpr, bool init_yield = false);
  COPY_DEF(ReturnStmt);
  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
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
  COPY_DEF(BlockStmt);
  void addBody(AList<Stmt>* init_body);
  void setBlkScope(SymScope* init_blkScope);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class WhileLoopStmt : public Stmt {
 public:
  BlockStmt* block;
  bool isWhileDo;
  Expr* condition;

  WhileLoopStmt(bool init_whileDo, Expr* init_cond, BlockStmt* init_block);
  COPY_DEF(WhileLoopStmt);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);

  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


enum ForLoopStmtTag {
  FORLOOPSTMT_FOR,           // for ... do
  FORLOOPSTMT_ORDEREDFORALL, // ordered forall ... do
  FORLOOPSTMT_FORALL         // forall .. do
};


class ForLoopStmt : public Stmt {
 public:
  ForLoopStmtTag forLoopStmtTag;
  AList<DefExpr>* indices;
  AList<Expr>* iterators;
  BlockStmt* innerStmt;
  SymScope* indexScope;

  ForLoopStmt(ForLoopStmtTag initForLoopStmtTag,
              AList<DefExpr>* initIndices,
              AList<Expr>* initIterators,
              BlockStmt* initInnerStmt = NULL,
              SymScope* initIndexScope = NULL);
  COPY_DEF(ForLoopStmt);
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
  COPY_DEF(CondStmt);
  void addElseStmt(BlockStmt* init_elseStmt);
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
  COPY_DEF(WhenStmt);
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
  COPY_DEF(SelectStmt);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


class LabelStmt : public Stmt {
 public:
  LabelSymbol* label;
  Stmt* stmt;
  
  LabelStmt(LabelSymbol* init_label, Stmt* init_stmt);
  COPY_DEF(LabelStmt);
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
  COPY_DEF(GotoStmt);
  void traverseStmt(Traversal* traversal);
  void print(FILE* outfile);
  void codegenStmt(FILE* outfile);
};


#endif
