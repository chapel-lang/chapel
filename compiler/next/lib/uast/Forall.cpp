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

#include "chpl/uast/Forall.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Forall::contentsMatchInner(const ASTNode* other) const {
  const Forall* lhs = this;
  const Forall* rhs = (const Forall*) other;

  if (!lhs->indexableLoopContentsMatchInner(rhs))
    return false;

  if (lhs->withClauseChildNum_ != rhs->withClauseChildNum_)
    return false;

  if (lhs->isExpressionLevel_ != rhs->isExpressionLevel_)
    return false;

  return true;
}

void Forall::markUniqueStringsInner(Context* context) const {
  indexableLoopMarkUniqueStringsInner(context);
}

owned<Forall> Forall::build(Builder* builder, Location loc,
                            owned<Decl> indexVariable,
                            owned<Expression> iterand,
                            owned<WithClause> withClause,
                            ASTList stmts,
                            bool usesDo,
                            bool isExpressionLevel) {
  assert(iterand.get() != nullptr);

  ASTList lst;
  int8_t indexVariableChildNum = -1;
  int8_t iterandChildNum = -1;
  int8_t withClauseChildNum = -1;

  if (indexVariable.get() != nullptr) {
    indexVariableChildNum = lst.size();
    lst.push_back(std::move(indexVariable));
  }

  if (iterand.get() != nullptr) {
    iterandChildNum = lst.size();
    lst.push_back(std::move(iterand));
  }

  if (withClause.get() != nullptr) {
    withClauseChildNum = lst.size();
    lst.push_back(std::move(withClause));
  }

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Forall* ret = new Forall(std::move(lst), indexVariableChildNum,
                           iterandChildNum,
                           withClauseChildNum,
                           usesDo,
                           isExpressionLevel);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Forall> Forall::build(Builder* builder, Location loc,
                            owned<Decl> indexVariable,
                            owned<Expression> iterand,
                            ASTList stmts,
                            bool usesDo,
                            bool isExpressionLevel) {
  return Forall::build(builder, loc, std::move(indexVariable),
                       std::move(iterand),
                       /*withClause*/ nullptr,
                       std::move(stmts),
                       usesDo,
                       isExpressionLevel);
}

owned<Forall> Forall::build(Builder* builder, Location loc,
                            owned<Expression> iterand,
                            owned<WithClause> withClause,
                            ASTList stmts,
                            bool usesDo) {
  return Forall::build(builder, loc, /*indexVariable*/ nullptr,
                       std::move(iterand),
                       std::move(withClause),
                       std::move(stmts),
                       usesDo,
                       /*isExpressionLevel*/ false);
}

owned<Forall> Forall::build(Builder* builder, Location loc,
                            owned<Expression> iterand,
                            ASTList stmts,
                            bool usesDo) {
  return Forall::build(builder, loc, /*indexVariable*/ nullptr,
                       std::move(iterand),
                       /*withClause*/ nullptr,
                       std::move(stmts),
                       usesDo,
                       /*isExpressionLevel*/ false);
}


} // namespace uast
} // namespace chpl
