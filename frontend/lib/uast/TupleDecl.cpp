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

#include "chpl/uast/TupleDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

const char* TupleDecl::intentOrKindToString(IntentOrKind kind) {
  return qualifierToString((Qualifier)kind);
}

void TupleDecl::dumpFieldsInner(const DumpSettings& s) const {
  const char* intentStr = intentOrKindToString(intentOrKind_);
  if (intentStr[0] != '\0') {
    s.out << " " << intentStr;
  }
  Decl::dumpFieldsInner(s);
}

std::string TupleDecl::dumpChildLabelInner(int i) const {
  if (declChildNum() <= i && i < declChildNum() + numElements_) {
    return "decl " + std::to_string(i - declChildNum());
  } else if (i == typeExpressionChildNum_) {
    return "type";
  } else if (i == initExpressionChildNum_) {
    return "init";
  }

  return Decl::dumpChildLabelInner(i);
}

bool TupleDecl::assertAcceptableTupleDecl() {
  asttags::AstTag firstNonTupleTag = asttags::AST_TAG_UNKNOWN;;
  int i = 0;

  for (const auto& elt: children_) {
    if (elt.get() == attributeGroup()) {
      // TODO: Make sure it is equivalent to components?
    } else if (i == typeExpressionChildNum_) {
      // no checking needed
    } else if (i == initExpressionChildNum_) {
      // no checking needed
    } else if (!elt->isTupleDecl()) {
      if (elt->isFormal() || elt->isVariable()) {
        if (firstNonTupleTag == asttags::AST_TAG_UNKNOWN) {
          firstNonTupleTag = elt->tag();
        }

        if (elt->tag() != firstNonTupleTag) {
          CHPL_ASSERT(0 == "cannot mix formal and variable components");
          return false;
        }
      } else {
        CHPL_ASSERT(0 == "variable, formal, or tuple decl components only");
        return false;
      }
    } else {
      CHPL_ASSERT(elt->isTupleDecl());
    }
    i++;
  }
  return true;
}

owned<TupleDecl> TupleDecl::build(Builder* builder, Location loc,
                                  owned<AttributeGroup> attributeGroup,
                                  Decl::Visibility vis,
                                  Decl::Linkage linkage,
                                  TupleDecl::IntentOrKind intentOrKind,
                                  AstList elements,
                                  owned<AstNode> typeExpression,
                                  owned<AstNode> initExpression) {
  AstList list;
  int attributeGroupChildNum = NO_CHILD;
  int numElements = 0;
  int typeExpressionChildNum = NO_CHILD;
  int initExpressionChildNum = NO_CHILD;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = list.size();
    list.push_back(std::move(attributeGroup));
  }

  numElements = elements.size();
  for (auto& ast : elements) {
    list.push_back(std::move(ast));
  }

  // then add the typeExpression, if any
  if (typeExpression.get() != nullptr) {
    typeExpressionChildNum = list.size();
    list.push_back(std::move(typeExpression));
  }

  // and then the initExpression
  if (initExpression.get() != nullptr) {
    initExpressionChildNum = list.size();
    list.push_back(std::move(initExpression));
  }

  TupleDecl* ret = new TupleDecl(std::move(list), attributeGroupChildNum,
                                 vis,
                                 linkage,
                                 intentOrKind,
                                 numElements,
                                 typeExpressionChildNum,
                                 initExpressionChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
