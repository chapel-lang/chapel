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

#ifndef CHPL_UAST_RECORD_H
#define CHPL_UAST_RECORD_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AggregateDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a record declaration. For example:

  \rst
  .. code-block:: chapel

      record myRecord {
        var a: int;
        proc method() { }
      }

  \endrst

  The record itself (myRecord) is represented by a Record AST node.
 */
class Record final : public AggregateDecl {
 private:
  int interfaceExprChildNum_;
  int numInterfaceExprs_;

  Record(AstList children, int attributeGroupChildNum, Decl::Visibility vis,
         Decl::Linkage linkage,
         int linkageNameChildNum,
         UniqueString name,
         int interfaceExprChildNum,
         int numInterfaceExprs,
         int elementsChildNum,
         int numElements)
    : AggregateDecl(asttags::Record, std::move(children),
                    attributeGroupChildNum,
                    vis,
                    linkage,
                    linkageNameChildNum,
                    name,
                    elementsChildNum,
                    numElements),
      interfaceExprChildNum_(interfaceExprChildNum),
      numInterfaceExprs_(numInterfaceExprs) {
    if (interfaceExprChildNum_ != NO_CHILD && elementsChildNum != NO_CHILD) {
      CHPL_ASSERT(interfaceExprChildNum_ + numInterfaceExprs_ ==
                  elementsChildNum);
    }

    if (interfaceExprChildNum_ != NO_CHILD) {
      for (int i = 0; i < numInterfaceExprs_; i++) {
        CHPL_ASSERT(isAcceptableInheritExpr(child(interfaceExprChildNum_ + i)));
      }
    }
  }

  Record(Deserializer& des)
    : AggregateDecl(asttags::Record, des) {
    interfaceExprChildNum_ = des.read<int>();
    numInterfaceExprs_ = des.read<int>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Record* lhs = this;
    const Record* rhs = (const Record*) other;
    return lhs->aggregateDeclContentsMatchInner(rhs) &&
           lhs->interfaceExprChildNum_ == rhs->interfaceExprChildNum_ &&
           lhs->numInterfaceExprs_ == rhs->numInterfaceExprs_;
  }

  void markUniqueStringsInner(Context* context) const override {
    aggregateDeclMarkUniqueStringsInner(context);
  }

  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Record() override = default;

  static owned<Record> build(Builder* builder, Location loc,
                             owned<AttributeGroup> attributeGroup,
                             Decl::Visibility vis,
                             Decl::Linkage linkage,
                             owned<AstNode> linkageName,
                             UniqueString name,
                             AstList interfaceExprs,
                             AstList contents);

  inline int numInterfaceExprs() const { return numInterfaceExprs_; }

  /**
    Return the ith interface implemented as part of this record's declaration.
   */
  const AstNode* interfaceExpr(int i) const {
    if (interfaceExprChildNum_ < 0 || i >= numInterfaceExprs_)
      return nullptr;

    auto ret = child(interfaceExprChildNum_ + i);
    return ret;
  }

  AstListNoCommentsIteratorPair<AstNode> interfaceExprs() const {
    if (interfaceExprChildNum_ < 0)
      return AstListNoCommentsIteratorPair<AstNode>(
                children_.end(), children_.end());

    return AstListNoCommentsIteratorPair<AstNode>(
              children_.begin() + interfaceExprChildNum_,
              children_.begin() + interfaceExprChildNum_ + numInterfaceExprs_);
  }

  void serialize(Serializer& ser) const override {
    AggregateDecl::serialize(ser);
    ser.write(interfaceExprChildNum_);
    ser.write(numInterfaceExprs_);
  }

  DECLARE_STATIC_DESERIALIZE(Record);
};


} // end namespace uast
} // end namespace chpl

#endif
