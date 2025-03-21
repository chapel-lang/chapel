/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/Formal.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


const char* Formal::intentToString(Formal::Intent intent) {
  switch (intent) {
    case DEFAULT_INTENT: return "";
    case CONST:          return "const";
    case CONST_REF:      return "const ref";
    case REF:            return "ref";
    case IN:             return "in";
    case CONST_IN:       return "const in";
    case OUT:            return "out";
    case INOUT:          return "inout";
    case PARAM:          return "param";
    case TYPE:           return "type";
  }
  return "<unknown>";
}

owned<Formal>
Formal::build(Builder* builder, Location loc,
              owned<AttributeGroup> attributeGroup,
              UniqueString name,
              Formal::Intent intent,
              owned<AstNode> typeExpression,
              owned<AstNode> initExpression) {
  AstList lst;
  int attributeGroupChildNum = NO_CHILD;
  int8_t typeExpressionChildNum = NO_CHILD;
  int8_t initExpressionChildNum = NO_CHILD;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
  }

  if (typeExpression.get() != nullptr) {
    typeExpressionChildNum = lst.size();
    lst.push_back(std::move(typeExpression));
  }

  if (initExpression.get() != nullptr) {
    initExpressionChildNum = lst.size();
    lst.push_back(std::move(initExpression));
  }

  Formal* ret = new Formal(std::move(lst), attributeGroupChildNum,
                           name,
                           intent,
                           typeExpressionChildNum,
                           initExpressionChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
