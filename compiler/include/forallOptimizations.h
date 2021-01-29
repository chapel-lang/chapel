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

#ifndef _PRE_NORMALIZE_OPTIMIZATIONS_H_
#define _PRE_NORMALIZE_OPTIMIZATIONS_H_

#include "CallExpr.h"
#include "symbol.h"

enum LocalityInfo { 
  UNKNOWN,  // it may be anything
  PENDING,  // we will make a decision later
  LOCAL,    // we know this is local
  UNAGGREGATABLE  // neither local, nor aggregatable
};

// we use this during normalize when transforming an assignment for potential
// aggregation
class AggregationCandidateInfo {
  public:

    CallExpr *candidate;
    ForallStmt *forall;

    LocalityInfo lhsLocalityInfo;
    LocalityInfo rhsLocalityInfo;

    CallExpr *lhsLogicalChild;
    Expr *rhsLogicalChild;

    // during normalize, we may generate aggregators for both sides of the
    // assignment. However, after resolve we can have at most one aggregator per
    // assignment ...
    Symbol *srcAggregator;   // remote rhs
    Symbol *dstAggregator;   // remote lhs

    AggregationCandidateInfo(CallExpr *candidate, ForallStmt *forall);

    void addAggregators();
    void removeSideEffectsFromPrimitive();
    void transformCandidate();
};

// interface for normalize
void doPreNormalizeArrayOptimizations();
Symbol *earlyNormalizeForallIterand(CallExpr *call, ForallStmt *forall);

// interface for resolution
Expr *preFoldMaybeLocalThis(CallExpr *call);
Expr *preFoldMaybeLocalArrElem(CallExpr *call);
Expr *preFoldMaybeAggregateAssign(CallExpr *call);

// interface for lowerForalls
void removeAggregationFromRecursiveForall(ForallStmt *forall);

// interface for licm
void transformConditionalAggregation(CondStmt *cond);
void cleanupRemainingAggCondStmts();

#endif
