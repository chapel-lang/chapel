/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/Class.h"

#include "chpl/uast/Builder.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"

namespace chpl {
namespace uast {


std::string Class::dumpChildLabelInner(int i) const {
  if (i == parentClassChildNum_) {
    return "parent-class";
  }

  return "";
}

owned<Class> Class::build(Builder* builder, Location loc,
                          owned<AttributeGroup> attributeGroup,
                          Decl::Visibility vis,
                          UniqueString name,
                          owned<AstNode> parentClass,
                          AstList contents) {
  AstList lst;
  int attributeGroupChildNum = NO_CHILD;
  int parentClassChildNum = NO_CHILD;
  int numParentClasses = 0;
  int elementsChildNum = NO_CHILD;
  int numElements = 0;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
  }

  if (parentClass.get() != nullptr) {
    parentClassChildNum = lst.size();
    numParentClasses = 1;
    lst.push_back(std::move(parentClass));
  }
  numElements = contents.size();
  if (numElements > 0) {
    elementsChildNum = lst.size();
    for (auto & elt : contents) {
      lst.push_back(std::move(elt));
    }
  }

  Class* ret = new Class(std::move(lst), attributeGroupChildNum, vis, name,
                         parentClassChildNum,
                         numParentClasses,
                         elementsChildNum,
                         numElements);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

const Identifier* Class::getInheritExprIdent(const AstNode* ast,
                                             bool& markedGeneric) {
  if (ast != nullptr) {
    if (ast->isIdentifier()) {
      // inheriting from e.g. Parent is OK
      markedGeneric = false;
      return ast->toIdentifier();
    } else if (auto call = ast->toFnCall()) {
      const AstNode* calledExpr = call->calledExpression();
      if (calledExpr != nullptr && calledExpr->isIdentifier() &&
          call->numActuals() == 1) {
        if (const AstNode* actual = call->actual(0)) {
          if (auto id = actual->toIdentifier()) {
            if (id->name() == USTR("?")) {
              // inheriting from e.g. Parent(?) is OK
              markedGeneric = true;
              return calledExpr->toIdentifier();
            }
          }
        }
      }
    }
  }

  markedGeneric = false;
  return nullptr;
}

bool Class::isAcceptableInheritExpr(const AstNode* ast) {
  bool ignoredMarkedGeneric = false;
  return getInheritExprIdent(ast, ignoredMarkedGeneric) != nullptr;
}


} // namespace uast
} // namespace chpl
