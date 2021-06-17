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

#ifndef CHPL_UAST_CLASS_H
#define CHPL_UAST_CLASS_H

#include "chpl/queries/Location.h"
#include "chpl/uast/AggregateDecl.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"

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

  The class itself (MyClass) is represented by a Class AST node. The Class
  AST node contains Field and Function children.
 */
class Class final : public AggregateDecl {
 private:
  int parentClassChildNum_;

  Class(ASTList children, Visibility vis, UniqueString name,
        int elementsChildNum,
        int numElements,
        int parentClassChildNum)
    : AggregateDecl(asttags::Class, std::move(children), vis, name,
                    elementsChildNum, numElements),
      parentClassChildNum_(parentClassChildNum) {
    assert(parentClassChildNum_ == -1 ||
           child(parentClassChildNum_)->isIdentifier());
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Class* lhs = this;
    const Class* rhs = (const Class*) other;
    return lhs->aggregateDeclContentsMatchInner(rhs) &&
           lhs->parentClassChildNum_ == rhs->parentClassChildNum_;
  }
  void markUniqueStringsInner(Context* context) const override {
    aggregateDeclMarkUniqueStringsInner(context);
  }

 public:
  ~Class() override = default;

  static owned<Class> build(Builder* builder, Location loc,
                            Decl::Visibility vis, UniqueString name,
                            owned<Identifier> parentClass,
                            ASTList contents);

  /**
    Return the Identifier indicating the parent class or nullptr
    if there was none.
   */
  const Identifier* parentClass() {
    if (parentClassChildNum_ < 0)
      return nullptr;

    auto ret = child(parentClassChildNum_);
    assert(ret->isIdentifier());
    return (const Identifier*)ret;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
