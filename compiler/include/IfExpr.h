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

#ifndef _IF_EXPR_H_
#define _IF_EXPR_H_

#include "baseAST.h"
#include "expr.h"

#include <ostream>


class IfExpr final : public Expr {
public:

  IfExpr(Expr* condition, Expr* thenExpr, Expr* elseExpr);

  Expr*      getCondition();
  BlockStmt* getThenStmt();
  BlockStmt* getElseStmt();

  void verify() override;

  DECLARE_COPY(IfExpr);
  IfExpr* copyInner(SymbolMap* map) override;

  Expr*         getFirstExpr() override;
  void          replaceChild(Expr* old_ast, Expr* new_ast) override;
  QualifiedType qualType() override;

  void   accept(AstVisitor* visitor) override;
  GenRet codegen() override;
  void   prettyPrint(std::ostream* o) override;

private:
  Expr* condition;
  BlockStmt* thenStmt;
  BlockStmt* elseStmt;
};

// This function works only before conditional statement folding
bool isLoweredIfExprBlock(BlockStmt* block);

#endif
