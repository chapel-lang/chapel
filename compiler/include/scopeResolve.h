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

#ifndef _SCOPE_RESOLVE_H_
#define _SCOPE_RESOLVE_H_

class astlocT;
class BaseAST;
class CallExpr;
class DefExpr;
class FnSymbol;
class Symbol;

#include <cstddef>
#include <map>
#include <vector>

void     addToSymbolTable(FnSymbol* fn);
void     addToSymbolTable(DefExpr* def);

Symbol*  lookup(const char*           name,
                BaseAST*              context);

void     lookup(const char*           name,
                BaseAST*              context,
                std::vector<Symbol*>& symbols,
                std::map<Symbol*, astlocT*>& renameLocs,
                bool storeRenames = false);

Symbol*  lookupAndCount(const char*           name,
                        BaseAST*              context,
                        int&                  nSymbolsFound,
                        bool storeRenames = false,
                        astlocT** renameLoc = NULL);

void checkConflictingSymbols(std::vector<Symbol *>& symbols,
                             const char* name,
                             BaseAST* context,
                             bool storeRenames,
                             std::map<Symbol*, astlocT*>& renameLocs);

BaseAST* getScope(BaseAST* ast);

void resolveUnresolvedSymExprs(BaseAST* ast);
void resolveUnmanagedBorrows(CallExpr* call);

void destroyModuleUsesCaches();

#endif
