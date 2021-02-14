/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "stlUtil.h"

#include <cstdio>
#include <map>
#include <set>

#ifdef HAVE_LLVM

#define FNAME(str) (llvm::Twine(getFunction()->cname)            + \
                    llvm::Twine("_")                             + \
                    llvm::Twine(getFunction()->codegenUniqueNum) + \
                    llvm::Twine(str)                             + \
                    llvm::Twine("_"))

#endif

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

class Stmt : public Expr {
public:
  Stmt(AstTag astTag) : Expr(astTag) {}
 ~Stmt() override = default;

  // Interface to Expr
  bool isStmt() const override { return true; }
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/
class ResolveScope;

// parent base class for UseStmt and ImportStmt
class VisibilityStmt : public Stmt {
 public:
  VisibilityStmt(AstTag astTag);
 ~VisibilityStmt() override = default;

  bool isARename() const;
  bool isARenamedSym(const char* name) const;
  const char* getRename() const;
  const char* getRenamedSym(const char* name) const;

  virtual std::set<const char*> typeWasNamed(Type* t) const = 0;

  virtual bool skipSymbolSearch(const char* name) const = 0;

  virtual BaseAST* getSearchScope() const = 0;

  Symbol* checkIfModuleNameMatches(const char* name);

 protected:
  void updateEnclosingBlock(ResolveScope* scope,
                            Symbol* sym);

  void validateRenamed();
  void noRepeatsInRenamed() const;

public:
  Expr* src;
  bool isPrivate;
  std::map<const char*, const char*> renamed;

protected:
  const char* modRename;

};

#include "UseStmt.h"

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

enum BlockTag {
  // Bits:
  BLOCK_NORMAL      = 0,
  BLOCK_SCOPELESS   = 1<<0, ///< does not introduce a new scope
  BLOCK_TYPE_ONLY   = 1<<1, ///< deleted after type resolution
  BLOCK_EXTERN      = 1<<2, ///< init block for an extern var
  BLOCK_C_FOR_LOOP  = 1<<3, ///< init/test/incr block for a CForLoop

  // Bit masks:
  BLOCK_TYPE        = BLOCK_SCOPELESS | BLOCK_TYPE_ONLY,
  BLOCK_EXTERN_TYPE = BLOCK_EXTERN    | BLOCK_TYPE
};

// Parent class for LoopStmt and various loops as well
class BlockStmt : public Stmt {
public:
                      BlockStmt(Expr*    initBody     = NULL,
                                BlockTag initBlockTag = BLOCK_NORMAL);
                      BlockStmt(BlockTag initBlockTag);

  DECLARE_COPY(BlockStmt);
  BlockStmt* copyInner(SymbolMap* map)             override;

  // Interface to BaseAST
  GenRet codegen()                                 override;
  void   verify()                                  override;
  void   accept(AstVisitor* visitor)               override;

  // Interface to Expr
  void   replaceChild(Expr* oldAst, Expr* newAst)  override;
  Expr*  getFirstExpr()                            override;
  Expr*  getNextExpr(Expr* expr)                   override;

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
  void                flattenAndRemove();

  void                insertAtHead(Expr* ast);
  void                insertAtTail(Expr* ast);
  void                insertAtTailBeforeFlow(Expr* ast);

  void                insertAtHead(AList exprs);
  void                insertAtTail(AList exprs);

  void                insertAtHead(const char* format, ...);
  void                insertAtTail(const char* format, ...);

  // I.E. Not a Loop or an OnStmt or ...
  bool                isRealBlockStmt()                            const;

  bool                isScopeless()                                const;
  bool                isBlockType(PrimitiveTag tag)                const;

  int                 length()                                     const;

  void                useListAdd(ModuleSymbol* mod, bool isPrivate);
  void                useListAdd(VisibilityStmt* stmt);
  bool                useListRemove(ModuleSymbol* mod);
  void                useListClear();

