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

#include "chpl/uast/Manage.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Manage> Manage::build(Builder* builder, Location loc,
                            ASTList managers,
                            BlockStyle blockStyle,
                            ASTList stmts) {
  int managerExprChildNum = -1;
  const int numManagerExprs = managers.size();
  int bodyChildNum = -1;
  const int numBodyStmts = stmts.size();
  ASTList children;

  if (managers.size()) {
    managerExprChildNum = children.size();
    children.swap(managers);
  }

  if (stmts.size()) {
    bodyChildNum = children.size();
    for (auto& ast : stmts) children.push_back(std::move(ast));
  }

  Manage* ret = new Manage(std::move(children), managerExprChildNum,
                           numManagerExprs,
                           blockStyle,
                           bodyChildNum,
                           numBodyStmts);

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
