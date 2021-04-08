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

#ifndef _WHILE_STMT_H_
#define _WHILE_STMT_H_

#include "LoopStmt.h"

// parent of WhileDoStmt and DoWhileStmt
class WhileStmt : public LoopStmt
{
public:
  Expr*                  condExprGet()                                const;
  SymExpr*               condExprForTmpVariableGet()                  const;

protected:
  WhileStmt(Expr*      sym, BlockStmt* initBody);
  WhileStmt(VarSymbol* sym, BlockStmt* initBody);
 ~WhileStmt() override = default;

  void      copyInnerShare(const WhileStmt& ref, SymbolMap* map);

  // Interface to BaseAST
  void      verify()                                 override;

  // Interface to Expr
  void      replaceChild(Expr* oldAst, Expr* newAst) override;

  // New interface
  bool      isWhileStmt()                      const override;
  void      checkConstLoops()                        override;
  bool      deadBlockCleanup()                       override;
  CallExpr* blockInfoGet()                     const override;
  CallExpr* blockInfoSet(CallExpr* expr)             override;

private:

  // Helper functions for checkConstLoops()
  SymExpr*               getWhileCondDef(VarSymbol* condSym);
  void                   checkWhileLoopCondition(Expr* condExp);
  bool                   symDeclaredInBlock(Symbol* condSym);
  void                   checkConstWhileLoop();
  bool                   loopBodyHasExits();

  Expr*                  mCondExpr;
};

#endif
