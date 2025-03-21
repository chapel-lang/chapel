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
 friend class AstNode;

 private:
  int inheritExprChildNum_;
  int numInheritExprs_;
  int elementsChildNum_;
  int numElements_;

  static bool validAggregateChildren(AstListIteratorPair<AstNode> it);

 protected:
  bool aggregateDeclContentsMatchInner(const AggregateDecl* other) const {
    const AggregateDecl* lhs = this;
    const AggregateDecl* rhs = other;
    return lhs->typeDeclContentsMatchInner(rhs) &&
           lhs->inheritExprChildNum_ == rhs->inheritExprChildNum_ &&
           lhs->numInheritExprs_ == rhs->numInheritExprs_ &&
           lhs->elementsChildNum_ == rhs->elementsChildNum_ &&
           lhs->numElements_ == rhs->numElements_;
  }

  void aggregateDeclMarkUniqueStringsInner(Context* context) const {
    typeDeclMarkUniqueStringsInner(context);
  }

  std::string aggregateDeclDumpChildLabelInner(int i) const;

  AggregateDecl(AstTag tag, AstList children, int attributeGroupChildNum,
                Decl::Visibility vis,
                Decl::Linkage linkage,
                int linkageNameChildNum,
                UniqueString name,
                int inheritExprChildNum,
                int numInheritExprs,
                int elementsChildNum,
                int numElements)
    : TypeDecl(tag, std::move(children), attributeGroupChildNum, vis, linkage,
               linkageNameChildNum,
               name),
      inheritExprChildNum_(inheritExprChildNum),
      numInheritExprs_(numInheritExprs),
      elementsChildNum_(elementsChildNum),
      numElements_(numElements) {

    if (inheritExprChildNum_ != NO_CHILD && elementsChildNum != NO_CHILD) {
      CHPL_ASSERT(inheritExprChildNum_ + numInheritExprs_ ==
                  elementsChildNum);
    }

    // Don't validate inherit expressions here, they're checked post-parse.

    CHPL_ASSERT(validAggregateChildren(declOrComments()));
  }

  void aggregateDeclSerializeInner(Serializer& ser) const {
    typeDeclSerializeInner(ser);
    ser.writeVInt(inheritExprChildNum_);
    ser.writeVInt(numInheritExprs_);
    ser.writeVInt(elementsChildNum_);
    ser.writeVInt(numElements_);
  }

  AggregateDecl(AstTag tag, Deserializer& des)
    : TypeDecl(tag, des) {
    inheritExprChildNum_ = des.readVInt();
    numInheritExprs_ = des.readVInt();
    elementsChildNum_ = des.readVInt();
    numElements_ = des.readVInt();
  }

  ~AggregateDecl() = 0; // this is an abstract base class

 public:
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

  /**
   Return the number of inherit exprs (parent classes or implemented interfaces)
   in this aggregate type.
   */
  inline int numInheritExprs() const { return numInheritExprs_; }

  /**
    Return the ith interface implemented as part of this record's declaration.
   */
  const AstNode* inheritExpr(int i) const {
    if (inheritExprChildNum_ < 0 || i >= numInheritExprs_)
      return nullptr;

    auto ret = child(inheritExprChildNum_ + i);
    return ret;
  }

  /**
    Return an iterator of all the inherit exprs (parent classes or implemented
    interfaces).
   */
  AstListNoCommentsIteratorPair<AstNode> inheritExprs() const {
    if (inheritExprChildNum_ < 0)
      return AstListNoCommentsIteratorPair<AstNode>(
                children_.end(), children_.end());

    return AstListNoCommentsIteratorPair<AstNode>(
              children_.begin() + inheritExprChildNum_,
              children_.begin() + inheritExprChildNum_ + numInheritExprs_);
  }

  /** Returns the inherited Identifier or Dot, including considering
      one marked generic with Superclass(?) */
  static const AstNode* getUnwrappedInheritExpr(const AstNode* ast,
                                            bool& markedGeneric);
};


} // end namespace uast
} // end namespace chpl

#endif
