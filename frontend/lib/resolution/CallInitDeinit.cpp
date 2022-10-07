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

#include "CallInitDeinit.h"

#include "Resolver.h"

/*#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/disambiguation.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
*/

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

void CallInitDeinit::process(Resolver& resolver) {
  CallInitDeinit uv(resolver);
  MutatingResolvedVisitor<CallInitDeinit> rv(resolver.context,
                                             resolver.symbol,
                                             uv,
                                             resolver.byPostorder);

  resolver.symbol->traverse(rv);
}

CallInitDeinit::CallInitDeinit(Resolver& resolver)
  : context(resolver.context), resolver(resolver)
{
}

bool CallInitDeinit::enter(const AstNode* ast, RV& rv) {
  return false;
}
void CallInitDeinit::exit(const AstNode* ast, RV& rv) {
}


} // end namespace resolution
} // end namespace chpl
