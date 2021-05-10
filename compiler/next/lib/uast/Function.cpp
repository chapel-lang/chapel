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

#include "chpl/uast/Function.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Function::contentsMatchInner(const ASTNode* other) const {
  const Function* lhs = this;
  const Function* rhs = (const Function*) other;
  return lhs->symContentsMatchInner(rhs) &&
         lhs->linkage_ == rhs->linkage_ &&
         lhs->returnIntent_ == rhs->returnIntent_ &&
         lhs->inline_ == rhs->inline_ &&
         lhs->override_ == rhs->override_ &&
         lhs->throws_ == rhs->throws_ &&
         lhs->linkageNameExprChildNum_ == rhs->linkageNameExprChildNum_ &&
         lhs->formalsChildNum_ == rhs->formalsChildNum_ &&
         lhs->thisFormalChildNum_ == rhs->thisFormalChildNum_ &&
         lhs->numFormals_ == rhs->numFormals_ &&
         lhs->returnTypeChildNum_ == rhs->returnTypeChildNum_ &&
         lhs->whereChildNum_ == rhs->whereChildNum_ &&
         lhs->lifetimeChildNum_ == rhs->lifetimeChildNum_ &&
         lhs->numLifetimeParts_ == rhs->numLifetimeParts_ &&
         lhs->bodyChildNum_ == rhs->bodyChildNum_ &&
         lhs->numBodyStmts_ == rhs->numBodyStmts_;
}
void Function::markUniqueStringsInner(Context* context) const {
  symMarkUniqueStringsInner(context);
}


} // namespace uast
} // namespace chpl
