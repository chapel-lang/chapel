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

#ifndef _USE_STMT_H_
#define _USE_STMT_H_

#include "stmt.h"

class ResolveScope;

class UseStmt final : public VisibilityStmt {
public:
  UseStmt(BaseAST* source, const char* modRename, bool isPrivate);

  UseStmt(BaseAST*                            source,
          const char*                         modRename,
          std::vector<const char*>*           args,
          bool                                exclude,
          std::map<const char*, const char*>* renames,
          bool isPrivate);

  DECLARE_COPY(UseStmt);
  UseStmt* copyInner(SymbolMap* map) override;

  Expr*   getFirstExpr() override;

  void    replaceChild(Expr* oldAst, Expr* newAst) override;

  void    accept(AstVisitor* visitor) override;

  void    verify() override;

  GenRet  codegen() override;

  bool            isPlainUse()                                           const;

  bool            hasOnlyList()                                          const;

  bool            hasExceptList()                                        const;

  void            scopeResolve(ResolveScope* scope);

  UseStmt*        applyOuterUse(const UseStmt* outer);
  ImportStmt*     applyOuterImport(const ImportStmt* outer);

  std::set<const char*> typeWasNamed(Type* t) const override;

  bool            skipSymbolSearch(const char* name)            const override;

  bool            providesNewSymbols(const UseStmt* other)               const;
  bool            providesNewSymbols(const ImportStmt* other)            const;

  BaseAST*        getSearchScope()                              const override;

  void            writeListPredicate(FILE* mFP)                          const;

  bool            canReexport;

private:
  bool            isEnum(const Symbol* sym)                              const;

  bool            isValid(Expr* expr)                                    const;

  void            validateList();

  void            validateNamed();

  bool            matchedNameOrRename(const char* name)             const;

  void            noRepeats()                                            const;

  void typeWasNamed(Type* t, std::set<const char*>* namedTypes) const;

public:
  std::vector<const char*>           named;

private:
  bool                               except;
};

#endif
