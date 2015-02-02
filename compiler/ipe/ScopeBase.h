/*
 * Copyright 2004-2015 Cray Inc.
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

/************************************ | *************************************
*                                                                           *
* Maintain lexical scoping information for AST definitions to support name  *
* resolution for the IPE.                                                   *
*                                                                           *
* It is important to note that these scopes are similar to but distinct     *
* from the dynamic environments that will be maintained by the IPE's        *
* runtime.                                                                  *
*                                                                           *
************************************* | ************************************/

#ifndef _SCOPE_BASE_H_
#define _SCOPE_BASE_H_

#include <cstdio>
#include <vector>

class BlockStmt;
class Expr;
class ModuleSymbol;
class ScopeModule;
class Symbol;
class UnresolvedSymExpr;
class VisibleSymbol;

class ScopeBase
{
public:
                                  ScopeBase(const ScopeBase* parent);
  virtual                        ~ScopeBase();

  void                            extend(BlockStmt* block);
  void                            describe(bool recursive = false)                      const;

  void                            visibleSymbols(UnresolvedSymExpr*          unres,
                                                 std::vector<VisibleSymbol>& symbols)   const;

protected:
  void                            addSym(Symbol*       symbol);
  void                            addUse(ModuleSymbol* module);


private:
                                  ScopeBase();

  virtual void                    describeHeader(FILE* fp)                              const = 0;

  void                            extend(BlockStmt*                  block,
                                         std::vector<ModuleSymbol*>& visited);

  void                            addUse(Expr*                       expr);

  void                            addUse(ModuleSymbol*               module,
                                         std::vector<ModuleSymbol*>& visited);


  bool                            isConflicted(Symbol*       sym)                       const;
  bool                            isConflicted(ModuleSymbol* mod)                       const;

  bool                            isVisited(ModuleSymbol*               module,
                                            std::vector<ModuleSymbol*>& visited);

  void                            visibleSymbols(UnresolvedSymExpr*          unres,
                                                 std::vector<VisibleSymbol>& symbols,
                                                 int                         distance)  const;

  ModuleSymbol*                   moduleByName(Expr* expr)                              const;

  const ScopeBase*                mParent;
  std::vector<ScopeModule*>       mUsedScopes;
  std::vector<Symbol*>            mSymbols;
};

#endif
