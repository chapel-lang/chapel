/*
 * Copyright 2004-2017 Cray Inc.
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

#include "visibleCandidates.h"

#include "callInfo.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "resolution.h"
#include "ResolutionCandidate.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             bool                       lastResort,
                             Vec<ResolutionCandidate*>& candidates);

static void filterCandidate (CallInfo&                  info,
                             FnSymbol*                  fn,
                             Vec<ResolutionCandidate*>& candidates);

void findVisibleCandidates(CallInfo&                  info,
                           Vec<FnSymbol*>&            visibleFns,
                           Vec<ResolutionCandidate*>& candidates) {
  // Search user-defined (i.e. non-compiler-generated) functions first.
  gatherCandidates(info, visibleFns, false, candidates);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    gatherCandidates(info, visibleFns, true, candidates);
  }
}

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             bool                       lastResort,
                             Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, fn, visibleFns) {
    // Only consider functions marked with/without FLAG_LAST_RESORT
    // (where existence of the flag matches the lastResort argument)
    if (fn->hasFlag(FLAG_LAST_RESORT) == lastResort) {

      // Consider
      //
      //   c1.foo(10, 20);
      //
      // where foo() is a simple method on some class/record
      //
      // Normalize currently converts this to
      //
      //   #<Call     #<Call "foo" _mt c1>    10    20>
      //
      // Resolution performs a post-order traversal of this expression
      // and so the inner call is visited before the outer call.
      //
      // In this context, the inner "call" is effectively a field access
      // rather than a true function call.  Normalize sets the methodTag
      // property to true to indicate this, and this form of call can only
      // be matched to parentheses-less methods and type constructors.
      //
      // Later steps will convert the outer call to become
      //
      //   #<Call "foo" _mt c1  10    20>
      //
      // This outer call has methodTag set to false and this call
      // should be filtered against the available visibleFunctions.
      //

      if (info.call->methodTag == false) {
        filterCandidate(info, fn, candidates);

      } else {
        if (fn->hasFlag(FLAG_NO_PARENS)        == true ||
            fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == true) {
          filterCandidate(info, fn, candidates);
        }
      }
    }
  }
}

static void filterCandidate(CallInfo&                  info,
                            FnSymbol*                  fn,
                            Vec<ResolutionCandidate*>& candidates) {
  ResolutionCandidate* candidate = new ResolutionCandidate(fn);

  if (fExplainVerbose &&
      ((explainCallLine && explainCallMatch(info.call)) ||
       info.call->id == explainCallID)) {
    USR_PRINT(fn, "Considering function: %s", toString(fn));

    if (info.call->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }
  }

  if (candidate->isApplicable(info) == true) {
    candidates.add(candidate);
  } else {
    delete candidate;
  }
}
