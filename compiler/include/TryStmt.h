/*
 * Copyright 2004-2016 Cray Inc.
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
  static BlockStmt*   build(bool tryBang, BlockStmt* body);

                      TryStmt(bool tryBang, BlockStmt* body);
                      ~TryStmt();
  BlockStmt*          getBody() const;

  void                accept(AstVisitor* visitor);
  Expr*               copy(SymbolMap* map = NULL, bool internal = false);
  Expr*               copyInner(SymbolMap* map);
  void                replaceChild(Expr* old_ast, Expr* new_ast);
  Expr*               getFirstChild();
  Expr*               getFirstExpr();
  Expr*               getNextExpr(Expr* expr);
  GenRet              codegen();

private:
  bool       tryBang;
  BlockStmt* body;
  TryStmt();

};

#endif
