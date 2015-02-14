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

#ifndef SCOPE_RESOLVE_H
#define SCOPE_RESOLVE_H
#include "astutil.h"

class AggregateType;


void build_constructors(AggregateType* ct);
void add_root_type(AggregateType* ct);

// The docs pass would like to use the following functions to resolve
// inheritance issues.

// Lydia note:
// Unfortunately, the implementation of addToSymbolTable seems to rely on
// only being called once in the compiler.  At the moment, it does not seem
// worth diving into whether the early usage of addToSymbolTable would cause
// other problems for the scopeResolve pass (because documenting the source
// code doesn't *need* to also generate an executable and we don't have another
// case that would benefit from early usage).  A motivated soul could look into
// this issue and thus allow both docs() and scopeResolve() to call this
// function.
void    addToSymbolTable(Vec<DefExpr*>& defs);
void    processImportExprs();
AggregateType* discoverParentAndCheck(Expr* storesName, AggregateType* child);

#ifdef HAVE_LLVM

#include "vec.h"

class Expr;
class ModuleSymbol;

// this one is defined in externCResolve.cpp
void convertDeclToChpl(ModuleSymbol* module, const char* name, Vec<Expr*>& addedToAST);
#endif

// this one is defined in util/clangUtil.cpp
void cleanupExternC();

#endif
