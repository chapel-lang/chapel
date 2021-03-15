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

#ifndef _EXTERN_C_RESOLVE_H_
#define _EXTERN_C_RESOLVE_H_

#ifdef HAVE_LLVM

class BaseAST;
class ModuleSymbol;
class Symbol;

// Return a symbol for the extern decl for a C declaration named cname
// or nullptr if there isn't a C declaration with that name in an extern block.
//
// This is called during scopeResolve
// it can call lookup() and addToSymbolTable()
//
Symbol* tryCResolve(BaseAST* context, const char* cname);
Symbol* tryCResolveLocally(ModuleSymbol* module, const char* cname);

#endif

#endif
