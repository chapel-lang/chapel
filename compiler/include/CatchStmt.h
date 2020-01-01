 /*
 * Copyright 2004-2020 Cray Inc.
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
  static CatchStmt* build(DefExpr* def, BlockStmt* body);
  static CatchStmt* build(const char* name, Expr* type, BlockStmt* body);
  static CatchStmt* build(const char* name, BlockStmt* body);
  static CatchStmt* build(BlockStmt* body);

  CatchStmt(const char* name, Expr* type, BlockStmt* body);
  ~CatchStmt();

  const char* name() const;
  Expr*       type() const;

  // body() returns the body of the catch block, including the
  // conditional testing its filter (e.g., `e: MyError`) if there is
  // one; bodyWithoutTest() just returns the code block that follows
  // the test without that conditional

  BlockStmt*  body() const;
  BlockStmt*  bodyWithoutTest() const;

  bool        isCatchall() const;

  void                accept(AstVisitor* visitor);
  void                replaceChild(Expr* old_ast, Expr* new_ast);
  Expr*               getFirstExpr();
  Expr*               getNextExpr(Expr* expr);
  void                verify();
  void                cleanup();

  GenRet              codegen();
  DECLARE_COPY(CatchStmt);

  const char* _name;
  Expr*       _type;
  BlockStmt*  _body;

private:
  CatchStmt();

};

#endif
