/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "llvm/ADT/SmallVector.h"

class astlocT;
class BaseAST;
class CallExpr;
class DefExpr;
class FnSymbol;
class ModuleSymbol;
class Symbol;
class VisibilityStmt;
class astlocT;

#include <cstddef>
#include <map>
#include <vector>

void     addToSymbolTable(FnSymbol* fn);
void     addToSymbolTable(DefExpr* def);

Symbol*  lookup(const char*           name,
                BaseAST*              context);

void     lookup(const char*           name,
                BaseAST*              context,
                llvm::SmallVectorImpl<Symbol*>& symbols,
                std::map<Symbol*, astlocT*>& renameLocs,
                std::map<Symbol*, VisibilityStmt*>& reexportPts,
                bool storeRenames = false);

Symbol*  lookupAndCount(const char*           name,
                        BaseAST*              context,
                        int&                  nSymbolsFound,
                        bool storeRenames = false,
                        astlocT** renameLoc = NULL,
                        bool issueErrors = true);

// Lookup a name while ignoring extern blocks.
// Also considers modules used/imported and the root module for builtins.
// For use by externCResolve.
Symbol* lookupInModuleOrBuiltins(ModuleSymbol* mod, const char* name,
                                int& nSymbolsFound);

void checkConflictingSymbols(llvm::SmallVectorImpl<Symbol *>& symbols,
                             const char* name,
                             BaseAST* context,
                             bool storeRenames,
                             std::map<Symbol*, astlocT*>& renameLocs,
                             std::map<Symbol*, VisibilityStmt*>& reexportPts);

BaseAST* getScope(BaseAST* ast);

void resolveUnresolvedSymExprs(BaseAST* ast);
void resolveUnmanagedBorrows(CallExpr* call);

#endif
