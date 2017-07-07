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

// forall loop statement

class ForallStmt : public Stmt
{
public:
  bool       zippered()       const; // was 'zip' keyword used?
  AList&     inductionVariables();   // DefExprs, one per iterated expr
  AList&     iteratedExpressions();  // SymExprs, CallExprs
  AList&     intentVariables();      // (future) DefExprs of LoopIntentVars
  BlockStmt* loopBody()       const; // the body of the forall loop
  // todo: remove this in favor of intentVariables
  ForallIntents* withClause() const; // forall intents

  ~ForallStmt();

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
                          BlockStmt* body, bool zippered);
  // helpers
  bool isIteratedExpression(Expr* expr);
  int  reduceIntentIdx(Symbol* var); // todo: replace with LoopIntentVars

private:
  bool           fZippered;
  AList          fIterVars;
  AList          fIterExprs;
  AList          fIntentVars;  // may be empty
  BlockStmt*     fLoopBody;    // always present
  ForallIntents* fWith;   // always present; todo: replace with intentVariables

  ForallStmt(bool zippered, BlockStmt* body, ForallIntents* with);
};

ForallStmt* enclosingForallStmt(Expr* expr);

inline bool   ForallStmt::zippered() const       { return fZippered; }
inline AList& ForallStmt::inductionVariables()   { return fIterVars; }
inline AList& ForallStmt::iteratedExpressions()  { return fIterExprs; }
inline AList& ForallStmt::intentVariables()      { return fIntentVars; }
inline BlockStmt*     ForallStmt::loopBody()   const { return fLoopBody; }
inline ForallIntents* ForallStmt::withClause() const { return fWith; }

#endif
