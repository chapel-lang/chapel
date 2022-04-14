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

#include "chpl/uast/Interface.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Interface> Interface::build(Builder* builder, Location loc,
                                  owned<Attributes> attributes,
                                  Decl::Visibility visibility,
                                  UniqueString name,
                                  bool isFormalListPresent,
                                  AstList formals,
                                  AstList body) {
  AstList children;
  int attributesChildNum = AstNode::NO_CHILD;
  int interfaceFormalsChildNum = AstNode::NO_CHILD;
  int numInterfaceFormals = 0;
  int bodyChildNum = AstNode::NO_CHILD;
  int numBodyStmts = 0;

  if (attributes.get() != nullptr) {
    attributesChildNum = children.size();
    children.push_back(std::move(attributes));
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

  Interface* ret = new Interface(std::move(children), attributesChildNum,
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
