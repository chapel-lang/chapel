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

#ifndef CHPL_UAST_ON_H
#define CHPL_UAST_ON_H

#include "chpl/framework/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/SimpleBlockLike.h"

namespace chpl {
namespace uast {


/**
  This class represents an on statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var x = 0;
      on Locales[1] do writeln(x);

  \endrst

 */
class On final : public SimpleBlockLike {
 friend class AstNode;

 private:
  static const int8_t destChildNum_ = 0;

  On(AstList children, BlockStyle blockStyle, int bodyChildNum,
     int numBodyStmts)
    : SimpleBlockLike(asttags::On, std::move(children), blockStyle,
                      bodyChildNum,
                      numBodyStmts) {
  }

  void serializeInner(Serializer& ser) const override {
    simpleBlockLikeSerializeInner(ser);
  }

  explicit On(Deserializer& des)
    : SimpleBlockLike(asttags::On, des) { }

  bool contentsMatchInner(const AstNode* other) const override {
    return simpleBlockLikeContentsMatchInner(other);
  }

  void markUniqueStringsInner(Context* context) const override {
    simpleBlockLikeMarkUniqueStringsInner(context);
  }

  std::string dumpChildLabelInner(int i) const override;

 public:

  /**
    Create and return an on statement.
  */
  static owned<On> build(Builder* builder, Location loc,
                         owned<AstNode> destination,
                         BlockStyle blockStyle,
                         AstList stmts);

  /**
    Returns the destination of this on statement.
  */
  const AstNode* destination() const {
    auto ret = child(destChildNum_);
    return ret;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
