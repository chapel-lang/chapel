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

#ifndef _SPLIT_INIT_H_
#define _SPLIT_INIT_H_

#include <vector>

class CallExpr;
class DefExpr;
class Expr;
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

// If call is a copy initialization call (e.g. chpl__autoCopy)
// with var as the argument, return the symbol capturing the result.
// Otherwise, returns NULL.
Symbol* findCopyElisionCandidate(CallExpr* call, VarSymbol* var);

bool findCopyElisionPoints(VarSymbol* var,
                           Expr* cur,
                           std::vector<CallExpr*>& points);

#endif
