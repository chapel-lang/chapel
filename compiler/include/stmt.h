#ifndef _STMT_H_
#define _STMT_H_

#include <cstdio>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "expr.h"

#include "genret.h"

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
  CallExpr* modUses;  // module uses via PRIM_USE
  LabelSymbol* breakLabel;
  LabelSymbol* continueLabel;
  const char* userLabel;
  CallExpr* byrefVars;  // 'ref' clause in begin/cobegin/coforall

  BlockStmt(Expr* init_body = NULL, BlockTag init_blockTag = BLOCK_NORMAL);
  ~BlockStmt();
  void verify();
  DECLARE_COPY(BlockStmt);
  void replaceChild(Expr* old_ast, Expr* new_ast);
  GenRet codegen();

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);
  void insertAtHead(const char* format, ...);
  void insertAtTail(const char* format, ...);
  void insertAtTailBeforeGoto(Expr* ast);

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
  void verify();
  DECLARE_COPY(CondStmt);
  void replaceChild(Expr* old_ast, Expr* new_ast);

  GenRet codegen();
};


enum GotoTag {
  GOTO_NORMAL,
  GOTO_BREAK,
  GOTO_CONTINUE,
  GOTO_RETURN,
  GOTO_GETITER_END,
  GOTO_ITER_RESUME,
  GOTO_ITER_END
};


class GotoStmt : public Expr {
 public:
  GotoTag gotoTag;
  Expr* label;

  GotoStmt(GotoTag init_gotoTag, const char* init_label);
  GotoStmt(GotoTag init_gotoTag, Symbol* init_label);
  GotoStmt(GotoTag init_gotoTag, Expr* init_label);
  void verify();
  DECLARE_COPY(GotoStmt);
  void replaceChild(Expr* old_ast, Expr* new_ast);
  GenRet codegen();

  const char* getName();
};

class ExternBlockStmt : public Expr {
 public:
  const char* c_code;

  ExternBlockStmt(const char* c_code);
  void verify();
  DECLARE_COPY(ExternBlockStmt);
  void replaceChild(Expr* old_ast, Expr* new_ast);
  GenRet codegen();
};


void codegenStmt(Expr* stmt);

// Extract (e.toGotoStmt)->(label.toSymExpr)->var and var->->iterResumeGoto,
// if possible; NULL otherwise.
LabelSymbol* getGotoLabelSymbol(GotoStmt* gs);
GotoStmt*    getGotoLabelsIterResumeGoto(GotoStmt* gs);

extern Vec<LabelSymbol*> removedIterResumeLabels;
void removeDeadIterResumeGotos();
void verifyNcleanRemovedIterResumeGotos();

extern Map<GotoStmt*,GotoStmt*> copiedIterResumeGotos;
void verifyNcleanCopiedIterResumeGotos();

#endif
