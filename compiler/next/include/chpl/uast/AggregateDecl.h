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

#ifndef CHPL_UAST_AGGREGATEDECL_H
#define CHPL_UAST_AGGREGATEDECL_H

#include "chpl/queries/Location.h"
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
    * Comment

 */
class AggregateDecl : public TypeDecl {
 private:
  int elementsChildNum_;
  int numElements_;

  static bool validAggregateChildren(ASTListIteratorPair<Expression> it);

 public:
  AggregateDecl(ASTTag tag, ASTList children, Visibility vis, UniqueString name,
                int elementsChildNum,
                int numElements)
    : TypeDecl(tag, std::move(children), vis, name),
      elementsChildNum_(elementsChildNum),
      numElements_(numElements) {

    assert(validAggregateChildren(declOrComments()));
  }
  ~AggregateDecl() = 0; // this is an abstract base class

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

  /**
    Return a way to iterate over the contained Decls and Comments.
   */
  ASTListIteratorPair<Expression> declOrComments() const {
    if (elementsChildNum_ < 0)
      return ASTListIteratorPair<Expression>(children_.end(), children_.end());

    return ASTListIteratorPair<Expression>(
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
  const Expression* declOrComment(int i) const {
    assert(0 <= i && i < numElements_);
    const ASTNode* ast = this->child(elementsChildNum_ + i);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }

  /**
   Return a way to iterate over the contained Decls (ignoring Comments)
   */
  ASTListNoCommentsIteratorPair<Decl> decls() const {
    if (elementsChildNum_ < 0)
      return ASTListNoCommentsIteratorPair<Decl>(
                children_.end(), children_.end());

    return ASTListNoCommentsIteratorPair<Decl>(
              children_.begin() + elementsChildNum_,
              children_.begin() + elementsChildNum_ + numElements_);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
