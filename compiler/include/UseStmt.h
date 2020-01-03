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

#ifndef _USE_STMT_H_
#define _USE_STMT_H_

#include "stmt.h"

class ResolveScope;

class UseStmt : public Stmt {
public:
  UseStmt(BaseAST* source, const char* modRename, bool isPrivate);

  UseStmt(BaseAST*                            source,
          const char*                         modRename,
          std::vector<const char*>*           args,
          bool                                exclude,
          std::map<const char*, const char*>* renames,
          bool isPrivate);

  DECLARE_COPY(UseStmt);

  virtual Expr*   getFirstExpr();

  virtual void    replaceChild(Expr* oldAst, Expr* newAst);

  virtual void    accept(AstVisitor* visitor);

  virtual void    verify();

  virtual GenRet  codegen();

  bool            isPlainUse()                                           const;

  bool            hasOnlyList()                                          const;

  bool            hasExceptList()                                        const;

  bool            isARename(const char* name)                            const;
  bool            isARename()                                            const;

  const char*     getRename(const char* name)                            const;
  const char*     getRename()                                            const;

  void            scopeResolve(ResolveScope* scope);

  UseStmt*        applyOuterUse(const UseStmt* outer);

  bool            skipSymbolSearch(const char* name, bool methodCall)    const;

  bool            providesNewSymbols(const UseStmt* other)               const;

  BaseAST*        getSearchScope()                                       const;

  Symbol*         checkIfModuleNameMatches(const char* name);

  void            writeListPredicate(FILE* mFP)                          const;

private:
  bool            isEnum(const Symbol* sym)                              const;

  void            updateEnclosingBlock(ResolveScope* scope,
                                       Symbol*       sym);

  bool            isValid(Expr* expr)                                    const;

  void            validateList();

  void            validateNamed();

  void            validateRenamed();

  void            trackMethods();
  bool            isAllowedMethodName(const char* name, bool methodCall) const;

  bool            matchedNameOrConstructor(const char* name)             const;

  void            noRepeats()                                            const;

public:
  Expr*                              src;
  std::vector<const char*>           named;
  std::map<const char*, const char*> renamed;
  bool isPrivate;

private:
  const char*                        modRename;
  bool                               except;
  std::vector<const char*>           methodsAndFields;
  std::vector<const char*>           functionsToAlwaysCheck;
};

#endif
