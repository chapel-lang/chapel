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

#include "chpl/uast/Interface.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void Interface::dumpFieldsInner(const DumpSettings& s) const {
  if (isFormalListExplicit_) {
    s.out << " explicit-formals";
  }
  return NamedDecl::dumpFieldsInner(s);
}

std::string Interface::dumpChildLabelInner(int i) const {
  if (interfaceFormalsChildNum_ <= i &&
      i < interfaceFormalsChildNum_ + numInterfaceFormals_) {
    return "if-formal " + std::to_string(i - interfaceFormalsChildNum_);
  } else if (i == bodyChildNum_) {
    return "body";
  }

  return NamedDecl::dumpChildLabelInner(i);
}


owned<Interface> Interface::build(Builder* builder, Location loc,
                                  owned<AttributeGroup> attributeGroup,
                                  Decl::Visibility visibility,
                                  UniqueString name,
                                  bool isFormalListPresent,
                                  AstList formals,
                                  AstList body) {
  AstList children;
  int attributeGroupChildNum = AstNode::NO_CHILD;
  int interfaceFormalsChildNum = AstNode::NO_CHILD;
  int numInterfaceFormals = 0;
  int bodyChildNum = AstNode::NO_CHILD;
  int numBodyStmts = 0;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = children.size();
    children.push_back(std::move(attributeGroup));
  }

  if (formals.size() != 0) {
    interfaceFormalsChildNum = children.size();
    numInterfaceFormals = formals.size();
    for (auto& ast : formals) children.push_back(std::move(ast));
  }

  if (body.size() != 0) {
    bodyChildNum = children.size();
    numBodyStmts = body.size();
    for (auto& ast : body) children.push_back(std::move(ast));
  }

  Interface* ret = new Interface(std::move(children), attributeGroupChildNum,
                                 visibility,
                                 name,
                                 interfaceFormalsChildNum,
                                 numInterfaceFormals,
                                 bodyChildNum,
                                 numBodyStmts,
                                 isFormalListPresent);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
