/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_EXPRESSION_H
#define CHPL_UAST_EXPRESSION_H

#include "chpl/uast/ASTNode.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for expressions
 */
class Expression : public ASTNode {
 protected:
  Expression(asttags::ASTTag tag)
    : ASTNode(tag) {
  }
  Expression(asttags::ASTTag tag, ASTList children)
    : ASTNode(tag, std::move(children)) {
  }
  bool expressionContentsMatchInner(const Expression* other) const {
    return true;
  }
  void expressionMarkUniqueStringsInner(Context* context) const {
  }

 public:
  virtual ~Expression() = 0; // this is an abstract base class
};


} // end namespace uast
} // end namespace chpl

#endif
