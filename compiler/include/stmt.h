#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "expr.h"


extern bool printCppLineno;
extern bool printChplLineno;
extern bool inUserModule;
extern bool justStartedGeneratingFunction;

void codegenStmt(FILE* outfile, Expr* stmt);

class ReturnStmt : public Expr {
 public:
  Expr* expr;
  bool yield;
  ReturnStmt(Expr* initExpr = NULL, bool init_yield = false);
  ReturnStmt(Symbol* initExpr, bool init_yield = false);
  ReturnStmt(char* initExpr, bool init_yield = false);
  virtual void verify();
  COPY_DEF(ReturnStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  bool returnsVoid();
};


enum BlockTag {
  BLOCK_NORMAL = 0,
  BLOCK_SCOPELESS,
  BLOCK_WHILE_DO,
  BLOCK_DO_WHILE,
  BLOCK_FOR,
  BLOCK_FORALL,
  BLOCK_ORDERED_FORALL,
  BLOCK_PARAM_FOR,
  BLOCK_ATOMIC,
  BLOCK_COBEGIN,
  BLOCK_BEGIN
};


class BlockStmt : public Expr {
 public:
  BlockTag blockTag;
  AList* body;
  SymScope* blkScope;
  LabelSymbol* pre_loop;
  LabelSymbol* post_loop;
  Vec<ModuleSymbol*> modUses; // modules used via use statement

  BlockStmt::BlockStmt(AList* init_body = new AList(), 
                       BlockTag init_blockTag = BLOCK_NORMAL);
  BlockStmt::BlockStmt(Expr* init_body,
                       BlockTag init_blockTag = BLOCK_NORMAL);
  BlockStmt::~BlockStmt();
  virtual void verify();
  COPY_DEF(BlockStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  void print(FILE* outfile);
  void codegen(FILE* outfile);

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);
  void insertAtHead(AList* ast);
  void insertAtTail(AList* ast);

  bool isLoop(void);
};


class CondStmt : public Expr {
 public:
  Expr* condExpr;
  BlockStmt* thenStmt;
  BlockStmt* elseStmt;

  CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt = NULL);
  virtual void verify();
  COPY_DEF(CondStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


enum gotoType {
  goto_normal = 0,
  goto_break,
  goto_continue
};


class GotoStmt : public Expr {
 public:
  Symbol* label;
  gotoType goto_type;

  GotoStmt(gotoType init_goto_type);
  GotoStmt(gotoType init_goto_type, char* init_label);
  GotoStmt(gotoType init_goto_type, Symbol* init_label);
  virtual void verify();
  COPY_DEF(GotoStmt);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


#endif
