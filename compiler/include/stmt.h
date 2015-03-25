/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _STMT_H_
#define _STMT_H_

#include "expr.h"

#ifdef HAVE_LLVM

#define FNAME(str) (llvm::Twine(getFunction()->cname)            + \
                    llvm::Twine("_")                             + \
                    llvm::Twine(getFunction()->codegenUniqueNum) + \
                    llvm::Twine(str)                             + \
                    llvm::Twine("_"))

#endif

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

class Stmt : public Expr {
public:
                 Stmt(AstTag astTag);
  virtual       ~Stmt();

  // Interface to Expr
  virtual bool   isStmt()                                      const;
};

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

enum BlockTag {
// Bits:
  BLOCK_NORMAL      = 0,
  BLOCK_SCOPELESS   = 1<<0, ///< does not introduce a new scope
  BLOCK_TYPE_ONLY   = 1<<1, ///< deleted after type resolution
  BLOCK_EXTERN      = 1<<2, ///< init block for an extern var
  BLOCK_C_FOR_LOOP  = 1<<3, ///< init/test/incr block for a CForLoop
// Bit masks:
  BLOCK_TYPE        = BLOCK_SCOPELESS | BLOCK_TYPE_ONLY,
};

class BlockStmt : public Stmt {
public:
                      BlockStmt(Expr*    initBody     = NULL,
                                BlockTag initBlockTag = BLOCK_NORMAL);
  virtual            ~BlockStmt();

  DECLARE_COPY(BlockStmt);

  // Interface to BaseAST
  virtual GenRet      codegen();
  virtual void        verify();
  virtual void        accept(AstVisitor* visitor);

  // Interface to Expr
  virtual void        replaceChild(Expr* oldAst, Expr* newAst);
  virtual Expr*       getFirstExpr();
  virtual Expr*       getNextExpr(Expr* expr);

  // New interface
  virtual bool        isLoopStmt()                                 const;

  virtual bool        isWhileStmt()                                const;
  virtual bool        isWhileDoStmt()                              const;
  virtual bool        isDoWhileStmt()                              const;

  virtual bool        isParamForLoop()                             const;
  virtual bool        isForLoop()                                  const;
  virtual bool        isCoforallLoop()                             const;
  virtual bool        isCForLoop()                                 const;

  virtual void        checkConstLoops();

  virtual bool        deadBlockCleanup();

  void                appendChapelStmt(BlockStmt* stmt);

  void                insertAtHead(Expr* ast);
  void                insertAtTail(Expr* ast);
  void                insertAtTailBeforeGoto(Expr* ast);

  void                insertAtHead(const char* format, ...);
  void                insertAtTail(const char* format, ...);

  // I.E. Not a Loop or an OnStmt or ...
  bool                isRealBlockStmt()                            const;

  bool                isScopeless()                                const;
  bool                isBlockType(PrimitiveTag tag)                const;

  int                 length()                                     const;

  void                moduleUseAdd(ModuleSymbol* mod);
  bool                moduleUseRemove(ModuleSymbol* mod);
  void                moduleUseClear();

  virtual CallExpr*   blockInfoGet()                               const;
  virtual CallExpr*   blockInfoSet(CallExpr* expr);

  BlockTag            blockTag;
  AList               body;
  CallExpr*           modUses;       // module uses via PRIM_USE
  const char*         userLabel;
  CallExpr*           byrefVars; //ref-clause in begin/cobegin/coforall/forall

private:
  bool                canFlattenChapelStmt(const BlockStmt* stmt)  const;

  CallExpr*           blockInfo;
};

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

class CondStmt : public Stmt {
public:
                      CondStmt(Expr*    iCondExpr,
                               BaseAST* iThenStmt,
                               BaseAST* iElseStmt = NULL);

                      DECLARE_COPY(CondStmt);

  virtual GenRet      codegen();
  virtual void        replaceChild(Expr* oldAst, Expr* newAst);
  virtual void        verify();
  virtual void        accept(AstVisitor* visitor);

  virtual Expr*       getFirstExpr();
  virtual Expr*       getNextExpr(Expr* expr);

  Expr*               foldConstantCondition();

  Expr*               condExpr;
  BlockStmt*          thenStmt;
  BlockStmt*          elseStmt;
};

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

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

  virtual GenRet      codegen();

  DECLARE_COPY(GotoStmt);

  virtual void        replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void        verify();
  virtual void        accept(AstVisitor* visitor);

  virtual Expr*       getFirstExpr();

  const char*         getName();
};

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

class ExternBlockStmt : public Stmt {
public:
                      ExternBlockStmt(const char* c_code);

  // Interface to BaseAST
  virtual GenRet      codegen();
  virtual void        verify();
  virtual void        accept(AstVisitor* visitor);

  DECLARE_COPY(ExternBlockStmt);

  // Interface to Expr
  virtual void        replaceChild(Expr* oldAst, Expr* newAst);

  virtual Expr*       getFirstExpr();

  // Local interface
  const char*         c_code;
};


/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

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
