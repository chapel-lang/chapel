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

#ifndef CHPL_UAST_CATCH_H
#define CHPL_UAST_CATCH_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Variable.h"

namespace chpl {
namespace uast {


/**
  This class represents a catch block. For example:

  \rst
  .. code-block:: chapel

    try {
      someCallThatWillThrow();
    } catch {
      writeln('Error!');
    }

  \endrst

 */
class Catch final : public AstNode {
 private:
  Catch(AstList children, int8_t errorChildNum, int8_t bodyChildNum,
        bool hasParensAroundError)
    : AstNode(asttags::Catch, std::move(children)),
      errorChildNum_(errorChildNum),
      bodyChildNum_(bodyChildNum),
      hasParensAroundError_(hasParensAroundError) {
  }

  Catch(Deserializer& des)
    : AstNode(asttags::Catch, des) {
        errorChildNum_ = des.read<int8_t>();
        bodyChildNum_ = des.read<int8_t>();
        hasParensAroundError_ = des.read<bool>();
      }

  bool contentsMatchInner(const AstNode* other) const override {
    const Catch* rhs = other->toCatch();
    return this->errorChildNum_ == rhs->errorChildNum_ &&
           this->bodyChildNum_ == rhs->bodyChildNum_ &&
           this->hasParensAroundError_ == rhs->hasParensAroundError_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  std::string dumpChildLabelInner(int i) const override;

  int8_t errorChildNum_;
  int8_t bodyChildNum_;
  bool hasParensAroundError_;

 public:
  ~Catch() override = default;

  /**
    Create and return a catch.
  */
  static owned<Catch> build(Builder* builder, Location loc,
                            owned<Variable> error,
                            owned<Block> body,
                            bool hasParensAroundError);

  /**
    Returns the error handled by this catch, or nullptr if there is none.
  */
  const Variable* error() const {
    if (errorChildNum_ < 0) return nullptr;
    auto ret = child(errorChildNum_);
    CHPL_ASSERT(ret && ret->isVariable());
    return (const Variable*)ret;
  }

  /**
    Return the block containing the body of this catch.
  */
  const Block* body() const {
    auto ret = child(bodyChildNum_);
    CHPL_ASSERT(ret && ret->isBlock());
    return (const Block*)ret;
  }

  /**
    Iterate over the statements contained in the body of this catch.
  */
  AstListIteratorPair<AstNode> stmts() const {
    return this->body()->stmts();
  }

  /**
    Return the number of statements contained in the body of this catch.
  */
  int numStmts() const {
    return this->body()->numStmts();
  }

  /**
    Return the i'th statement contained in the body of this catch.
  */
  const AstNode* stmt(int i) const {
    return this->body()->stmt(i);
  }

  /**
    Return true if the error of this catch block has parens arounds its
    declaration.
  */
  bool hasParensAroundError() const {
    return hasParensAroundError_;
  }

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
    ser.write(errorChildNum_);
    ser.write(bodyChildNum_);
    ser.write(hasParensAroundError_);
  }

  DECLARE_STATIC_DESERIALIZE(Catch);

};


} // end namespace uast
} // end namespace chpl

#endif
