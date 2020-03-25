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

#ifndef _EXPR_ANALYSIS_H_
#define _EXPR_ANALYSIS_H_

#include "expr.h"
#include "symbol.h"


/*
 * This class contains necessary data structures and member functions
 * for expression analysis. At this point it is only used in
 * denormalize pass and functionality is rather simple.
 *
 * Its main use in the pass is to decide if expressions are movable
 * during denormalization. 
 *
 * For now, this is a very conservative analysis. A more precise
 * analysis could distinguish between reads and writes to memory and
 * to take into account alias analysis.
 *
 * Overall an expression is "unsafe" if:
 *
 * - It is an unsafe primitive(check implementation for a list)
 * - It is a function CallExpr where FnSymbol:
 *   - Has ref arguments(const ref should be fine but not implemented)
 *   - Touches any globals
 *   - Touches any externs
 *   - Has unsafe primitives
 *
 * Function safety check is recursive
 */
class SafeExprAnalysis {

  private:
    std::map<Expr*,bool> safeExprCache;
    std::map<FnSymbol*,bool> safeFnCache;
    std::map<FnSymbol*,bool> globalManipFuncCache;
    std::map<FnSymbol*,bool> externManipFuncCache;
  public:
    bool isNonEssentialPrimitive(CallExpr* ce);

    bool exprHasNoSideEffects(Expr* e, Expr* exprToMove);
    bool fnHasNoSideEffects(FnSymbol* fn);

    bool isSafePrimitive(CallExpr* ce);

    bool isRegisteredGlobalManip(FnSymbol* fn);
    bool getGlobalManip(FnSymbol* fn);
    void registerGlobalManip(FnSymbol* fn, bool manip);

    bool isRegisteredExternManip(FnSymbol* fn);
    bool getExternManip(FnSymbol* fn);
    void registerExternManip(FnSymbol* fn, bool manip);
};

#endif
