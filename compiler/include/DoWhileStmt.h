/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _DO_WHILE_STMT_H_
#define _DO_WHILE_STMT_H_

#include "WhileStmt.h"

class DoWhileStmt : public WhileStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      build(Expr* cond, BlockStmt* body);


  //
  // Instance interface
  //
public:
  virtual               ~DoWhileStmt();

  virtual DoWhileStmt*   copy(SymbolMap* map = NULL, bool internal = false);

  virtual bool           isDoWhileStmt()                              const;

  virtual GenRet         codegen();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

private:
                         DoWhileStmt();

                         DoWhileStmt(Expr*      cond, BlockStmt* body);
                         DoWhileStmt(VarSymbol* var,  BlockStmt* body);
};

#endif

