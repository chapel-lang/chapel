/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/disambiguation.h"

#include "chpl/queries/query-impl.h"
#include "chpl/types/all-types.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct Resolver;

const MostSpecificCandidates&
findMostSpecificCandidates(Context* context,
                           std::vector<const TypedFnSignature*> lst,
                           CallInfo call) {
  QUERY_BEGIN(findMostSpecificCandidates, context, lst, call);

  MostSpecificCandidates result;

  if (lst.size() > 1) {

    // TODO: find most specific -- pull over disambiguation code
    // TODO: handle return intent overloading
    // TODO: this is demo code
    if (call.numActuals() > 1) {
      if (call.actuals(1).type().type()->isIntType()) {
        result.setBestRef(lst[0]);
      } else {
        result.setBestRef(lst[lst.size()-1]);
      }
    } else {
      result.setBestRef(lst[0]);
    }
  }
  if (lst.size() == 1) {
    result.setBestRef(lst[0]);
  }

  return QUERY_END(result);
}


} // end namespace resolution
} // end namespace chpl
