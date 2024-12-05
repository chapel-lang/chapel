/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef RETURN_TYPE_INFERENCE_H
#define RETURN_TYPE_INFERENCE_H

#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace uast {
  class AggregateDecl;
}
namespace resolution {
struct Resolver;

const std::vector<InheritanceImplements>&
getImplementedInterfaces(Context* context,
                         const uast::AggregateDecl* ad);

// this helper function computes a CompositeType based upon
// a decl and some substitutions
const types::CompositeType*
helpGetTypeForDecl(Context* context,
                   const uast::AggregateDecl* ad,
                   const SubstitutionsMap& substitutions,
                   const PoiScope* poiScope,
                   const types::Type* instantiatedFrom);

/* Compute or infer the return type for this function
   and store the result in resolver.returnType. */
void computeReturnType(Resolver& resolver);


} // end namespace resolution
} // end namespace chpl

#endif
