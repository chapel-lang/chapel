/*
 * Copyright 2004-2017 Cray Inc.
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
  AList&     iteratedExpressions();  // SymExprs, CallExprs
  AList&     intentVariables();      // DefExprs of LoopIntentVars
  BlockStmt* loopBody()       const; // the body of the forall loop

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
  // helpers
  Expr* firstIteratedExpr()        const;
  int   numIteratedExprs()         const;
  bool  isIteratedExpression(Expr* expr);
  int   reduceIntentIdx(Symbol* var);
  int   numIntentVars()            const;
  ShadowVarSymbol* getForallIntent(int index); //todo rename

private:
  bool           fZippered;
  AList          fIterVars;
  AList          fIterExprs;
  AList          fIntentVars;  // may be empty
  BlockStmt*     fLoopBody;    // always present

  ForallStmt(bool zippered, BlockStmt* body);
};

// accessor implementations
inline bool   ForallStmt::zippered()       const { return fZippered;   }
inline AList& ForallStmt::inductionVariables()   { return fIterVars;   }
inline AList& ForallStmt::iteratedExpressions()  { return fIterExprs;  }
inline AList& ForallStmt::intentVariables()      { return fIntentVars; }
inline BlockStmt* ForallStmt::loopBody()   const { return fLoopBody;   }

// conveniences
inline Expr* ForallStmt::firstIteratedExpr() const { return fIterExprs.head;  }
inline int   ForallStmt::numIteratedExprs()  const { return fIterExprs.length;}
inline int   ForallStmt::numIntentVars()     const { return fIntentVars.length;}
inline ShadowVarSymbol* ForallStmt::getForallIntent(int index)
  { return toShadowVarSymbol(toDefExpr(fIntentVars.get(index))->sym); }

#define for_shadow_var_defs(SVD,TEMP,FS)    \
  for_alist(TEMP,(FS)->intentVariables())   \
    if (DefExpr* SVD = toDefExpr(TEMP))

#define for_shadow_vars(SV,TEMP,FS)                    \
  for_alist(TEMP,(FS)->intentVariables())              \
    if (DefExpr* SVD = toDefExpr(TEMP))                \
      if (ShadowVarSymbol* SV = toShadowVarSymbol(SVD->sym))

// helpers
ForallStmt* enclosingForallStmt(Expr* expr);

// used for lowering ForallStmt and forall intents
VarSymbol* parIdxVar(const ForallStmt* fs);
VarSymbol* parIdxCopyVar(const ForallStmt* fs);
BlockStmt* userLoop(const ForallStmt* fs);

#endif
