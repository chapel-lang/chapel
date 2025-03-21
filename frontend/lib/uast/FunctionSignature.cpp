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

#include "chpl/uast/FunctionSignature.h"
#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void FunctionSignature::dumpFieldsInner(const DumpSettings& s) const {
  const char* kindStr = Function::kindToString(kind_);
  const char* returnIntentStr = Function::returnIntentToString(returnIntent_);
  if (kindStr[0] != '\0') {
    s.out << " " << kindStr;
  }
  if (returnIntentStr[0] != '\0') {
    s.out << " " << returnIntentStr;
  }
  if (throws_) {
    s.out << " throws";
  }
  if (isParenless_) {
    s.out << " parenless";
  }
}

std::string FunctionSignature::dumpChildLabelInner(int i) const {
  if (i == thisFormalChildNum_) {
    return "this-formal";
  } else if (formalsChildNum_ <= i && i < formalsChildNum_ + numFormals_) {
    return "formal " + std::to_string(i - formalsChildNum_);
  } else if (i == returnTypeChildNum_) {
    return "ret-type";
  }

  return "";
}


owned<FunctionSignature>
FunctionSignature::build(Builder* builder, Location loc,
                         FunctionSignature::Kind kind,
                         owned<Formal> receiver,
                         FunctionSignature::ReturnIntent returnIntent,
                         bool isParenless,
                         AstList formals,
                         owned<AstNode> returnType,
                         bool throws) {
  AstList children;

  int formalsChildNum = NO_CHILD;
  int thisFormalChildNum = NO_CHILD;
  int numFormals = 0;
  int returnTypeChildNum = NO_CHILD;

  if (receiver.get()) {
    formalsChildNum = children.size();
    thisFormalChildNum = children.size();
    children.push_back(std::move(receiver));
    numFormals++;
  }

  if (formals.size()) {
    if (formalsChildNum == NO_CHILD) formalsChildNum = children.size();
    numFormals += formals.size();
    for (auto& ast : formals) children.push_back(std::move(ast));
  }

  if (returnType.get()) {
    returnTypeChildNum = children.size();
    children.push_back(std::move(returnType));
  }

  auto ret = new FunctionSignature(std::move(children), kind, returnIntent,
                                   throws,
                                   isParenless,
                                   formalsChildNum,
                                   thisFormalChildNum,
                                   numFormals,
                                   returnTypeChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
