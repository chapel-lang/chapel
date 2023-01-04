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

#include "chpl/uast/Function.h"

#include "chpl/uast/Block.h"
#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void Function::dumpFieldsInner(const DumpSettings& s) const {
  if (inline_) {
    s.out << " inline";
  }
  if (override_) {
    s.out << " override";
  }
  const char* kindStr = kindToString(kind_);
  const char* returnIntentStr = returnIntentToString(returnIntent_);
  if (kindStr[0] != '\0') {
    s.out << " " << kindStr;
  }
  if (returnIntentStr[0] != '\0') {
    s.out << " " << returnIntentStr;
  }
  if (throws_) {
    s.out << " throws";
  }
  if (primaryMethod_) {
    s.out << " primary";
  }
  if (parenless_) {
    s.out << " parenless";
  }
  NamedDecl::dumpFieldsInner(s);
}

std::string Function::dumpChildLabelInner(int i) const {
  if (i == thisFormalChildNum_) {
    return "this-formal";
  } else if (formalsChildNum_ <= i && i < formalsChildNum_ + numFormals_) {
    return "formal " + std::to_string(i - formalsChildNum_);
  } else if (i == returnTypeChildNum_) {
    return "ret-type";
  } else if (i == whereChildNum_) {
    return "where";
  } else if (lifetimeChildNum_ <= i &&
             i < lifetimeChildNum_ + numLifetimeParts_) {
    return "lifetime " + std::to_string(i - lifetimeChildNum_);
  } else if (i == bodyChildNum_) {
    return "body";
  }

  return NamedDecl::dumpChildLabelInner(i);
}

const char* Function::returnIntentToString(ReturnIntent intent) {
  switch (intent) {
    case Function::DEFAULT_RETURN_INTENT: return "";
    case Function::CONST:                 return "const";
    case Function::CONST_REF:             return "const ref";
    case Function::REF:                   return "ref";
    case Function::PARAM:                 return "param";
    case Function::TYPE:                  return "type";
  }

  return "<unknown>";
}

const char* Function::kindToString(Kind kind) {
  switch (kind) {
    case Function::PROC:     return "proc";
    case Function::ITER:     return "iter";
    case Function::OPERATOR: return "operator";
    case Function::LAMBDA:   return "lambda";
  }

  return "<unknown>";
}


owned<Function> Function::build(Builder* builder, Location loc,
                                owned<Attributes> attributes,
                                Decl::Visibility vis,
                                Function::Linkage linkage,
                                owned<AstNode> linkageNameExpr,
                                UniqueString name,
                                bool inline_,
                                bool override_,
                                Function::Kind kind,
                                owned<Formal> receiver,
                                Function::ReturnIntent returnIntent,
                                bool throws,
                                bool primaryMethod,
                                bool parenless,
                                AstList formals,
                                owned<AstNode> returnType,
                                owned<AstNode> where,
                                AstList lifetime,
                                owned<Block> body) {
  AstList lst;

  int attributesChildNum = -1;
  int linkageNameExprChildNum = -1;
  int formalsChildNum = -1;
  int thisFormalChildNum = -1;
  int numFormals = 0;
  int returnTypeChildNum = -1;
  int whereChildNum = -1;
  int lifetimeChildNum = -1;
  int numLifetimeParts = 0;
  int bodyChildNum = -1;

  if (attributes.get() != nullptr) {
    attributesChildNum = lst.size();
    lst.push_back(std::move(attributes));
  }

  if (linkageNameExpr.get() != nullptr) {
    linkageNameExprChildNum = lst.size();
    lst.push_back(std::move(linkageNameExpr));
  }

  if (receiver.get() == nullptr && formals.size() == 0) {
    // leave formalsChildNum == -1
  } else {
    formalsChildNum = lst.size();
    if (receiver.get() != nullptr) {
      thisFormalChildNum = lst.size();
      lst.push_back(std::move(receiver));
      numFormals++;
    }
    int nFormals = formals.size();
    for (auto & formal : formals) {
      lst.push_back(std::move(formal));
    }
    numFormals += nFormals;
  }

  if (returnType.get() != nullptr) {
    returnTypeChildNum = lst.size();
    lst.push_back(std::move(returnType));
  }
  if (where.get() != nullptr) {
    whereChildNum = lst.size();
    lst.push_back(std::move(where));
  }
  numLifetimeParts = lifetime.size();
  if (numLifetimeParts != 0) {
    lifetimeChildNum = lst.size();
    for (auto & part : lifetime) {
      lst.push_back(std::move(part));
    }
  }

  if (body.get() != nullptr) {
    bodyChildNum = lst.size();
    lst.push_back(std::move(body));
  }

  Function* ret = new Function(std::move(lst), attributesChildNum, vis,
                               linkage,
                               name,
                               inline_,
                               override_,
                               kind,
                               returnIntent,
                               throws,
                               primaryMethod,
                               parenless,
                               linkageNameExprChildNum,
                               formalsChildNum,
                               thisFormalChildNum,
                               numFormals,
                               returnTypeChildNum,
                               whereChildNum,
                               lifetimeChildNum,
                               numLifetimeParts,
                               bodyChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
