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

#include "chpl/uast/CStringLiteral.h"

#include "chpl/uast/Builder.h"

#include <cstdlib>

namespace chpl {
namespace uast {


bool CStringLiteral::contentsMatchInner(const ASTNode* other) const {
  const CStringLiteral* lhs = this;
  const CStringLiteral* rhs = (const CStringLiteral*) other;
  return lhs->literalContentsMatchInner(rhs) &&
         lhs->quotes_ == rhs->quotes_ &&
         lhs->value_ == rhs->value_;
}
void CStringLiteral::markUniqueStringsInner(Context* context) const {
  literalMarkUniqueStringsInner(context);
}

owned<CStringLiteral> CStringLiteral::build(Builder* builder, Location loc,
                                          StringLiteral::QuoteStyle quotes,
                                          std::string value) {
  CStringLiteral* ret = new CStringLiteral(quotes, std::move(value));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
