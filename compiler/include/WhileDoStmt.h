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

#ifndef _WHILE_DO_STMT_H_
#define _WHILE_DO_STMT_H_

#include "WhileStmt.h"

class WhileDoStmt : public WhileStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      build(Expr* cond, BlockStmt* body);

private:
  static bool            isPrimitiveCForLoop(Expr* cond);


  //
  // Instance interface
  //
public:
                         WhileDoStmt(Expr*      cond, BlockStmt* body);
  virtual               ~WhileDoStmt();

  virtual WhileDoStmt*   copy(SymbolMap* map = NULL, bool internal = false);

  virtual bool           isWhileDoStmt()                              const;

  virtual GenRet         codegen();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

private:
                         WhileDoStmt();

                         WhileDoStmt(VarSymbol* var,  BlockStmt* body);
};

#endif

