/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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


void Variable::dumpFieldsInner(const DumpSettings& s) const {
  if (isConfig_) {
    s.out << " config";
  }
  if (isField_) {
    s.out << " field";
  }
  VarLikeDecl::dumpFieldsInner(s);
}

owned<Variable>
Variable::build(Builder* builder, Location loc,
                owned<AttributeGroup> attributeGroup,
                Decl::Visibility vis,
                Decl::Linkage linkage,
                owned<AstNode> linkageName,
                UniqueString name,
                Variable::Kind kind,
                bool isConfig,
                bool isField,
                owned<AstNode> typeExpression,
                owned<AstNode> initExpression) {
  AstList lst;
  int attributeGroupChildNum = NO_CHILD;
  int linkageNameChildNum = NO_CHILD;
  int8_t typeExpressionChildNum = NO_CHILD;
  int8_t initExpressionChildNum = NO_CHILD;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
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

  Variable* ret = new Variable(std::move(lst), attributeGroupChildNum,
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

void Variable::setInitExprForConfig(owned<AstNode> ie) {
  if (this->initExpressionChildNum_ > NO_CHILD) {
    // have an existing initExpr, swap it
    this->children_[this->initExpressionChildNum_].swap(ie);
  } else {
    // no initExpr and no typeExpr nor attribute
    initExpressionChildNum_ = children_.size();
    children_.push_back(std::move(ie));
    if (this->typeExpressionChildNum_ > NO_CHILD ||
        this->attributeGroupChildNum() > NO_CHILD) {
      CHPL_ASSERT(numChildren() > 1);
    } else {
      CHPL_ASSERT(numChildren() == 1);
    }
  }
}


} // namespace uast
} // namespace chpl
