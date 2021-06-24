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

#include "chpl/uast/Identifier.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Identifier::contentsMatchInner(const ASTNode* other) const {
  const Identifier* lhs = this;
  const Identifier* rhs = (const Identifier*) other;
  return lhs->expressionContentsMatchInner(rhs) &&
         lhs->name_ == rhs->name_;
}
void Identifier::markUniqueStringsInner(Context* context) const {
  expressionMarkUniqueStringsInner(context);
  this->name_.mark(context);
}

owned<Identifier> Identifier::build(Builder* builder,
                                    Location loc, UniqueString name) {
  Identifier* ret = new Identifier(name);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
