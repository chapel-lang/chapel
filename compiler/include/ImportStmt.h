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

#ifndef _IMPORT_STMT_H_
#define _IMPORT_STMT_H_

#include "stmt.h"

#include <map>
#include <vector>

class ResolveScope;

class ImportStmt: public VisibilityStmt {
 public:
  ImportStmt(BaseAST* source, bool isPrivate = true);
  ImportStmt(BaseAST* source, const char* rename, bool isPrivate = true);
  ImportStmt(BaseAST* source, std::vector<const char*>* namesList,
             std::map<const char*, const char*>* renamesList,
             bool isPrivate = true);

  DECLARE_COPY(ImportStmt);

  virtual Expr* getFirstExpr();

  virtual void replaceChild(Expr* oldAst, Expr* newAst);

  virtual void accept(AstVisitor* visitor);

  virtual void verify();

  virtual GenRet codegen();

  void scopeResolve(ResolveScope* scope);

  virtual BaseAST* getSearchScope() const;

  bool skipSymbolSearch(const char* name) const;

  bool providesQualifiedAccess() const;
  bool providesUnqualifiedAccess() const;

  ImportStmt* applyOuterUse(const UseStmt* outer);
  ImportStmt* applyOuterImport(const ImportStmt* outer);

  bool providesNewSymbols(const UseStmt* other) const;
  bool providesNewSymbols(const ImportStmt* other) const;

 private:
  bool checkValid(Expr* expr) const;

  void validateList();
  void validateUnqualified();
  void noRepeats() const;

 public:
  std::vector<const char*> unqualified;
};

#endif
