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

#ifndef CHPL_UAST_INTERFACE_H
#define CHPL_UAST_INTERFACE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Qualifier.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents an interface.

  \rst
  .. code-block:: chapel

      interface example(T) {
        operator ==(a: T, b: T): bool;
      }

  \endrst

  Creates an interface named 'example' with one interface formal named 'T'.
  The interface body contains one required function named '=='.
*/
class Interface final : public NamedDecl {
 friend class AstNode;

 private:
  int interfaceFormalsChildNum_;
  int numInterfaceFormals_;
  int bodyChildNum_;
  int numBodyStmts_; // TODO is this field necessary?
                     // isn't the body always the last thing here?
  bool isFormalListExplicit_;

  Interface(AstList children, int attributeGroupChildNum,
            Visibility visibility,
            UniqueString name,
            int interfaceFormalsChildNum,
            int numInterfaceFormals,
            int bodyChildNum,
            int numBodyStmts,
            bool isFormalListExplicit)
      : NamedDecl(asttags::Interface, std::move(children),
                  attributeGroupChildNum,
                  visibility,
                  Decl::DEFAULT_LINKAGE,
                  /*linkageNameChildNum*/ AstNode::NO_CHILD,
                  name),
        interfaceFormalsChildNum_(interfaceFormalsChildNum),
        numInterfaceFormals_(numInterfaceFormals),
        bodyChildNum_(bodyChildNum),
        numBodyStmts_(numBodyStmts),
        isFormalListExplicit_(isFormalListExplicit) {
    // TODO: Some assertions here...
  }

  void serializeInner(Serializer& ser) const override {
    namedDeclSerializeInner(ser);
    ser.writeVInt(interfaceFormalsChildNum_);
    ser.writeVInt(numInterfaceFormals_);
    ser.writeVInt(bodyChildNum_);
    ser.writeVInt(numBodyStmts_);
    ser.write(isFormalListExplicit_);
  }

  explicit Interface(Deserializer& des)
    : NamedDecl(asttags::Interface, des) {
      interfaceFormalsChildNum_ = des.readVInt();
      numInterfaceFormals_ = des.readVInt();
      bodyChildNum_ = des.readVInt();
      numBodyStmts_ = des.readVInt();
      isFormalListExplicit_ = des.read<bool>();
    }

  bool contentsMatchInner(const AstNode* other) const override {
    const Interface* lhs = this;
    const Interface* rhs = (const Interface*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
      lhs->interfaceFormalsChildNum_ == rhs->interfaceFormalsChildNum_ &&
      lhs->numInterfaceFormals_ == rhs->numInterfaceFormals_ &&
      lhs->bodyChildNum_ == rhs->bodyChildNum_ &&
      lhs->numBodyStmts_ == rhs->numBodyStmts_ &&
      lhs->isFormalListExplicit_ == rhs->isFormalListExplicit_;
  }

  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Interface() override = default;

  /**
    Returns 'true' if this interface has a formal list.

    For the following:

    \rst
    .. code-block:: chapel

        interface example(T) {
          operator ==(a: T, b: T): bool;
        }

    \endrst

    The formal list '(T)' is present, so this method returns 'true'.
  */
  bool isFormalListExplicit() const {
    return isFormalListExplicit_;
  }

  /**
    Iterate over the formals in the formal list.
  */
  AstListIteratorPair<AstNode> formals() const {
    auto begin = children_.begin() + interfaceFormalsChildNum_;
    auto end = begin + numInterfaceFormals_;
    return AstListIteratorPair<AstNode>(begin, end);
  }

  /**
    Return the number of interface formals. For the following:

    \rst
    .. code-block:: chapel

        interface example(T) {
          operator ==(a: T, b: T): bool;
        }

    \endrst

    This method would return '1'.
  */
  int numFormals() const {
    return numInterfaceFormals_;
  }

  /**
    Return the i'th interface formal.
  */
  const AstNode* formal(int i) const {
    CHPL_ASSERT(i >= 0 && i < numBodyStmts_);
    auto ret = child(i + interfaceFormalsChildNum_);
    CHPL_ASSERT(ret);
    return ret;
  }

  /**
    Iterate over the statements in the body of this interface.
  */
  AstListIteratorPair<AstNode> stmts() const {
    auto begin = children_.begin() + bodyChildNum_;
    auto end = begin + numBodyStmts_;
    return AstListIteratorPair<AstNode>(begin, end);
  }

  /**
    Return the number of statements in the body of this interface.
  */
  int numStmts() const {
    return numBodyStmts_;
  }

  /**
    Get the i'th statement in the body of this interface.
  */
  const AstNode* stmt(int i) const {
    CHPL_ASSERT(i >= 0 && i < numBodyStmts_);
    auto ret = child(i + bodyChildNum_);
    CHPL_ASSERT(ret);
    return ret;
  }

  static owned<Interface> build(Builder* builder, Location loc,
                                owned<AttributeGroup> attributeGroup,
                                Decl::Visibility visibility,
                                UniqueString name,
                                bool isFormalListExplicit,
                                AstList formals,
                                AstList body);
};


} // end namespace uast
} // end namespace chpl

#endif
