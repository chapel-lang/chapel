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

#ifndef _DEFER_STMT_H_
#define _DEFER_STMT_H_

#include "stmt.h"

class DeferStmt final : public Stmt
{

public:

  BlockStmt*          body() const;

  // To be invoked by the parser; returns a BlockStmt
  // containing the DeferStmt.
  static BlockStmt*   build(BlockStmt* body);

                      DeferStmt(BlockStmt* body);
                      DeferStmt(CallExpr* call);
                     ~DeferStmt() override = default;

  void                accept(AstVisitor* visitor) override;
  void                replaceChild(Expr* old_ast, Expr* new_ast) override;
  Expr*               getFirstExpr() override;
  Expr*               getNextExpr(Expr* expr) override;
  void                verify() override;
  GenRet              codegen() override;
  DECLARE_COPY(DeferStmt);
  DeferStmt* copyInner(SymbolMap* map) override;

private:
  static BlockStmt*   buildChplStmt(Expr* expr);
                      DeferStmt();

  BlockStmt*          _body;
};

void checkDefersAfterParsing();

#endif
