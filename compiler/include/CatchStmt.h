 /*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _CATCH_STMT_H_
#define _CATCH_STMT_H_

#include "stmt.h"

/*
try {
  maybeError();
} catch e: OtherError {
  handleGracefully();
}

CatchStmt is implemented using a BlockStmt, which contains two parts:
  1. filter expression
    - e: OtherError
    - the variable name following catch and the type following the colon
    - DefExpr*
  2. catch body
    - { handleGracefully() }
    - the code inside the block
    - BlockStmt*

This strategy enables other parts of the compiler to use the same
scoping rules without needing to know about CatchStmt
*/

class CatchStmt : public Stmt {

public:
  static CatchStmt* build(BlockStmt* body);
  static CatchStmt* build(Expr* expr, BlockStmt* body);

  CatchStmt(Expr* expr, BlockStmt* body);
  ~CatchStmt();

  DefExpr*   expr() const;
  BlockStmt* body() const;

  void                accept(AstVisitor* visitor);
  void                replaceChild(Expr* old_ast, Expr* new_ast);
  Expr*               getFirstChild();
  Expr*               getFirstExpr();
  void                verify();
  GenRet              codegen();
  DECLARE_COPY(CatchStmt);

  BlockStmt* _body;

private:
  CatchStmt();

};

#endif
