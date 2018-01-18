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
  bool       zippered()       const; // was 'zip' keyword used?
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

  virtual void        replaceChild(Expr* oldAst, Expr* newAst);
  virtual void        verify();
  virtual void        accept(AstVisitor* visitor);
  virtual GenRet      codegen();

  virtual Expr*       getFirstChild();
  virtual Expr*       getFirstExpr();
  virtual Expr*       getNextExpr(Expr* expr);

  // for the parser
  static BlockStmt* build(Expr* indices, Expr* iterator, CallExpr* intents,
                          BlockStmt* body, bool zippered = false);

  static ForallStmt* fromForLoop(ForLoop* forLoop);

  // helpers
  Expr* firstIteratedExpr()        const;
  int   numIteratedExprs()         const;
  bool  isIteratedExpression(Expr* expr);
  int   reduceIntentIdx(Symbol* var);
  int   numShadowVars()            const;
  ShadowVarSymbol* getShadowVar(int index) const;

private:
  bool           fZippered;
  AList          fIterVars;
  AList          fIterExprs;
  AList          fShadowVars;  // may be empty
  BlockStmt*     fLoopBody;    // always present
  bool           fFromForLoop; // see comment below

  ForallStmt(bool zippered, BlockStmt* body);

public:
  LabelSymbol*   fContinueLabel;  // update_symbols() needs this
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

// accessor implementations
inline bool   ForallStmt::zippered()       const { return fZippered;   }
inline AList& ForallStmt::inductionVariables()   { return fIterVars;   }
inline AList& ForallStmt::iteratedExpressions()  { return fIterExprs;  }
inline AList& ForallStmt::shadowVariables()      { return fShadowVars; }
inline BlockStmt* ForallStmt::loopBody()   const { return fLoopBody;   }
inline bool ForallStmt::iterCallAlreadyTagged() const { return fFromForLoop; }
inline bool ForallStmt::needToHandleOuterVars() const { return !fFromForLoop; }
inline bool ForallStmt::createdFromForLoop()    const { return fFromForLoop; }

// conveniences
inline Expr* ForallStmt::firstIteratedExpr() const { return fIterExprs.head;  }
inline int   ForallStmt::numIteratedExprs()  const { return fIterExprs.length;}
inline int   ForallStmt::numShadowVars()     const { return fShadowVars.length;}
inline ShadowVarSymbol* ForallStmt::getShadowVar(int index) const
  { return toShadowVarSymbol(toDefExpr(fShadowVars.get(index))->sym); }

#define for_shadow_var_defs(SVD,TEMP,FS)    \
  for_alist(TEMP,(FS)->shadowVariables())   \
    if (DefExpr* SVD = toDefExpr(TEMP))

#define for_shadow_vars(SV,TEMP,FS)                    \
  for_alist(TEMP,(FS)->shadowVariables())              \
    if (DefExpr* SVD = toDefExpr(TEMP))                \
      if (ShadowVarSymbol* SV = toShadowVarSymbol(SVD->sym))

// helpers
ForallStmt* enclosingForallStmt(Expr* expr);

// used for lowering ForallStmt and forall intents
VarSymbol* parIdxVar(const ForallStmt* fs);
VarSymbol* parIdxCopyVar(const ForallStmt* fs);
BlockStmt* userLoop(const ForallStmt* fs);

#endif
