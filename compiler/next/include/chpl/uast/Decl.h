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

#include "chpl/uast/Exp.h"
#include "chpl/uast/Sym.h"
#include "chpl/queries/UniqueString.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for declarations
 */
class Decl : public Exp {
 protected:
  Decl(ASTTag tag, owned<Sym> symbol);
  bool declContentsMatchInner(const Decl* other) const {
    return true;
  }
  void declMarkUniqueStringsInner(Context* context) const {
  }

 public:
  virtual ~Decl() = 0; // this is an abstract base class

  /** Returns the symbol declared by the declaration. */
  const Sym* sym() const {
    const ASTBase* ast = child(0);
    assert(ast->isSym());
    return (const Sym*) ast;
  }
  UniqueString name() const {
    return sym()->name();
  }
  Sym::Visibility visibility() const {
    return sym()->visibility();
  }
};


} // end namespace uast
} // end namespace chpl

#endif
