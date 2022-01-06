/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_TYPEQUERY_H
#define CHPL_UAST_TYPEQUERY_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a type query.

  \rst
  .. code-block:: chapel

      // Here ?D is a type query which names the domain of A.
      proc foo(A: [?D] int) {
        for idx in D do writeln(idx, A[idx]);
      }

  \endrst
 */
class TypeQuery final : public Expression {

 private:
  UniqueString name_;

  // TODO: Intercept TQUESTION productions so that TypeQuery can be empty?
  TypeQuery(UniqueString name)
    : Expression(asttags::TypeQuery), name_(name) {
    assert(!name.isEmpty() && name.c_str()[0] != '?');
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const TypeQuery* lhs = this;
    const TypeQuery* rhs = (const TypeQuery*) other;
    return lhs->expressionContentsMatchInner(rhs) &&
           lhs->name_ == rhs->name_;
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
    this->name_.mark(context);
  }

 public:
  ~TypeQuery() override = default;

  static owned<TypeQuery> build(Builder* builder, Location loc,
                                UniqueString name);

  /**
    Returns the name of this type query, e.g. in `proc foo(?x)`, this
    method would return the `x` portion of the type query `?x`.
  */
  UniqueString name() const {
    return name_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
