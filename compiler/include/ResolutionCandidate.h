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

#ifndef _RESOLUTION_CANDIDATE_H_
#define _RESOLUTION_CANDIDATE_H_

#include "baseAST.h"
#include "vec.h"

#include <map>
#include <vector>

class ArgSymbol;
class CallInfo;
class VisibilityInfo;
class FnSymbol;
class Symbol;

void clearCoercibleCache(void);

typedef enum {
  // These are in order of severity, for failedCandidateIsBetterMatch.

  RESOLUTION_CANDIDATE_MATCH,

  // Types do not match but are related (e.g. borrowed C vs unmanaged C)
  RESOLUTION_CANDIDATE_TYPE_RELATED,

  // Types do not match but are in the same general category
  // (class-ish things, numeric things, ...)
  RESOLUTION_CANDIDATE_TYPE_SAME_CATEGORY,

  // Where clause does not match
  RESOLUTION_CANDIDATE_WHERE_FAILED,

  // Implicit where clause does not match
  RESOLUTION_CANDIDATE_IMPLICIT_WHERE_FAILED,

  // Could not fulfill interface constraints
  RESOLUTION_CANDIDATE_INTERFACE_CONSTRAINTS_NOT_SATISFIED,

  // A formal of an interface or CG function as an actual or an UG function
  RESOLUTION_CANDIDATE_INTERFACE_FORMAL_AS_ACTUAL,

  // Types do not match and are in different categories
  RESOLUTION_CANDIDATE_UNRELATED_TYPE,

  // Formal is param but actual is not
  RESOLUTION_CANDIDATE_NOT_PARAM,

  // Formal is type but actual is not or vice versa
  RESOLUTION_CANDIDATE_NOT_TYPE,

  // Actual type is not established yet
  RESOLUTION_CANDIDATE_ACTUAL_TYPE_NOT_ESTABLISHED,

  // Too many arguments
  RESOLUTION_CANDIDATE_TOO_MANY_ARGUMENTS,

  // Too few arguments
  RESOLUTION_CANDIDATE_TOO_FEW_ARGUMENTS,

  // Named argument uses argument name not present
  RESOLUTION_CANDIDATE_NO_NAMED_ARGUMENT,

  // expand if var args failure (shouldn't be user facing)
  // computeSubstitutions failure
  // failure to instantiate signature
  // dispatch for this formal for initializers
  // don't promote copy init
  RESOLUTION_CANDIDATE_OTHER,

} ResolutionCandidateFailureReason;

class ResolutionCandidate {
public:
                          ResolutionCandidate(FnSymbol* fn);

  bool                    isApplicable(CallInfo& info,
                                       VisibilityInfo* visInfo);

  FnSymbol*               fn;
  std::vector<Symbol*>    formalIdxToActual;
  std::vector<ArgSymbol*> actualIdxToFormal;

  // One ImplementsStmt per IfcConstraint when 'fn' is CG
  std::vector<ImplementsStmt*> witnesses;
  // Is this a CG "interim instantiation"?
  bool                    isInterimInstantiation;

  Symbol*                 failingArgument; // actual or formal
  ResolutionCandidateFailureReason reason;

private:
                          ResolutionCandidate();

  bool                    isApplicableConcrete(CallInfo& info,
                                               VisibilityInfo* visInfo);

  bool                    isApplicableGeneric(CallInfo& info,
                                              VisibilityInfo* visInfo);

  bool                    isApplicableCG(CallInfo& info,
                                         VisibilityInfo* visInfo);

  bool                    computeAlignment(CallInfo& info);

  bool                    computeSubstitutions(Expr* ctx);

  bool                    verifyGenericFormal(ArgSymbol* formal)       const;

  void                    computeSubstitution(ArgSymbol* formal,
                                              Symbol*    actual,
                                              Expr*      ctx);

  void                    computeSubstitutionForDefaultExpr(ArgSymbol* formal,
                                                            Expr*      ctx);

  void                    resolveTypedefedArgTypes();

  bool                    checkResolveFormalsWhereClauses(CallInfo& info,
                                                    VisibilityInfo* visInfo);

  bool                    checkGenericFormals(Expr* ctx);

  SymbolMap               substitutions;
};


void explainCandidateRejection(CallInfo& info, FnSymbol* fn);

void explainGatherCandidate(const CallInfo&            info,
                            Vec<ResolutionCandidate*>& candidates);

bool failedCandidateIsBetterMatch(ResolutionCandidate* a,
                                  ResolutionCandidate* b);

#endif