  void                modRefsAdd(ModuleSymbol* mod);
  bool                modRefsRemove(ModuleSymbol* mod);
  void                modRefsClear();

  virtual CallExpr*   blockInfoGet()                               const;
  virtual CallExpr*   blockInfoSet(CallExpr* expr);

  BlockTag            blockTag;
  AList               body;
  CallExpr*           useList;       // module/enum uses for this block
  CallExpr*           modRefs;       // modules referenced directly
  const char*         userLabel;
  CallExpr*           byrefVars;     // task intents - task constructs only

private:
  bool                canFlattenChapelStmt(const BlockStmt* stmt)  const;

  CallExpr*           blockInfo;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

class CondStmt final : public Stmt {
public:
  CondStmt(Expr*    iCondExpr,
           BaseAST* iThenStmt,
           BaseAST* iElseStmt = NULL,
           bool     isIfExpr = false);

  DECLARE_COPY(CondStmt);
  CondStmt*  copyInner(SymbolMap* map)                  override;

  GenRet     codegen()                                  override;
  void       replaceChild(Expr* oldAst, Expr* newAst)   override;
  void       verify()                                   override;
  void       accept(AstVisitor* visitor)                override;

  Expr*      getFirstExpr()                             override;
  Expr*      getNextExpr(Expr* expr)                    override;

  CallExpr*  foldConstantCondition(bool addEndOfStatement);

  Expr*      condExpr;
  BlockStmt* thenStmt;
  BlockStmt* elseStmt;

  bool       isIfExpr() const;

private:
  bool       fIsIfExpr;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

enum GotoTag {
  GOTO_NORMAL,
  GOTO_BREAK,
  GOTO_CONTINUE,
  GOTO_RETURN,
  GOTO_GETITER_END,
  GOTO_ITER_RESUME,
  GOTO_ITER_END,
  GOTO_ERROR_HANDLING,
  GOTO_BREAK_ERROR_HANDLING,
  GOTO_ERROR_HANDLING_RETURN,
};


class GotoStmt final : public Stmt {
 public:
  GotoTag gotoTag;
  Expr* label;

  GotoStmt(GotoTag init_gotoTag, const char* init_label);
  GotoStmt(GotoTag init_gotoTag, Symbol* init_label);
  GotoStmt(GotoTag init_gotoTag, Expr* init_label);

  GenRet codegen()                                   override;

  DECLARE_COPY(GotoStmt);
  GotoStmt* copyInner(SymbolMap* map)                override;

  void   replaceChild(Expr* old_ast, Expr* new_ast)  override;
  void   verify()                                    override;
  void   accept(AstVisitor* visitor)                 override;
  Expr*  getFirstExpr()                              override;

  const char*  getName();

  bool         isGotoReturn()                           const;
  LabelSymbol* gotoTarget()                             const;
};

/************************************* | **************************************
*                                                                             *
* An ImplementsStmt states that an interface is implemented by given type(s). *
* Syntactically it is similar to an interface constraint (see IfcConstraint), *
* and also allows a block statement, ex.                                      *
*   implements InterfaceName(actualType...);                                  *
*   actualType implements InterfaceName;                                      *
*   actualType implements InterfaceName { proc defaultImplementation... }     *
*                                                                             *
************************************** | *************************************/

class ImplementsStmt final : public Stmt {
public:
  static ImplementsStmt* build(const char* name,
                               CallExpr* actuals,
                               BlockStmt* body);
  ImplementsStmt(IfcConstraint* con, BlockStmt* body);

  DECLARE_COPY(ImplementsStmt);
  ImplementsStmt* copyInner(SymbolMap* map)         override;
  GenRet codegen()                                  override;
  void   verify()                                   override;
  void   accept(AstVisitor* visitor)                override;

  void   replaceChild(Expr* oldAst, Expr* newAst)   override;
  Expr*  getFirstExpr()                             override;
  Expr*  getNextExpr(Expr* expr)                    override;

