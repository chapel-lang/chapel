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


//////////////////////////
    // a forall intent //
   //////////////////////////

class ForallIntent : public Expr 
{
public:
  TFITag intent()      const;
  Expr*  variable()    const;  // non-NULL always
  Expr*  reduceExpr()  const;  // non-NULL only for reduce intents
  bool   isReduce()    const;

  ForallIntent(TFITag intent, Expr* var, Expr* reduceExpr = NULL);

  // Expr interface
  DECLARE_COPY(ForallIntent);
  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);
  virtual Expr*   getFirstChild();
  virtual Expr*   getFirstExpr();
  virtual Expr*   getNextExpr(Expr* expr);
  virtual void    verify();
  virtual GenRet  codegen();
  virtual void    accept(AstVisitor* visitor);

private:
  TFITag fiIntent;
  Expr*  fiVar;
  Expr*  riSpec;
};

// accessor implementations
inline TFITag ForallIntent::intent()     const { return fiIntent; }
inline Expr*  ForallIntent::variable()   const { return fiVar; }
inline Expr*  ForallIntent::reduceExpr() const { return riSpec; }
inline bool   ForallIntent::isReduce()   const { return fiIntent==TFI_REDUCE; }


///////////////////////////////////
    // forall loop statement //
///////////////////////////////////

class ForallStmt : public Stmt
{
public:
  bool       zippered()       const; // was 'zip' keyword used?
  AList&     inductionVariables();   // DefExprs, one per iterated expr
  AList&     iteratedExpressions();  // SymExprs, CallExprs
  AList&     intentVariables();      // (future) DefExprs of LoopIntentVars
  BlockStmt* loopBody()       const; // the body of the forall loop
  // todo: remove 'forallIntents' in favor of intentVariables()
  AList&     forallIntents();        // forall intents

  DECLARE_COPY(ForallStmt);

  virtual void        replaceChild(Expr* oldAst, Expr* newAst);
  virtual void        verify();
  virtual void        accept(AstVisitor* visitor);
  virtual GenRet      codegen();

  virtual Expr*       getFirstChild();
  virtual Expr*       getFirstExpr();
  virtual Expr*       getNextExpr(Expr* expr);

  // for the parser
  static BlockStmt* build(Expr* indices, Expr* iterator, ForallIntents* fi,
                          BlockStmt* body, bool zippered = false);
  // helpers
  Expr* firstIteratedExpr()        const;
  int   numIteratedExprs()         const;
  bool  isIteratedExpression(Expr* expr);
  int   reduceIntentIdx(Symbol* var); // todo: replace with ForallIntents
  int           numForallIntents()         const;
  ForallIntent* getForallIntent(int index) const; //todo use LoopIntentVars

private:
  bool           fZippered;
  AList          fIterVars;
  AList          fIterExprs;
  AList          fIntentVars;  // may be empty
  AList          fFIntents;    // todo: use fIntentVars instead
  BlockStmt*     fLoopBody;    // always present

  ForallStmt(bool zippered, BlockStmt* body);
};

// accessor implementations
inline bool   ForallStmt::zippered()       const { return fZippered;   }
inline AList& ForallStmt::inductionVariables()   { return fIterVars;   }
inline AList& ForallStmt::iteratedExpressions()  { return fIterExprs;  }
inline AList& ForallStmt::intentVariables()      { return fIntentVars; }
inline AList& ForallStmt::forallIntents()        { return fFIntents;   }
inline BlockStmt* ForallStmt::loopBody()   const { return fLoopBody;   }

// conveniences
inline Expr* ForallStmt::firstIteratedExpr() const { return fIterExprs.head;  }
inline int   ForallStmt::numIteratedExprs()  const { return fIterExprs.length;}
inline int   ForallStmt::numForallIntents()  const { return fFIntents.length; }
inline ForallIntent* ForallStmt::getForallIntent(int index) const
  { return toForallIntent(fFIntents.get(index)); }

#define for_forall_intents(FI,TEMP,FS)           \
  for_alist(TEMP,(FS)->forallIntents())          \
    if (ForallIntent* FI = toForallIntent(TEMP))

// helpers
ForallStmt* enclosingForallStmt(Expr* expr);

// used for lowering ForallStmt and forall intents
VarSymbol* parIdxVar(const ForallStmt* fs);
VarSymbol* parIdxCopyVar(const ForallStmt* fs);
BlockStmt* userLoop(const ForallStmt* fs);

#endif
