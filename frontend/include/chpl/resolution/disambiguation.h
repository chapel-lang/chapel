/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_DISAMBIGUATION_H
#define CHPL_RESOLUTION_DISAMBIGUATION_H

#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace resolution {


/**
  Given the result of filterCandidatesInstantiating, run overload
  resolution aka disambiguation to determine the most specific functions.

  If a most specific function cannot be found due to ambiguity,
  returns an empty MostSpecificCandidates that also tracks the fact
  there was an ambiguity (so it can be differentiated from no candidates).

  'lst' contains all of the TypedFnSignatures that are candidates for
  the call 'call'.

  If forwarding is in use, for any candidate, 'forwardingInfo' contains
  an element for each of the candidates in 'lst' and stores the
  actual type passed to the method receiver. This indicates which
  'forwarding' statement is in use.
 */
MostSpecificCandidates
findMostSpecificCandidates(
    Context* context,
    const std::vector<const TypedFnSignature*>& lst,
    const std::vector<types::QualifiedType>& forwardingInfo,
    const CallInfo& call,
    const Scope* callInScope,
    const PoiScope* callInPoiScope);


} // end namespace resolution
} // end namespace chpl

#endif
