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

#include "chpl/uast/For.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool For::contentsMatchInner(const ASTNode* other) const {
  const For* lhs = this;
  const For* rhs = (const For*) other;

  if (lhs->isParam_ != rhs->isParam_)
    return false;

  if (!lhs->indexableLoopContentsMatchInner(rhs))
    return false;

  return true;
}

void For::markUniqueStringsInner(Context* context) const {
  indexableLoopMarkUniqueStringsInner(context);
}

owned<For> For::build(Builder* builder,
                      Location loc,
                      owned<Decl> indexVar,
                      owned<Expression> iterand,
                      ASTList stmts,
                      bool usesDo,
                      bool isExpressionLevel,
                      bool isParam) {
  assert(iterand.get() != nullptr);
  if (isParam) assert(!isExpressionLevel);

  ASTList lst;
  int8_t indexVarChildNum = -1;
  int8_t iterandChildNum = -1;

  if (indexVar.get() != nullptr) {
    indexVarChildNum = lst.size();
    lst.push_back(std::move(indexVar));
  }

  if (iterand.get() != nullptr) {
    iterandChildNum = lst.size();
    lst.push_back(std::move(iterand));
  }

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  For* ret = new For(std::move(lst), indexVarChildNum,
                     iterandChildNum,
                     usesDo,
                     isExpressionLevel,
                     isParam);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
