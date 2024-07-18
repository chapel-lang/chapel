/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef _LOOP_EXPR_H_
#define _LOOP_EXPR_H_

#include "stmt.h"

enum LoopExprType {
  FOR_EXPR,
  FOREACH_EXPR,
  FORALL_EXPR
};

class LoopExpr final : public Expr {
public:
  Expr* indices;       // DefExpr for index or
                       // calls to _build_tuple containing DefExprs

  Expr* iteratorExpr;  // Expr or CallExpr to _build_tuple in zippered case
  Expr* cond;          // filtering condition or NULL if none
  BlockStmt* loopBody;

  LoopExprType type;

  // 'true' if the iteratorExpr is zippered
  bool zippered;

  // 'true' for forall-exprs with square brackets, like:
  //     [i in 1..10] i;
  // 'false' for forall-exprs or for-exprs that use keywords, like:
  //     forall i in 1..10 do i;
  //     for    i in 1..10 do i;
  bool maybeArrayType;

  LoopExpr(Expr* indices,
           Expr* iteratorExpr,
           Expr* cond,
           Expr* loopBody,
           LoopExprType type,
           bool zippered,
           bool maybeArrayType);
  LoopExpr(LoopExprType type, bool zippered, bool maybeArrayType);

  DECLARE_COPY(LoopExpr);
  LoopExpr* copyInner(SymbolMap* map)                 override;

  void    replaceChild(Expr* old_ast, Expr* new_ast)  override;
  void    verify()                                    override;
  void    accept(AstVisitor* visitor)                 override;
  GenRet  codegen()                                   override;
  Expr*   getFirstExpr()                              override;
};

/**

 Check if a given symbol needs to be converted into a formal when its
 containing expression is lifted into a function. For instance,
 in the following loop expression:

     foreach i in 1..10 do f(x, y, z)

 If we are trying to create an iterator proc to implement that loop, we may
 need to capture 'x', 'y', 'z' and potentially even 'f' (if 'f' were an
 object with a call operator, and not an actual function) as outer variables.

     iter foreachLoop(x, y, z, f) { ... }

 There are some exceptions for what gets captured (global variables, modules,
 etc.) that are encoded by this function.

*/
bool considerForOuter(Symbol* sym);
/**
  Create a new function ArgSymbol from a given outer variable when lifting
  an expression into a function. See the comment on considerForOuter
  for an example of the sort of transformations this supports.
 */
ArgSymbol* newOuterVarArg(Symbol* sym);
void normalizeGeneratedLoweringFn(FnSymbol* fn);

void lowerLoopExprs(BaseAST* ast);

#endif
