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

#ifndef _RESOLVE_SCOPE_H_
#define _RESOLVE_SCOPE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

class BaseAST;
class BlockStmt;
class CallExpr;
class DefExpr;
class Expr;
class FnSymbol;
class LoopExpr;
class ForallStmt;
class ModuleSymbol;
class Symbol;
class TypeSymbol;
class UnresolvedSymExpr;
class UseStmt;
class VisibilityStmt;

// A preliminary version of a class to support the scope resolve pass
// This is currently a thin wrapping over a previous typedef + functions
class ResolveScope {
public:
  static ResolveScope*  getRootModule();

  static ResolveScope*  findOrCreateScopeFor(DefExpr* def);

  static ResolveScope*  getScopeFor(BaseAST* ast);

  static void           destroyAstMap();

public:
                        ResolveScope(ModuleSymbol*       modSym,
                                     const ResolveScope* parent);

                        ResolveScope(BaseAST*            ast,
                                     const ResolveScope* parent);

  std::string           name()                                           const;

  int                   depth()                                          const;

  int                   numBindings()                                    const;

  BlockStmt*            asBlockStmt()                                    const;

  ModuleSymbol*         enclosingModule()                                const;

  bool                  extend(Symbol*        sym, bool isTopLevel=false);

  bool                  extend(VisibilityStmt* stmt);

  Symbol*               lookup(Expr*       expr, bool isUse=false)       const;

  Symbol*               lookupNameLocally(const char* name,
                                          bool isUse=false)              const;

  // Support for UseStmt with only/except
  // Has the potential to return multiple fields
  // Includes public and private fields
  void                  getFields(const char*           fieldName,
                                  std::vector<Symbol*>& symbols)         const;

  void                  describe()                                       const;

private:
  typedef std::vector<VisibilityStmt*>   UseImportList;
  typedef std::vector<Symbol*>           SymList;

  typedef std::set<const ResolveScope*>  ScopeSet;

  typedef std::map<const char*, Symbol*> Bindings;
  typedef std::map<Symbol*, UseImportList> UseImportMap;

                        ResolveScope();

  void                  addBuiltIns();

  bool                  isAggregateTypeAndConstructor(Symbol* sym0,
                                                      Symbol* sym1);

  bool                  isSymbolAndMethod(Symbol* sym0,
                                          Symbol* sym1);

  Symbol*               lookup(UnresolvedSymExpr* usymExpr,
                               bool isUse=false)                         const;

  Symbol*               lookupWithUses(UnresolvedSymExpr* usymExpr,
                                       bool isUse=false)                 const;

  bool                  isRepeat(Symbol* toAdd, const SymList& symbols)  const;

  Symbol*               getFieldFromPath(CallExpr* dottedExpr,
                                         bool isUse=false)               const;

  Symbol*               getField(const char* fieldName)                  const;

  Symbol*               getFieldLocally(const char* fieldName)           const;

  void                  getFields(const char* fieldName,
                                  ScopeSet&   visited,
                                  SymList&    symbols)                   const;

  bool                  getFieldsWithUses(const char* fieldName,
                                          SymList&    symbols)           const;

  void buildBreadthFirstUseImportList(UseImportList& useList) const;

  void buildBreadthFirstUseImportList(UseImportList& modules,
                                      UseImportList& current,
                                      UseImportMap&  visited) const;

   bool                 skipUse(UseImportMap&  visited,
                                const UseStmt* current)                  const;

  BaseAST*              mAstRef;
  const ResolveScope*   mParent;
  Bindings              mBindings;
  UseImportList         mUseImportList;
};

extern ResolveScope* rootScope;

#endif
