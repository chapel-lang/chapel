/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _WHILE_STMT_H_
#define _WHILE_STMT_H_

#include "LoopStmt.h"

class WhileStmt : public LoopStmt
{
public:
  Expr*                  condExprGet()                                const;
  SymExpr*               condExprForTmpVariableGet()                  const;

protected:
                         WhileStmt(Expr*      sym,
                                   BlockStmt* initBody);

                         WhileStmt(VarSymbol* sym,
                                   BlockStmt* initBody);

  virtual               ~WhileStmt();

  void                   copyShare(const WhileStmt& ref,
                                   SymbolMap*       mapRef,
                                   bool             internal);

  // Interface to BaseAST
  virtual void           verify();

  // Interface to Expr
  virtual void           replaceChild(Expr* oldAst, Expr* newAst);

  // New interface
  virtual bool           isWhileStmt()                                const;

  virtual void           checkConstLoops();

  virtual bool           deadBlockCleanup();

  virtual CallExpr*      blockInfoGet()                               const;
  virtual CallExpr*      blockInfoSet(CallExpr* expr);

private:
                         WhileStmt();

  // Helper functions for checkConstLoops()
  SymExpr*               getWhileCondDef(VarSymbol* condSym);
  void                   checkWhileLoopCondition(Expr* condExp);
  bool                   symDeclaredInBlock(Symbol* condSym);
  void                   checkConstWhileLoop();
  bool                   loopBodyHasExits();

  Expr*                  mCondExpr;
};

#endif
