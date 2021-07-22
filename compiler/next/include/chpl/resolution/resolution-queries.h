/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_RESOLUTION_QUERIES_H
#define CHPL_RESOLUTION_RESOLUTION_QUERIES_H

#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-types.h"

namespace chpl {
namespace resolution {


  ////// resolution basics

  /**
    Resolve the contents of a Module
   */
  const ResolutionResultByPostorderID& resolveModule(Context* context, ID id);

  /**
    Compute the type for a NamedDecl with a particular id.
   */
  const types::QualifiedType& typeForModuleLevelSymbol(Context* context, ID id);

  /**
    Compute the type for a Builtin type using just its name
   */
  const types::QualifiedType& typeForBuiltin(Context* context, UniqueString name);

  /////// function resolution

  /**
    Compute an UntypedFnSignature for a Function, or return
    nullptr if the passed ID is not a Function.
    The pointer result is unique'd and safe to use as a query argument.
   */
  const UntypedFnSignature* untypedSignature(Context* context, ID id);


  /**
    Compute a TypedFnSignature from an UntypedFnSignature.
    The TypedFnSignature will represent generic and potentially unknown
    types if the function is generic.

    The pointer result is unique'd and safe to use as a query argument.
   */
  const TypedFnSignature*
  typedSignatureInital(Context* context,
                       const UntypedFnSignature* untypedSignature);

  /**
    Instantiate a TypedFnSignature from
     * the result of typedSignatureInital,
     * a CallInfo describing the types at the call site, and
     * a point-of-instantiation scope representing the POI scope of the call

    Returns nullptr if the instantiation failed.
   */
  const TypedFnSignature* instantiateSignature(Context* context,
                                               const TypedFnSignature* sig,
                                               CallInfo call,
                                               const PoiScope* poiScope);

  /**
    Compute a ResolvedFunction given a TypedFnSignature.
    Checks the generic cache for potential for reuse. When reuse occurs,
    the ResolvedFunction might point to a different TypedFnSignature.
   */
  const ResolutionResultByPostorderID&
  resolvedFunction(Context* context,
                   const TypedFnSignature* sig,
                   const PoiScope* poiScope);


  /////// call resolution

  /**
    Compute the (potentially generic) TypedFnSignatures of possibly applicable
    candidate functions from a list of visible functions.
   */
  const std::vector<const TypedFnSignature*>&
  filterCandidatesInitial(Context* context,
                          std::vector<BorrowedIdsWithName> lst,
                          CallInfo call);

  /**
    Further filter the result of filterCandidatesInitial down by doing
    instantiations. After this, all of the resulting TypedFnSignatures
    are actually candidates.
   */
  std::vector<const TypedFnSignature*>
  filterCandidatesInstantiating(Context* context,
                                std::vector<const TypedFnSignature*> lst,
                                CallInfo call,
                                const PoiScope* poiScope);

  /**
    Given the result of filterCandidatesInstantiating, run
    overload resolution aka disambiguation
    to determine the most specific functions.
   */
  const MostSpecificCandidates&
  findMostSpecificCandidates(Context* context,
                             std::vector<const TypedFnSignature*> lst,
                             CallInfo call);


  /**
    Given a CallInfo representing a call, a Scope representing the
    scope of that call, and a PoiScope representing the point-of-instantiation
    scope of that call, find the most specific candidates.
   */
  MostSpecificCandidates resolveCall(Context* context,
                                     const uast::Call* call,
                                     CallInfo ci,
                                     const Scope* scope,
                                     const PoiScope* poiScope);


} // end namespace resolution
} // end namespace chpl
#endif
