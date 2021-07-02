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

#include "chpl/uast/VisibilityClause.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<VisibilityClause>
VisibilityClause::build(Builder* builder, Location loc,
                        owned<Expression> symbol,
                        LimitationKind limitationKind,
                        ASTList limitations) {
  assert(symbol.get() != nullptr);

  ASTList lst;
  int numLimitations = 0;

  lst.push_back(std::move(symbol));

  if (limitations.size()) {
    numLimitations = limitations.size();
    for (auto& ast : limitations) {
      lst.push_back(std::move(ast));
    }
  }

  VisibilityClause* ret = new VisibilityClause(std::move(lst),
                                               limitationKind,
                                               numLimitations);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<VisibilityClause>
VisibilityClause::build(Builder* builder, Location loc,
                        owned<Expression> symbol) {
  return build(builder, loc, std::move(symbol),
               VisibilityClause::NONE,
               ASTList());
}

} // namespace uast
} // namespace chpl
