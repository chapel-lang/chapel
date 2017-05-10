/*
 * Copyright 2004-2017 Cray Inc.
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

class BaseAST;
class BlockStmt;
class DefExpr;
class FnSymbol;
class Symbol;
class TypeSymbol;

// A preliminary version of a class to support the scope resolve pass
// This is currently a thin wrapping over a previous typedef + functions
class ResolveScope {
public:
  static ResolveScope*            findOrCreateScopeFor(DefExpr* def);

  static ResolveScope*            getScopeFor(BaseAST* ast);

  static void                     destroyAstMap();

public:
                                  ResolveScope(BlockStmt*  blockStmt);
                                  ResolveScope(FnSymbol*   fnSym);
                                  ResolveScope(TypeSymbol* typeSym);

  bool                            extend(Symbol*     sym);

  Symbol*                         lookup(const char* name)               const;

private:
                                  ResolveScope();

  bool                            isAggregateTypeAndConstructor(Symbol* sym0,
                                                                Symbol* sym1);

  bool                            isSymbolAndMethod(Symbol* sym0,
                                                    Symbol* sym1);

  BaseAST*                        mAstRef;
  std::map<const char*, Symbol*>  mBindings;
};

#endif
