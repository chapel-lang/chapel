/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_AGGREGATEDECL_H
#define CHPL_UAST_AGGREGATEDECL_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/TypeDecl.h"

namespace chpl {
namespace uast {


/**
  This is the parent class combining functionality for class, record, and union
  declarations.

  The contained decls can be:
    * Variable
    * Function
    * TupleDecl
    * MultiDecl
    * TypeDecl
    * Comment

 */
class AggregateDecl : public TypeDecl {
 private:
  int elementsChildNum_;
  int numElements_;

  static bool validAggregateChildren(AstListIteratorPair<AstNode> it);

 protected:
  bool aggregateDeclContentsMatchInner(const AggregateDecl* other) const {
    const AggregateDecl* lhs = this;
    const AggregateDecl* rhs = other;
    return lhs->typeDeclContentsMatchInner(rhs) &&
           lhs->elementsChildNum_ == rhs->elementsChildNum_ &&
           lhs->numElements_ == rhs->numElements_;
  }

  void aggregateDeclMarkUniqueStringsInner(Context* context) const {
    typeDeclMarkUniqueStringsInner(context);
  }

 public:
  AggregateDecl(AstTag tag, AstList children, int attributeGroupChildNum,
                Decl::Visibility vis,
                Decl::Linkage linkage,
                int linkageNameChildNum,
                UniqueString name,
                int elementsChildNum,
                int numElements)
    : TypeDecl(tag, std::move(children), attributeGroupChildNum, vis, linkage,
               linkageNameChildNum,
               name),
      elementsChildNum_(elementsChildNum),
      numElements_(numElements) {

    CHPL_ASSERT(validAggregateChildren(declOrComments()));
  }

  AggregateDecl(AstTag tag, Deserializer& des)
    : TypeDecl(tag, des) {
    elementsChildNum_ = des.read<int>();
    numElements_ = des.read<int>();
  }

  ~AggregateDecl() = 0; // this is an abstract base class

  /**
    Return a way to iterate over the contained Decls and Comments.
   */
  AstListIteratorPair<AstNode> declOrComments() const {
    if (elementsChildNum_ < 0)
      return AstListIteratorPair<AstNode>(children_.end(), children_.end());

    return AstListIteratorPair<AstNode>(
              children_.begin() + elementsChildNum_,
              children_.begin() + elementsChildNum_ + numElements_);
  }

  /**
   Return the number of Decls and Comments contained in this AggregateDecl.
   */
  int numDeclOrComments() const {
    return numElements_;
  }

  /**
   Return the i'th Decl in this AggregateDecl.
   */
  const AstNode* declOrComment(int i) const {
    CHPL_ASSERT(0 <= i && i < numElements_);
    const AstNode* ast = this->child(elementsChildNum_ + i);
    return ast;
  }

  /**
   Return a way to iterate over the contained Decls (ignoring Comments)
   */
  AstListNoCommentsIteratorPair<Decl> decls() const {
    if (elementsChildNum_ < 0)
      return AstListNoCommentsIteratorPair<Decl>(
                children_.end(), children_.end());

    return AstListNoCommentsIteratorPair<Decl>(
              children_.begin() + elementsChildNum_,
              children_.begin() + elementsChildNum_ + numElements_);
  }

  void serialize(Serializer& ser) const override {
    TypeDecl::serialize(ser);
    ser.write(elementsChildNum_);
    ser.write(numElements_);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
