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

#ifndef CHPL_UAST_IDENTIFIER_H
#define CHPL_UAST_IDENTIFIER_H

#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a reference to a symbol by name.
  E.g. in

  \rst
  .. code-block:: chapel

      var x = 1; // here, 'x' is not an Identifier (it is the declared symbol)
      f(x);      // here, 'f' and 'x' are Identifiers
  \endrst
 */
class Identifier final : public AstNode {
 friend class AstNode;

 private:
  UniqueString name_;

  Identifier(UniqueString name)
    : AstNode(asttags::Identifier), name_(name) {

    CHPL_ASSERT(!name.isEmpty());
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(name_);
  }

  explicit Identifier(Deserializer& des)
    : AstNode(asttags::Identifier, des) {
    name_ = des.read<UniqueString>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Identifier* lhs = this;
    const Identifier* rhs = (const Identifier*) other;
    return lhs->name_ == rhs->name_;
  }
  void markUniqueStringsInner(Context* context) const override {
    this->name_.mark(context);
  }

  void dumpFieldsInner(const DumpSettings& s) const override;

 public:
  ~Identifier() override = default;
  static owned<Identifier> build(Builder* builder, Location loc, UniqueString name);
  UniqueString name() const { return name_; }
};

/*
 * Returns true if 'node' is an Identifier with the name "?"
 */
bool isQuestionMark(const AstNode* node);


} // end namespace uast
} // end namespace chpl

#endif
