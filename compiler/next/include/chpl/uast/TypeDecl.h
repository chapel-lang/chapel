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

#ifndef CHPL_UAST_TYPEDECL_H
#define CHPL_UAST_TYPEDECL_H

#include "chpl/uast/NamedDecl.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for Symbols defining a type
  (e.g. classes, records, enums).
 */
class TypeDecl : public NamedDecl {
 protected:
  TypeDecl(asttags::ASTTag tag, ASTList children,
           Decl::Visibility vis, UniqueString name)
    : NamedDecl(tag, std::move(children), vis, name) {

    assert(isExpressionASTList(children_));
  }
  bool typeDeclContentsMatchInner(const TypeDecl* other) const {
    return namedDeclContentsMatchInner(other);
  }
  void typeDeclMarkUniqueStringsInner(Context* context) const {
    namedDeclMarkUniqueStringsInner(context); 
  }

 public:
  virtual ~TypeDecl() = 0; // this is an abstract base class
};


} // end namespace uast
} // end namespace chpl

#endif
