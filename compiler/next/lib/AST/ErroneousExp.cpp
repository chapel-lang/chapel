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

#include "chpl/AST/ErroneousExp.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace uast {


ErroneousExp::ErroneousExp()
  : Exp(asttags::ErroneousExp) {
}

bool ErroneousExp::contentsMatchInner(const ASTBase* other) const {
  const ErroneousExp* lhs = this;
  const ErroneousExp* rhs = (const ErroneousExp*) other;
  return lhs->expContentsMatchInner(rhs);
}

owned<ErroneousExp> ErroneousExp::build(Builder* builder, Location loc) {
  ErroneousExp* ret = new ErroneousExp();
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
