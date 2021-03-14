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

#ifndef _IMPORT_STMT_H_
#define _IMPORT_STMT_H_

#include "stmt.h"

#include <map>
#include <vector>

class ResolveScope;

class ImportStmt final : public VisibilityStmt {
 public:
  ImportStmt(BaseAST* source, bool isPrivate = true);
  ImportStmt(BaseAST* source, const char* rename, bool isPrivate = true);
  ImportStmt(BaseAST* source, std::vector<const char*>* namesList,
             std::map<const char*, const char*>* renamesList,
             bool isPrivate = true);

  DECLARE_COPY(ImportStmt);
  ImportStmt* copyInner(SymbolMap* map) override;

  Expr* getFirstExpr() override;

  void replaceChild(Expr* oldAst, Expr* newAst) override;

  void accept(AstVisitor* visitor) override;

  void verify() override;

  GenRet codegen() override;

  void scopeResolve(ResolveScope* scope);

  BaseAST* getSearchScope() const override;

  std::set<const char*> typeWasNamed(Type* t) const override;

  bool skipSymbolSearch(const char* name) const override;

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

  void typeWasNamed(Type* t, std::set<const char*>* namedTypes) const;

 public:
  std::vector<const char*> unqualified;
};

#endif
