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

#ifndef _WHILE_DO_STMT_H_
#define _WHILE_DO_STMT_H_

#include "WhileStmt.h"

class WhileDoStmt final : public WhileStmt
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
                        ~WhileDoStmt() override = default;
                         WhileDoStmt(Expr* cond, BlockStmt* body)
                           : WhileStmt(cond, body) { }

  DECLARE_COPY(WhileDoStmt);
  WhileDoStmt*           copyInner(SymbolMap* map)                   override;

  bool                   isWhileDoStmt()                             const override
                         { return true; }

  GenRet                 codegen()                                   override;
  void                   accept(AstVisitor* visitor)                 override;

  Expr*                  getFirstExpr()                              override;
  Expr*                  getNextExpr(Expr* expr)                     override;

private:
                         WhileDoStmt(VarSymbol* var,  BlockStmt* body)
                           : WhileStmt(var, body) {
                         }
};

#endif
