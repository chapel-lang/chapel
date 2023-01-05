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

#ifndef CHPL_UAST_DECL_H
#define CHPL_UAST_DECL_H

#include "chpl/uast/Attributes.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for declarations.
  Note that most Decls inherit from NamedDecl,
  however these declarations might be contained in MultiDecl or TupleDecl.
 */
class Decl : public AstNode {
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

  // Use -1 to indicate that there is no such child.
  int attributesChildNum_;
  Visibility visibility_;
  Linkage linkage_;
  int linkageNameChildNum_;

 protected:
  Decl(AstTag tag, int attributesChildNum, Visibility visibility,
       Linkage linkage)
    : AstNode(tag),
      attributesChildNum_(attributesChildNum),
      visibility_(visibility),
      linkage_(linkage),
      linkageNameChildNum_(-1) {
  }

  Decl(AstTag tag, AstList children, int attributesChildNum,
       Visibility visibility,
       Linkage linkage,
       int linkageNameChildNum)
    : AstNode(tag, std::move(children)),
      attributesChildNum_(attributesChildNum),
      visibility_(visibility),
      linkage_(linkage),
      linkageNameChildNum_(linkageNameChildNum) {


    if (linkageNameChildNum_ >= 0) {
      CHPL_ASSERT(linkage_ != DEFAULT_LINKAGE);
    }

    CHPL_ASSERT(-1 <= attributesChildNum_ &&
                 attributesChildNum_ < (ssize_t)children_.size());

    if (attributesChildNum_ >= 0) {
      CHPL_ASSERT(child(attributesChildNum_)->isAttributes());
    }

    CHPL_ASSERT(-1 <= linkageNameChildNum_ &&
                 linkageNameChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(-1 <= linkageNameChildNum_ &&
                 linkageNameChildNum_ < (ssize_t)children_.size());
  }

  bool declContentsMatchInner(const Decl* other) const {
    return this->visibility_ == other->visibility_ &&
           this->linkage_ == other->linkage_ &&
           this->linkageNameChildNum_ == other->linkageNameChildNum_ &&
           this->attributesChildNum_ == other->attributesChildNum_;
  }

  void declMarkUniqueStringsInner(Context* context) const {
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

  int attributesChildNum() const {
    return attributesChildNum_;
  }

 public:
  virtual ~Decl() = 0; // this is an abstract base class

  /**
    Return the visibility of this declaration, e.g. "PUBLIC" or "PRIVATE".
  */
  Visibility visibility() const {
    return visibility_;
  }

  /**
    Return the linkage of this declaration, e.g. "EXTERN" or "EXPORT".
  */
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
  const AstNode* linkageName() const {
    if (linkageNameChildNum_ < 0) return nullptr;
    auto ret = child(linkageNameChildNum_);
    return ret;
  }

  /**
    Return the attributes associated with this declaration, or nullptr
    if none exist.
  */
  const Attributes* attributes() const {
    if (attributesChildNum_ < 0) return nullptr;
    auto ret = child(attributesChildNum_);
    CHPL_ASSERT(ret->isAttributes());
    return (const Attributes*)ret;
  }

  /**
    Convert Decl::Visibility to a string
    */
  static const char* visibilityToString(Visibility v);

  /**
    Convert Decl::Linkage to a string
    */
  static const char* linkageToString(Linkage x);
};


} // end namespace uast
} // end namespace chpl

namespace std {

template<> struct hash<chpl::uast::Decl::Visibility>
{
  size_t operator()(const chpl::uast::Decl::Visibility& key) const {
    return (size_t) key;
  }
};

} // end namespace std

#endif
