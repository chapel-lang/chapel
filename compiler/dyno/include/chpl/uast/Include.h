/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_INCLUDE_H
#define CHPL_UAST_INCLUDE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents an include statement. For example:

  \rst
  .. code-block:: chapel

      module Foo {
        // Example 1:
        include private module Memory;
      }

  \endrst

  Includes the contents of the module Memory so that they are privately
  visible within the module Foo.
*/
class Include final : public Expression {
 private:
  Decl::Visibility visibility_;
  bool isPrototype_;
  UniqueString name_;

  Include(Decl::Visibility visibility, bool isPrototype, UniqueString name)
    : Expression(asttags::Include),
      visibility_(visibility),
      isPrototype_(isPrototype),
      name_(name) {
    assert(!name_.isEmpty());
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Include* lhs = this;
    const Include* rhs = other->toInclude();
    return lhs->visibility_ == rhs->visibility_ &&
      lhs->isPrototype_ == rhs->isPrototype_ &&
      lhs->name_ == rhs->name_ &&
      lhs->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    name_.mark(context);
    expressionMarkUniqueStringsInner(context);
  }

 public:

  /**
    Create and return an include statement.
  */
  static owned<Include> build(Builder* builder, Location loc,
                              Decl::Visibility visibility,
                              bool isPrototype,
                              UniqueString name);

  /**
    Return the visibility of this include statement.
  */
  Decl::Visibility visibility() const {
    return visibility_;
  }

  /**
    Returns 'true' if this include statement is for a prototype module.
  */
  bool isPrototype() const {
    return isPrototype_;
  }

  /**
    Return the name of the module included by this include statement.
  */
  UniqueString name() const {
    return name_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
