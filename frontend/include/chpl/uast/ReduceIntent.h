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

#ifndef CHPL_UAST_REDUCE_INTENT_H
#define CHPL_UAST_REDUCE_INTENT_H

#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/NamedDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a reduce intent.

  \rst
  .. code-block:: chapel

      // Example 1:
      var sum = 0;
      forall elm in A with (PlusReduceOp(int) reduce sum) {
        sum reduce= elm;   // bools are implicitly coerced to 'int' input type
        writeln(sum);      // accumulation state: int
      }

  \endrst

  Where PlusReduceOp(int) is a FnCall with PlusReduceOp as the op and int is
  the input type

}

*/
class ReduceIntent final : public NamedDecl {
 friend class AstNode;

 private:
  static const int opChildNum_ = 0;

  ReduceIntent(AstList children, UniqueString name)
      : NamedDecl(asttags::ReduceIntent, std::move(children),
                  /* attributeGroupChildNum= */ AstNode::NO_CHILD,
                  Decl::DEFAULT_VISIBILITY,
                  Decl::DEFAULT_LINKAGE,
                  /*linkageNameChildNum=*/ AstNode::NO_CHILD,
                  name) {
    CHPL_ASSERT(numChildren() == 1);
  }

  void serializeInner(Serializer& ser) const override {
    namedDeclSerializeInner(ser);
  }

  explicit ReduceIntent(Deserializer& des)
    : NamedDecl(asttags::ReduceIntent, des) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const ReduceIntent* rhs = other->toReduceIntent();
    return this->namedDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

  std::string dumpChildLabelInner(int i) const override;

 public:
  ~ReduceIntent() override = default;

  /**
    Create and return a reduction.
  */
  static owned<ReduceIntent> build(Builder* builder,
                                   Location loc,
                                   owned<AstNode> op,
                                   UniqueString name);

  /**
    Returns the reduce op expression, e.g. `minmax(int)` in the expression
    `minmax(int) reduce sum`.
  */
  const AstNode* op() const {
    return this->child(opChildNum_);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
