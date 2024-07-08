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
 friend class AstNode;

 private:
  Record(AstList children, int attributeGroupChildNum, Decl::Visibility vis,
         Decl::Linkage linkage,
         int linkageNameChildNum,
         UniqueString name,
         int inheritExprChildNum,
         int numInheritExprs,
         int elementsChildNum,
         int numElements)
    : AggregateDecl(asttags::Record, std::move(children),
                    attributeGroupChildNum,
                    vis,
                    linkage,
                    linkageNameChildNum,
                    name,
                    inheritExprChildNum,
                    numInheritExprs,
                    elementsChildNum,
                    numElements) {}

  void serializeInner(Serializer& ser) const override {
    aggregateDeclSerializeInner(ser);
  }

  explicit Record(Deserializer& des) : AggregateDecl(asttags::Record, des) { }

  bool contentsMatchInner(const AstNode* other) const override {
    const Record* lhs = this;
    const Record* rhs = (const Record*) other;
    return lhs->aggregateDeclContentsMatchInner(rhs);
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
                             AstList inheritExprs,
                             AstList contents);
};


} // end namespace uast
} // end namespace chpl

#endif
