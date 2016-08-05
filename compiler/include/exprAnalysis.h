/*
 * Copyright 2004-2016 Cray Inc.
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

#ifndef _EXPR_ANALYSIS_H_
#define _EXPR_ANALYSIS_H_

#include "expr.h"
#include "symbol.h"

//std::map<Expr*,bool> safeExprCache;
//std::map<FnSymbol*,bool> globalManipFuncCache;

bool isNonEssentialPrimitive(CallExpr* ce);

/*
 * Returns true if `e` has no side effects. Checked side effects are:
 *  - Read/write to a global
 *  - Is/containsessential primitive
 *  - If it's a function call has ref arguments
 */
bool exprHasNoSideEffects(Expr* e);

bool isSafePrimitive(CallExpr* ce);

bool isRegisteredGlobalManip(FnSymbol* fn);
bool getGlobalManip(FnSymbol* fn);
void registerGlobalManip(FnSymbol* fn, bool manip);

#endif
