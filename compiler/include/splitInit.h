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

#ifndef _SPLIT_INIT_H_
#define _SPLIT_INIT_H_

#include <vector>

class CallExpr;
class DefExpr;
class Expr;
class FnSymbol;
class Symbol;
class SymExpr;
class VarSymbol;

bool isSplitInitExpr(Expr* e);
bool findInitPoints(DefExpr* def,
                    std::vector<CallExpr*>& initAssigns,
                    Expr*& usePreventingSplitInit,
                    bool allowReturns);
bool findInitPoints(CallExpr* defaultInit,
                    std::vector<CallExpr*>& initAssigns,
                    Expr*& usePreventingSplitInit,
                    bool allowReturns);

void elideCopies(FnSymbol* fn);

void splitInitMissingTypeError(Symbol* sym, Expr* mention, bool unresolved);

#endif
