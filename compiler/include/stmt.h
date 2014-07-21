#ifndef _STMT_H_
#define _STMT_H_

#include <cstdio>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "expr.h"

#include "genret.h"

enum BlockTag {
// Bits:
  BLOCK_NORMAL		= 0,
  BLOCK_SCOPELESS	= 1<<0, ///< does not introduce a new scope
  BLOCK_TYPE_ONLY  	= 1<<1, ///< deleted after type resolution
  BLOCK_EXTERN		= 1<<2, ///< init block for an extern var
// Bit masks:
  BLOCK_TYPE		= BLOCK_SCOPELESS | BLOCK_TYPE_ONLY,
};


class Stmt : public Expr {
public:
  Stmt(AstTag astTag) : Expr(astTag) {}
  virtual ~Stmt();

  virtual bool isStmt() const { return true; }
  virtual void replaceChild(Expr* old_ast, Expr* new_ast) = 0;
  virtual void verify()                                   = 0;
};


class BlockStmt : public Stmt {
public:
  BlockTag      blockTag;
  AList         body;
  CallExpr*     blockInfo;
  CallExpr*     modUses;       // module uses via PRIM_USE
  LabelSymbol*  breakLabel;
  LabelSymbol*  continueLabel;
  const char*   userLabel;
  CallExpr*     byrefVars;     // 'ref' clause in begin/cobegin/coforall

                BlockStmt(Expr* init_body = NULL, BlockTag init_blockTag = BLOCK_NORMAL);
  virtual      ~BlockStmt();

  DECLARE_COPY(BlockStmt);

  virtual void  replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void  verify();
  virtual void  accept(AstVisitor* visitor);
  GenRet        codegen();

  void          appendChapelStmt(BlockStmt* stmt);

  void          insertAtHead(Expr* ast);
  void          insertAtTail(Expr* ast);
  void          insertAtTailBeforeGoto(Expr* ast);

  void          insertAtHead(const char* format, ...);
  void          insertAtTail(const char* format, ...);

  bool          isScopeless()                           const;
  bool          isLoop()                                const;
  int           length()                                const;

  void          addUse(ModuleSymbol* mod);
  void          removeUse(ModuleSymbol* mod);

private:
  bool         canFlattenChapelStmt(const BlockStmt* stmt)    const;
};


class CondStmt : public Stmt {
 public:
  Expr* condExpr;
  BlockStmt* thenStmt;
  BlockStmt* elseStmt;

  CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt = NULL);
  Expr* fold_cond_stmt();
  DECLARE_COPY(CondStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void verify();
  virtual void    accept(AstVisitor* visitor);

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


class GotoStmt : public Stmt {
 public:
  GotoTag gotoTag;
  Expr* label;

  GotoStmt(GotoTag init_gotoTag, const char* init_label);
  GotoStmt(GotoTag init_gotoTag, Symbol* init_label);
  GotoStmt(GotoTag init_gotoTag, Expr* init_label);
  DECLARE_COPY(GotoStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void verify();
  virtual void    accept(AstVisitor* visitor);

  GenRet codegen();

  const char* getName();
};

class ExternBlockStmt : public Stmt {
 public:
  const char* c_code;

  ExternBlockStmt(const char* c_code);
  DECLARE_COPY(ExternBlockStmt);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void verify();
  virtual void    accept(AstVisitor* visitor);

  GenRet codegen();
};


// Probably belongs in Expr; doesn't really mean Stmt, but rather
// statement-level expression.
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
