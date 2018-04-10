/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _FORALL_STMT_H_
#define _FORALL_STMT_H_

#include "stmt.h"


///////////////////////////////////
    // forall loop statement //
///////////////////////////////////

class ForallStmt : public Stmt
{
public:
  bool       zippered()       const; // 'zip' keyword used and >1 index var
  AList&     inductionVariables();   // DefExprs, one per iterated expr
  AList&     iteratedExpressions();  // SymExprs, one per iterated expr
  AList&     shadowVariables();      // DefExprs of ShadowVarSymbols
  BlockStmt* loopBody()       const; // the body of the forall loop
  LabelSymbol* continueLabel();      // create it if not already

  // when originating from a ForLoop
  bool       createdFromForLoop()    const;  // is converted from a for-loop
  bool       iterCallAlreadyTagged() const;  // already has 'tag' actual
  bool       needToHandleOuterVars() const;  // yes, convert to shadow vars

  DECLARE_COPY(ForallStmt);

  virtual void        verify();
  virtual void        accept(AstVisitor* visitor);
  virtual GenRet      codegen();

  virtual void        replaceChild(Expr* oldAst, Expr* newAst);
  virtual Expr*       getFirstExpr();
  virtual Expr*       getNextExpr(Expr* expr);

  // for the parser
  static BlockStmt* build(Expr* indices, Expr* iterator, CallExpr* intents,
                          BlockStmt* body, bool zippered = false);

  static ForallStmt* fromForLoop(ForLoop* forLoop);

  // helpers

  int numInductionVars()  const;
  int numIteratedExprs()  const;
  int numShadowVars()     const;

  Expr* firstIteratedExpr() const;

  int   reduceIntentIdx(Symbol* var);
  void  setNotZippered();

private:
  bool           fZippered;
  AList          fIterVars;
  AList          fIterExprs;
  AList          fShadowVars;  // may be empty
  BlockStmt*     fLoopBody;    // always present
  bool           fFromForLoop; // see comment below

  ForallStmt(bool zippered, BlockStmt* body);

public:
  LabelSymbol*   fContinueLabel;     // update_symbols() needs the labels
  LabelSymbol*   fErrorHandlerLabel;
  bool           fFromResolvedForLoop;

  // for recursive iterators during lowerIterators
  DefExpr*       fRecIterIRdef;
  DefExpr*       fRecIterICdef;
  CallExpr*      fRecIterGetIterator;
  CallExpr*      fRecIterFreeIterator;
};

/* fFromForLoop and its accessors

These support handling of some ForLoops by converting them to ForallStmts.
They cause skipping certain actions for these "conversion" ForallStmt nodes.

Why not just have a single accessor to fFromForLoop? This is to emphasize
that the three accessors check different properties. These properties could
potentially be independent of each other.

As fFromForLoop is currently local to implementForallIntents, we may be able
to replace fFromForLoop with a HashSet. If so, we need to ensure that the
set membership is propagated through cloning, if applicable.
*/

/// accessor implementations ///

inline bool   ForallStmt::zippered()         const { return fZippered;   }
inline AList& ForallStmt::inductionVariables()     { return fIterVars;   }
inline AList& ForallStmt::iteratedExpressions()    { return fIterExprs;  }
inline AList& ForallStmt::shadowVariables()        { return fShadowVars; }
inline BlockStmt* ForallStmt::loopBody()     const { return fLoopBody;   }

inline bool ForallStmt::iterCallAlreadyTagged() const { return  fFromForLoop; }
inline bool ForallStmt::needToHandleOuterVars() const { return !fFromForLoop; }
inline bool ForallStmt::createdFromForLoop()    const { return  fFromForLoop; }

/// conveniences ///

inline int   ForallStmt::numInductionVars()  const { return fIterVars.length; }
inline int   ForallStmt::numIteratedExprs()  const { return fIterExprs.length;}
inline int   ForallStmt::numShadowVars()     const { return fShadowVars.length;}
inline Expr* ForallStmt::firstIteratedExpr() const { return fIterExprs.head;  }

#define for_shadow_var_defs(SVD,TEMP,FS)    \
  for_alist(TEMP,(FS)->shadowVariables())   \
    if (DefExpr* SVD = toDefExpr(TEMP))

#define for_shadow_vars(SV,TEMP,FS)                    \
  for_alist(TEMP,(FS)->shadowVariables())              \
    if (DefExpr* SVD = toDefExpr(TEMP))                \
      if (ShadowVarSymbol* SV = toShadowVarSymbol(SVD->sym))

/// helpers ///

bool        isForallIterExpr(Expr* expr);
bool        isForallRecIterHelper(Expr* expr);
bool        isForallLoopBody(Expr* expr);
ForallStmt* enclosingForallStmt(Expr* expr);
VarSymbol*  parIdxVar(ForallStmt* fs);

/// done ///

#endif
