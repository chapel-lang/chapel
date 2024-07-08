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

#ifndef CHPL_UAST_SCAN_H
#define CHPL_UAST_SCAN_H

#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/Call.h"

namespace chpl {
namespace uast {


/**
  This class represents a scan expression. For example:

  \rst
  ..code-block :: chapel
    var A: [1..3] int = 1;
    // Prints '1 2 3'
    writeln(+ scan A);
  \endrst

  The scan expression is '+ scan A'.
*/
class Scan final : public Call {
 friend class AstNode;

 private:
  static const int opChildNum_ = 0;
  static const int iterandChildNum_ = 1;

  Scan(AstList children)
    : Call(asttags::Scan, std::move(children), false) {
    CHPL_ASSERT(numChildren() == 2);
  }

  void serializeInner(Serializer& ser) const override {
    callSerializeInner(ser);
  }

  explicit Scan(Deserializer& des)
    : Call(asttags::Scan, des) { }

  bool contentsMatchInner(const AstNode* other) const override {
    const Scan* rhs = other->toScan();
    return this->callContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
  }

  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Scan() override = default;

  /**
    Create and return a scan.
  */
  static owned<Scan> build(Builder* builder, Location loc,
                           owned<AstNode> op,
                           owned<AstNode> iterand);

  /**
    Returns the scan op expression, e.g., `+` in the expression `+ scan A`.
  */
  const AstNode* op() const {
    return this->child(opChildNum_);
  }

  /**
    Returns the iterand of the scan, e.g., 'A' in the expression `+ scan A`.
  */
  const AstNode* iterand() const {
    return this->child(iterandChildNum_);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
