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


static int32_t computeStatementChildNum(int8_t indexVarChildNum,
                                        int8_t iterandChildNum) {
  return (indexVarChildNum >= 0) + (iterandChildNum >= 0);
}

For::For(ASTList children, int8_t indexVarChildNum, int8_t iterandChildNum,
         bool usesDo,
         bool expressionLevel,
         bool param)
  : Loop(asttags::For, std::move(children),
         computeStatementChildNum(indexVarChildNum, iterandChildNum),
         usesDo),
    indexVarChildNum_(indexVarChildNum),
    iterandChildNum_(iterandChildNum),
    expressionLevel_(expressionLevel),
    param_(param) {

#ifndef NDEBUG
  assert(iterandChildNum >= 0);
  // check that all children are exprs (and not, say, Symbols)
  for (const ASTNode* child : this->children()) {
    assert(child->isExpression());
  }
#endif
}

bool For::contentsMatchInner(const ASTNode* other) const {
  const For* lhs = this;
  const For* rhs = (const For*) other;
  return lhs->expressionContentsMatchInner(rhs);
}

void For::markUniqueStringsInner(Context* context) const {
  expressionMarkUniqueStringsInner(context);
}

owned<For> For::build(Builder* builder,
                      Location loc,
                      owned<Expression> indexVar,
                      owned<Expression> iterand,
                      ASTList stmts,
                      bool usesDo,
                      bool expressionLevel,
                      bool param) {
#ifndef NDEBUG
  assert(indexVar.get() != nullptr);
  assert(iterand.get() != nullptr);
  assert(param ? !expressionLevel : true);
#endif

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

  For* ret = new For(std::move(lst), indexVarChildNum, iterandChildNum,
                     usesDo,
                     expressionLevel,
                     param);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<For> For::build(Builder* builder,
                      Location loc,
                      owned<Expression> iterand,
                      ASTList stmts,
                      bool usesDo) {
#ifndef NDEBUG
  assert(iterand.get() != nullptr);
#endif

  const bool expressionLevel = false;
  const bool param = false;
  const int8_t indexVarChildNum = -1;

  ASTList lst;
  int8_t iterandChildNum = -1;

  if (iterand.get() != nullptr) {
    iterandChildNum = lst.size();
    lst.push_back(std::move(iterand));
  }

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  For* ret = new For(std::move(lst), indexVarChildNum, iterandChildNum,
                     usesDo,
                     expressionLevel,
                     param);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
