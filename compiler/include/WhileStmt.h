/*
 * Copyright 2004-2014 Cray Inc.
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

#include "stmt.h"

class WhileStmt : public BlockStmt
{
public:
  CallExpr*              condExprGet()                                const;
  CallExpr*              condExprSet(CallExpr* info);

protected:
                         WhileStmt(BlockStmt* initBody);
  virtual               ~WhileStmt();

  void                   copyShare(const WhileStmt& ref,
                                   SymbolMap*       mapRef,
                                   bool             internal);

  virtual void           verify();

  virtual bool           isLoop()                                     const;
  virtual bool           isWhileStmt()                                const;

  virtual void           checkConstLoops();

  virtual bool           deadBlockCleanup();

private:
                         WhileStmt();

  void                   checkWhileLoopCondition(Expr* condExp);
  bool                   symDeclaredInBlock(Symbol* condSym);
  void                   checkConstWhileLoop();
  bool                   loopBodyHasExits();
  SymExpr*               getWhileCondDef(CallExpr* info, VarSymbol* condSym);
};

#endif

