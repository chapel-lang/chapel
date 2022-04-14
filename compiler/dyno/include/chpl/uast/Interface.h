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
  bool isFormalListPresent_;

  Interface(AstList children, int attributesChildNum,
            Visibility visibility,
            UniqueString name,
            int interfaceFormalsChildNum,
            int numInterfaceFormals,
            int bodyChildNum,
            int numBodyStmts,
            bool isFormalListPresent)
      : NamedDecl(asttags::Interface, std::move(children),
                  attributesChildNum,
                  visibility,
                  Decl::DEFAULT_LINKAGE,
                  /*linkageNameChildNum*/ AstNode::NO_CHILD,
                  name),
        interfaceFormalsChildNum_(interfaceFormalsChildNum),
        numInterfaceFormals_(numInterfaceFormals),
        bodyChildNum_(bodyChildNum),
        numBodyStmts_(numBodyStmts),
        isFormalListPresent_(isFormalListPresent) {
    // TODO: Some assertions here...
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Interface* lhs = this;
    const Interface* rhs = (const Interface*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
      lhs->interfaceFormalsChildNum_ == rhs->interfaceFormalsChildNum_ &&
      lhs->numInterfaceFormals_ == rhs->numInterfaceFormals_ &&
      lhs->bodyChildNum_ == rhs->bodyChildNum_ &&
      lhs->numBodyStmts_ == rhs->numBodyStmts_ &&
      lhs->isFormalListPresent_ == rhs->isFormalListPresent_;
  }

  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

 public:
  ~Interface() override = default;

  bool isFormalListPresent() const {
    return isFormalListPresent_;
  }

  AstListIteratorPair<AstNode> formals() const {
    auto begin = children_.begin() + interfaceFormalsChildNum_;
    auto end = begin + numInterfaceFormals_;
    return AstListIteratorPair<AstNode>(begin, end);
  }

  int numFormals() const {
    return numInterfaceFormals_;
  }

  const AstNode* formal(int i) const {
    assert(i >= 0 && i < numBodyStmts_);
    auto ret = stmt(i + interfaceFormalsChildNum_);
    assert(ret);
    return ret;
  }

  AstListIteratorPair<AstNode> stmts() const {
    auto begin = children_.begin() + bodyChildNum_;
    auto end = begin + numBodyStmts_;
    return AstListIteratorPair<AstNode>(begin, end);
  }

  int numStmts() const {
    return numBodyStmts_;
  }

  const AstNode* stmt(int i) const {
    assert(i >= 0 && i < numBodyStmts_);
    auto ret = stmt(i + bodyChildNum_);
    assert(ret);
    return ret;
  }

  static owned<Interface> build(Builder* builder, Location loc,
                                owned<Attributes> attributes,
                                Decl::Visibility visibility,
                                UniqueString name,
                                bool isFormalListPresent,
                                AstList formals,
                                AstList body);
};


} // end namespace uast
} // end namespace chpl

#endif