  InterfaceSymbol* ifcSymbol()  const { return iConstraint->ifcSymbol(); }
  int              numActuals() const { return iConstraint->numActuals(); }

  // the constraint being implemented, always non-null
  IfcConstraint* iConstraint;

  // (possibly empty) body of this statement, always non-null
  BlockStmt*     implBody;

  // for each associated type or required function in the interface,
  // the map points to its implementation for this ImplementsStmt
  SymbolMap      witnesses;
};

// support for implements wrapper functions
const char*     implementsStmtWrapperName(InterfaceSymbol* isym);
const char*     interfaceNameForWrapperFn(FnSymbol* fn);
ImplementsStmt* implementsStmtForWrapperFn(FnSymbol* wrapFn, bool& isSuccess);
FnSymbol*       wrapperFnForImplementsStmt(ImplementsStmt* istm);

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

class ExternBlockStmt final : public Stmt {
public:
  ExternBlockStmt(const char* c_code);

  // Interface to BaseAST
  GenRet codegen()                                 override;
  void   verify()                                  override;
  void   accept(AstVisitor* visitor)               override;

  DECLARE_COPY(ExternBlockStmt);
  ExternBlockStmt* copyInner(SymbolMap* map)       override;


  // Interface to Expr
  void   replaceChild(Expr* oldAst, Expr* newAst)  override;
  Expr*  getFirstExpr()                            override;

  // Local interface
  const char* c_code;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

class ForwardingStmt final : public Stmt {
public:
                      ForwardingStmt(DefExpr* toFnDef);
                      ForwardingStmt(DefExpr* toFnDef,
                                   std::set<const char*>* args,
                                   bool exclude,
                                   std::map<const char*, const char*>* renames);

  // Interface to BaseAST
  GenRet              codegen()                                 override;
  void                verify()                                  override;
  void                accept(AstVisitor* visitor)               override;

  DECLARE_COPY(ForwardingStmt);
  ForwardingStmt* copyInner(SymbolMap* map)                     override;

  // Interface to Expr
  void                replaceChild(Expr* oldAst, Expr* newAst)  override;
  Expr*               getFirstExpr()                            override;

  // forwarding function - contains forwarding expression; used during parsing
  DefExpr*            toFnDef;
  // name of forwarding function; used before, during resolution
  const char*         fnReturningForwarding;
  // stores the type returned by the forwarding function
  // (i.e. the type of the expression to forward to).
  // Used during resolution to avoid repeated work.
  Type*               type;
  // Contains a function that resolution can use to store some expressions
  // it computes. This function should remain in the tree for proper
  // scoping comparisons during resolution, but isn't needed after that.
  FnSymbol*           scratchFn;

  // The names of symbols from an 'except' or 'only' list
  std::set<const char *> named;
  // Map of newName: oldName
  std::map<const char*, const char*> renamed;
  // Is 'named' an 'except' list? (vs. 'only' list)
  bool except;
};


/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

extern Vec<LabelSymbol*>         removedIterResumeLabels;
extern Map<GotoStmt*, GotoStmt*> copiedIterResumeGotos;

const char*  gotoTagToString(GotoTag gotoTag);
CondStmt*    isConditionalInCondStmt(Expr* expr);
Expr*        skip_cond_test(Expr* expr);

// Probably belongs in Expr; doesn't really mean Stmt, but rather
// statement-level expression.
void         codegenStmt(Expr* stmt);

// Extract (e.toGotoStmt)->(label.toSymExpr)->var and var->->iterResumeGoto,
// if possible; NULL otherwise.
LabelSymbol* getGotoLabelSymbol(GotoStmt* gs);
GotoStmt*    getGotoLabelsIterResumeGoto(GotoStmt* gs);

void         removeDeadIterResumeGotos();
void         verifyRemovedIterResumeGotos();
void         verifyCopiedIterResumeGotos();

#endif
