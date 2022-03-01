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

#ifndef CHPL_UAST_INTERFACE_H
#define CHPL_UAST_INTERFACE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents an interface.

  \rst
  .. code-block:: chapel

      interface example(T) {
        // TODO: What goes in the body?
      }

  \endrst

  Creates an interface named example with a constraint 'T'.
 */
class Interface final : public NamedDecl {
 private:
  int interfaceFormalsChildNum_;
  int numInterfaceFormals_;
  int bodyChildNum_;
  int numBodyStmts_;

  Interface(ASTList children, int attributesChildNum,
            Visibility visibility,
            UniqueString name,
            int interfaceFormalsChildNum,
            int numInterfaceFormals,
            int bodyChildNum,
            int numBodyStmts)
      : NamedDecl(asttags::Interface, std::move(children),
                  attributesChildNum,
                  visibility,
                  Decl::DEFAULT_LINKAGE,
                  /*linkageNameChildNum*/ -1,
                  name),
        interfaceFormalsChildNum_(interfaceFormalsChildNum),
        numInterfaceFormals_(numInterfaceFormals),
        bodyChildNum_(bodyChildNum),
        numBodyStmts_(numBodyStmts) {
    // TODO: Some assertions here...
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Interface* lhs = this;
    const Interface* rhs = (const Interface*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
      lhs->interfaceFormalsChildNum_ == rhs->interfaceFormalsChildNum_ &&
      lhs->numInterfaceFormals_ == rhs->numInterfaceFormals_ &&
      lhs->bodyChildNum_ == rhs->bodyChildNum_ &&
      lhs->numBodyStmts_ == rhs->numBodyStmts_;
  }

  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

 public:
  ~Interface() override = default;

  static owned<Interface> build(Builder* builder, Location loc,
                                owned<Attributes> attributes,
                                Decl::Visibility visibility,
                                UniqueString name,
                                ASTList formals,
                                ASTList bbody);
};


} // end namespace uast
} // end namespace chpl

#endif
