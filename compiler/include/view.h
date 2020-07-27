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

#ifndef _VIEW_H_
#define _VIEW_H_

#include "expr.h"
#include "vec.h"
#include <vector>

class GenRet;

BaseAST* aid(int id);
BaseAST* aid09(int id);
Expr*    aidExpr(int id);

// counterparts of the above for convenient shortcuts
BaseAST* aid(BaseAST* ast);
Expr*    aidExpr(BaseAST* ast);

void        list_view_noline(const BaseAST* ast);
void        nprint_view(BaseAST* ast);

// defined in codegen/codegen.cpp
void        nprint_view(GenRet& gen);

// These are not used by the compiler but are available for use in GDB
//
// The most commonly used functions are referenced in
// $CHPL_HOME/compiler/etc/gdb.commands and appear to be
//
//    aid
//    print_view
//    iprint_view
//    nprint_view
//    list_view
//    viewFlags
//    stringLoc

void        print_view(BaseAST* ast);
void        print_view_noline(BaseAST* ast);

void        iprint_view(int id);

void        nprint_view(int id);
void        nprint_view_noline(BaseAST* ast);

void        mark_view(BaseAST* ast, int id);

void        list_view(int id);
void        list_view(const BaseAST* ast);

void        astDump_view(int id);
void        astDump_view(BaseAST* ast);
void        astDumpToNode_view(int id);
void        astDumpToNode_view(BaseAST* ast);

void        viewFlags(int id);

void        map_view(SymbolMap* map);
void        map_view(SymbolMap& map);

void        vec_view(Vec<Symbol*,   VEC_INTEGRAL_SIZE>* v);
void        vec_view(Vec<Symbol*,   VEC_INTEGRAL_SIZE>& v);
void        vec_view(Vec<FnSymbol*, VEC_INTEGRAL_SIZE>* v);
void        vec_view(Vec<FnSymbol*, VEC_INTEGRAL_SIZE>& v);
void        vec_view(std::vector<Symbol*>* syms);
void        vec_view(std::vector<Symbol*>& syms);
void        vec_view(std::vector<FnSymbol*>* syms);
void        vec_view(std::vector<FnSymbol*>& syms);


void        fnsWithName(const char* name);
void        fnsWithName(const char* name, Vec<FnSymbol*>& fnVec);

void        whocalls(int id);
void        whocalls(BaseAST* ast);

FnSymbol*   debugGetTheIteratorFn(int id);
FnSymbol*   debugGetTheIteratorFn(BaseAST* ast);
FnSymbol*   debugGetTheIteratorFn(Type* type);
FnSymbol*   debugGetTheIteratorFn(ForLoop* forLoop);

// NB these return the same static buffer
const char* stringLoc(int id);
const char* stringLoc(BaseAST* ast);

const char* shortLoc(int id);
const char* shortLoc(BaseAST* ast);

const char* debugLoc(int id);
const char* debugLoc(BaseAST* ast);

int debugID(int id);
int debugID(BaseAST* ast);
void debugSummary(int id);
void debugSummary(BaseAST* ast);
void debugSummary(const char* str);
Symbol* debugParentSym(int id);
Symbol* debugParentSym(BaseAST* ast);
Expr* debugParentExpr(int id);
Expr* debugParentExpr(BaseAST* ast);
void blockSummary(int id);
void blockSummary(BaseAST* ast);
void blockSummary(BlockStmt* block, Symbol* sym);

#endif
