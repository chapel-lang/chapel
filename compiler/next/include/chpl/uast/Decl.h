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
  Note that most Decls inherit from NamedDecl,
  however these declarations might be contained in MultiDecl or TupleDecl.
 */
class Decl : public Expression {
 public:
  enum Visibility {
    DEFAULT_VISIBILITY,
    PUBLIC,
    PRIVATE,
  };

  enum Linkage {
    DEFAULT_LINKAGE,
    EXTERN,
    EXPORT
  };

 private:
  Visibility visibility_;
  Linkage linkage_;
  int linkageNameChildNum_;

 protected:
  Decl(ASTTag tag, Visibility visibility, Linkage linkage)
    : Expression(tag),
      visibility_(visibility),
      linkage_(linkage),
      linkageNameChildNum_(-1) {
  }

  Decl(ASTTag tag, ASTList children, Visibility visibility, Linkage linkage,
       int linkageNameChildNum)
    : Expression(tag, std::move(children)),
      visibility_(visibility),
      linkage_(linkage),
      linkageNameChildNum_(linkageNameChildNum) {

    if (linkageNameChildNum_ >= 0) {
      assert(linkage_ != DEFAULT_LINKAGE);
    }

    assert(-1 <= linkageNameChildNum_ &&
                 linkageNameChildNum_ < (ssize_t)children_.size());
    assert(-1 <= linkageNameChildNum_ &&
                 linkageNameChildNum_ < (ssize_t)children_.size());
  }

  bool declContentsMatchInner(const Decl* other) const {
    return this->visibility_ == other->visibility_ &&
           this->linkage_ == other->linkage_ &&
           this->linkageNameChildNum_ == other->linkageNameChildNum_ &&
           expressionContentsMatchInner(other);
  }

  void declMarkUniqueStringsInner(Context* context) const {
    expressionMarkUniqueStringsInner(context);
  }

 public:
  virtual ~Decl() = 0; // this is an abstract base class

  Visibility visibility() const {
    return visibility_;
  }

  Linkage linkage() const {
    return linkage_;
  }

  /**
   Return the linkage name expression, e.g. "f_c_name"
   in the below, or nullptr if there is none.

   \rst
    .. code-block:: chapel

        extern "f_c_name" proc f(arg) { }
    \endrst
   */
  const Expression* linkageName() const {
    if (linkageNameChildNum_ < 0) return nullptr;
    auto ret = child(linkageNameChildNum_);
    assert(ret->isExpression());
    return (Expression*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
