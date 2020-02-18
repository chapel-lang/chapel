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

#ifndef _IMPORT_STMT_H_
#define _IMPORT_STMT_H_

#include "stmt.h"

class ResolveScope;

class ImportStmt: public VisibilityStmt {
 public:
  ImportStmt(BaseAST* source);

  DECLARE_COPY(ImportStmt);

  virtual Expr* getFirstExpr();

  virtual void replaceChild(Expr* oldAst, Expr* newAst);

  virtual void accept(AstVisitor* visitor);

  virtual void verify();

  virtual GenRet codegen();

  void scopeResolve(ResolveScope* scope);

  BaseAST* getSearchScope() const;

  Symbol* checkIfModuleNameMatches(const char* name);

};

#endif
