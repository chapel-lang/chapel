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
  UNKNOWN, // analysis cannot understand the idiom
  PENDING, // we will make a decision later
  LOCAL    // we know this is local
};

class AggregationCandidateInfo {
  public:

    CallExpr *candidate;
    ForallStmt *forall;

    LocalityInfo lhsLocalityInfo;
    LocalityInfo rhsLocalityInfo;

    CallExpr *lhsLogicalChild;
    CallExpr *rhsLogicalChild;

    Symbol *srcAggregator;   // remote rhs
    Symbol *dstAggregator;   // remote lhs

    CallExpr *srcAggCall;
    CallExpr *dstAggCall;

    AggregationCandidateInfo();
    AggregationCandidateInfo(CallExpr *candidate, ForallStmt *forall);

    void logicalChildAnalyzed(CallExpr *logicalChild, bool confirmed);
    void registerLogicalChild(CallExpr *logicalChild, bool lhs, LocalityInfo locInfo);
    void tryAddingAggregator();

    void update();
};

extern std::map<CallExpr *, AggregationCandidateInfo *> aggCandidateCache;

// interface for normalize
void doPreNormalizeArrayOptimizations();
Symbol *earlyNormalizeForallIterand(CallExpr *call, ForallStmt *forall);

// interface for resolution
Expr *preFoldMaybeLocalThis(CallExpr *call);
void updateAggregationCandidates();

#endif
