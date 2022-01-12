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

#include "chpl/uast/Variable.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Variable>
Variable::build(Builder* builder, Location loc,
                owned<Attributes> attributes,
                Decl::Visibility vis,
                Decl::Linkage linkage,
                owned<Expression> linkageName,
                UniqueString name,
                Variable::Kind kind,
                bool isConfig,
                bool isField,
                owned<Expression> typeExpression,
                owned<Expression> initExpression) {
  ASTList lst;
  int attributesChildNum = -1;
  int linkageNameChildNum = -1;
  int8_t typeExpressionChildNum = -1;
  int8_t initExpressionChildNum = -1;

  if (attributes.get() != nullptr) {
    attributesChildNum = lst.size();
    lst.push_back(std::move(attributes));
  }

  if (linkageName.get() != nullptr) {
    linkageNameChildNum = lst.size();
    lst.push_back(std::move(linkageName));
  }

  if (typeExpression.get() != nullptr) {
    typeExpressionChildNum = lst.size();
    lst.push_back(std::move(typeExpression));
  }

  if (initExpression.get() != nullptr) {
    initExpressionChildNum = lst.size();
    lst.push_back(std::move(initExpression));
  }

  Variable* ret = new Variable(std::move(lst), attributesChildNum,
                               vis,
                               linkage,
                               linkageNameChildNum,
                               name,
                               kind,
                               isConfig,
                               isField,
                               typeExpressionChildNum,
                               initExpressionChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
