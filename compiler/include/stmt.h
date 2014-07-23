#ifndef _STMT_H_
#define _STMT_H_

#include "expr.h"

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class Stmt : public Expr {
public:
                 Stmt(AstTag astTag);
  virtual       ~Stmt();

  // Interface to Expr
  virtual bool   isStmt()                                      const;
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

enum BlockTag {
  BLOCK_NORMAL,
  BLOCK_SCOPELESS, // does not introduce a new scope
  BLOCK_TYPE       // deleted after type resolution
};

class BlockStmt : public Stmt {
public:
                  BlockStmt(Expr*    initBody     = NULL, 
                            BlockTag initBlockTag = BLOCK_NORMAL);
  virtual        ~BlockStmt();

  DECLARE_COPY(BlockStmt);

  // Interface to BaseAST
  virtual GenRet  codegen();
  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);

  // Interface to Expr
  virtual void    replaceChild(Expr* oldAst, Expr* newAst);

  // New interface
  void            appendChapelStmt(BlockStmt* stmt);

  void            insertAtHead(Expr* ast);
  void            insertAtTail(Expr* ast);
  void            insertAtTailBeforeGoto(Expr* ast);

  void            insertAtHead(const char* format, ...);
  void            insertAtTail(const char* format, ...);

  bool            isScopeless()                                const;
  bool            isLoop()                                     const;
  int             length()                                     const;

  void            moduleAddUse(ModuleSymbol* mod);
  void            moduleRemoveUse(ModuleSymbol* mod);

  BlockTag        blockTag;
  AList           body;
  CallExpr*       blockInfo;
  CallExpr*       modUses;       // module uses via PRIM_USE
  LabelSymbol*    breakLabel;
  LabelSymbol*    continueLabel;
  const char*     userLabel;
  CallExpr*       byrefVars;     // 'ref' clause in begin/cobegin/coforall

private:
  bool            canFlattenChapelStmt(const BlockStmt* stmt)  const;
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

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

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

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

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class ExternBlockStmt : public Stmt {
public:
                  ExternBlockStmt(const char* c_code);

  // Interface to BaseAST
  virtual GenRet  codegen();
  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);

  DECLARE_COPY(ExternBlockStmt);

  // Interface to Expr
  virtual void    replaceChild(Expr* oldAst, Expr* newAst);

  // Local interface
  const char*     c_code;
};


/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

extern Vec<LabelSymbol*>         removedIterResumeLabels;
extern Map<GotoStmt*, GotoStmt*> copiedIterResumeGotos;


// Probably belongs in Expr; doesn't really mean Stmt, but rather
// statement-level expression.
void         codegenStmt(Expr* stmt);

// Extract (e.toGotoStmt)->(label.toSymExpr)->var and var->->iterResumeGoto,
// if possible; NULL otherwise.
LabelSymbol* getGotoLabelSymbol(GotoStmt* gs);
GotoStmt*    getGotoLabelsIterResumeGoto(GotoStmt* gs);

void         removeDeadIterResumeGotos();
void         verifyNcleanRemovedIterResumeGotos();

void         verifyNcleanCopiedIterResumeGotos();

#endif
