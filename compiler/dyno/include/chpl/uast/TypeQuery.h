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

#ifndef CHPL_UAST_TYPEQUERY_H
#define CHPL_UAST_TYPEQUERY_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/NamedDecl.h"

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

  It is a NamedDecl because, as you can see above, D is available
  within the body of `foo`. So, `?D` is represented as a TypeQuery
  with name `D`.

  Note that, at present, `proc bar(arg: ?)` uses an Identifier `?`
  rather than a TypeQuery.
 */
class TypeQuery final : public NamedDecl {

 private:
  TypeQuery(UniqueString name)
    : NamedDecl(asttags::TypeQuery, DEFAULT_VISIBILITY, DEFAULT_LINKAGE,
                /* attributesChildNum */ -1, name) {
    assert(!name.isEmpty() && name.c_str()[0] != '?');
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const TypeQuery* lhs = this;
    const TypeQuery* rhs = (const TypeQuery*) other;
    return lhs->namedDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

 public:
  ~TypeQuery() override = default;

  static owned<TypeQuery> build(Builder* builder, Location loc,
                                UniqueString name);
};


} // end namespace uast
} // end namespace chpl

#endif
