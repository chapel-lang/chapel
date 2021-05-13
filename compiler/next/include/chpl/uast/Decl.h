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

#ifndef CHPL_UAST_DECL_H
#define CHPL_UAST_DECL_H

#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for declarations.
  Note that all individual Syms are declared by a class inheriting from SymDecl;
  however these declarations might be contained in something else
  (e.g. a TupleDecl or MultiDecl).
 */
class Decl : public Expression {
 protected:
  Decl(ASTTag tag, ASTList children)
    : Expression(tag, std::move(children)) {
  }
  bool declContentsMatchInner(const Decl* other) const {
    return expressionContentsMatchInner(other);
  }
  void declMarkUniqueStringsInner(Context* context) const {
    expressionMarkUniqueStringsInner(context);
  }

 public:
  virtual ~Decl() = 0; // this is an abstract base class
};


} // end namespace uast
} // end namespace chpl

#endif
