#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "expr.h"


extern bool printCppLineno;

void codegenStmt(FILE* outfile, Expr* stmt);

enum BlockTag {
  BLOCK_NORMAL = 0,
  BLOCK_ATOMIC,
  BLOCK_BEGIN,
  BLOCK_COBEGIN,
  BLOCK_DO_WHILE,
  BLOCK_FOR,
  BLOCK_FORALL,
  BLOCK_ORDERED_FORALL,
  BLOCK_PARAM_FOR,
  BLOCK_COFORALL,
  BLOCK_SERIAL,
  BLOCK_SCOPELESS,
  BLOCK_TYPE,
  BLOCK_WHILE_DO
};


class BlockStmt : public Expr {
 public:
  BlockTag blockTag;
  AList body;
  CallExpr* loopInfo;
  SymScope* blkScope;
  LabelSymbol* pre_loop;
  LabelSymbol* post_loop;
  Vec<ModuleSymbol*> modUses; // modules used via use statement

  BlockStmt(AList* init_body, BlockTag init_blockTag = BLOCK_NORMAL);
  BlockStmt(Expr* init_body = NULL, BlockTag init_blockTag = BLOCK_NORMAL);
  virtual ~BlockStmt();
  virtual void verify();
  COPY_DEF(BlockStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  void codegen(FILE* outfile);

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);
  void insertAtHead(AList* ast);
  void insertAtTail(AList* ast);

  bool isLoop(void);
  int length(void);
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
  GotoStmt(gotoType init_goto_type, const char* init_label);
  GotoStmt(gotoType init_goto_type, Symbol* init_label);
  virtual void verify();
  COPY_DEF(GotoStmt);
  void codegen(FILE* outfile);
};


#endif
