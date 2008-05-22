#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "expr.h"


enum BlockTag {
  BLOCK_NORMAL,
  BLOCK_ATOMIC,
  BLOCK_DO_WHILE,
  BLOCK_FOR,
  BLOCK_FORALL,
  BLOCK_PARAM_FOR,
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
  LabelSymbol* pre_loop;
  LabelSymbol* post_loop;
  Vec<ModuleSymbol*> modUses; // modules used via use statement

  BlockStmt(Expr* init_body = NULL, BlockTag init_blockTag = BLOCK_NORMAL);
  virtual ~BlockStmt();
  virtual void verify();
  COPY_DEF(BlockStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  void codegen(FILE* outfile);

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);

  bool isLoop(void);
  int length(void);
};


class CondStmt : public Expr {
 public:
  Expr* condExpr;
  BlockStmt* thenStmt;
  BlockStmt* elseStmt;

  CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt = NULL);
  Expr* fold_cond_stmt();
  virtual void verify();
  COPY_DEF(CondStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);

  void codegen(FILE* outfile);
};


enum GotoTag {
  GOTO_NORMAL,
  GOTO_BREAK,
  GOTO_CONTINUE
};


class GotoStmt : public Expr {
 public:
  SymExpr* label;
  GotoTag gotoTag;

  GotoStmt(GotoTag init_gotoTag, const char* init_label);
  GotoStmt(GotoTag init_gotoTag, Symbol* init_label);
  GotoStmt(GotoTag init_gotoTag, SymExpr* init_label);
  virtual void verify();
  COPY_DEF(GotoStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  void codegen(FILE* outfile);
};


void codegenStmt(FILE* outfile, Expr* stmt);


#endif
