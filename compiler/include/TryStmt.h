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

#ifndef _TRY_STMT_H_
#define _TRY_STMT_H_

#include "stmt.h"

class TryStmt : public Stmt
{

public:

  static BlockStmt*   build(bool tryBang, Expr*      expr);
  static BlockStmt*   build(bool tryBang, BlockStmt* body, BlockStmt* catches,
                            bool isSyncTry = false);
  static BlockStmt*   buildWithCatchall(BlockStmt* body, BlockStmt* onErr);

  TryStmt(bool tryBang, BlockStmt* body, BlockStmt* catches,
          bool isSyncTry = false);
                      ~TryStmt();
  BlockStmt*          body() const;
  bool                tryBang() const;
  bool                isSyncTry() const;

  void                accept(AstVisitor* visitor);
  void                replaceChild(Expr* old_ast, Expr* new_ast);
  Expr*               getFirstExpr();
  Expr*               getNextExpr(Expr* expr);
  void                verify();
  GenRet              codegen();
  DECLARE_COPY(TryStmt);

  // intended to be private, but astutil.h macros need public access
  BlockStmt*          _body;
  AList               _catches;

private:
  bool                _tryBang;
  bool                _isSyncTry;

  static BlockStmt*   buildChplStmt(Expr* expr);
                      TryStmt();

};

#endif
