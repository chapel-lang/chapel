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

#ifndef CHPL_UAST_CLASS_H
#define CHPL_UAST_CLASS_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AggregateDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a class declaration. For example:

  \rst
  .. code-block:: chapel

      class MyClass : ParentClass {
        var a: int;
        proc method() { }
      }

  \endrst

  The class itself (MyClass) is represented by a Class AST node.
 */
class Class final : public AggregateDecl {
 friend class AstNode;

 private:
  Class(AstList children, int attributeGroupChildNum, Decl::Visibility vis,
        UniqueString name,
        int parentClassChildNum,
        int numParentClasses,
        int elementsChildNum,
        int numElements)
    : AggregateDecl(asttags::Class, std::move(children),
                    attributeGroupChildNum,
                    vis,
                    Decl::DEFAULT_LINKAGE,
                    /*linkageNameChildNum*/ NO_CHILD,
                    name,
                    parentClassChildNum,
                    numParentClasses,
                    elementsChildNum,
                    numElements) {}

  void serializeInner(Serializer& ser) const override {
    aggregateDeclSerializeInner(ser);
  }

  explicit Class(Deserializer& des)
    : AggregateDecl(asttags::Class, des) { }

  bool contentsMatchInner(const AstNode* other) const override {
    const Class* lhs = this;
    const Class* rhs = (const Class*) other;
    return lhs->aggregateDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    aggregateDeclMarkUniqueStringsInner(context);
  }

  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Class() override = default;

  static owned<Class> build(Builder* builder, Location loc,
                            owned<AttributeGroup> attributeGroup,
                            Decl::Visibility vis,
                            UniqueString name,
                            AstList inheritExprs,
                            AstList contents);
};


} // end namespace uast
} // end namespace chpl

#endif
