#ifndef _STMT_H_
#define _STMT_H_

#include <stdio.h>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "expr.h"


enum BlockTag {
  BLOCK_NORMAL,
  BLOCK_SCOPELESS, // does not introduce a new scope
  BLOCK_TYPE       // deleted after type resolution
};


class BlockStmt : public Expr {
 public:
  BlockTag blockTag;
  AList body;
  CallExpr* blockInfo;
  CallExpr* modUses;  // module uses via PRIMITIVE_USE

  BlockStmt(Expr* init_body = NULL, BlockTag init_blockTag = BLOCK_NORMAL);
  virtual ~BlockStmt();
  virtual void verify();
  DECLARE_COPY(BlockStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  void codegen(FILE* outfile);

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);

  bool isLoop(void);
  int length(void);

  void addUse(ModuleSymbol* mod);
};


class CondStmt : public Expr {
 public:
  Expr* condExpr;
  BlockStmt* thenStmt;
  BlockStmt* elseStmt;

  CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt = NULL);
  Expr* fold_cond_stmt();
  virtual void verify();
  DECLARE_COPY(CondStmt);
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
  DECLARE_COPY(GotoStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  void codegen(FILE* outfile);
};


void codegenStmt(FILE* outfile, Expr* stmt);


#endif
