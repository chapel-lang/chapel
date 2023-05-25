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

#ifndef CHPL_UAST_UNION_H
#define CHPL_UAST_UNION_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AggregateDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a union declaration. For example:

  \rst
  .. code-block:: chapel

      union myUnion {
        var a: int;
        var b: real;
      }

  \endrst

  The union itself (myUnion) is represented by a Union AST node.
 */
class Union final : public AggregateDecl {
 private:
  Union(AstList children, int attributeGroupChildNum, Decl::Visibility vis,
        Decl::Linkage linkage,
        int linkageNameChildNum,
        UniqueString name,
        int elementsChildNum,
        int numElements)
    : AggregateDecl(asttags::Union, std::move(children),
                    attributeGroupChildNum,
                    vis,
                    linkage,
                    linkageNameChildNum,
                    name,
                    elementsChildNum,
                    numElements) {

    // Cannot export unions right now, this should be a parse error.
    CHPL_ASSERT(linkage != Decl::EXPORT);
  }

  Union(Deserializer& des)
    : AggregateDecl(asttags::Union, des) { }


  bool contentsMatchInner(const AstNode* other) const override {
    const Union* lhs = this;
    const Union* rhs = (const Union*) other;
    return lhs->aggregateDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    aggregateDeclMarkUniqueStringsInner(context);
  }

 public:
  ~Union() override = default;

  static owned<Union> build(Builder* builder, Location loc,
                            owned<AttributeGroup> attributeGroup,
                            Decl::Visibility vis,
                            Decl::Linkage linkage,
                            owned<AstNode> linkageName,
                            UniqueString name,
                            AstList contents);

  void serialize(Serializer& ser) const override {
    AggregateDecl::serialize(ser);
  }

  DECLARE_STATIC_DESERIALIZE(Union);

};


} // end namespace uast
} // end namespace chpl

#endif
