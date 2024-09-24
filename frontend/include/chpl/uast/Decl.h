/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/AttributeGroup.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for declarations.
  Note that most Decls inherit from NamedDecl,
  however these declarations might be contained in MultiDecl or TupleDecl.
 */
class Decl : public AstNode {
 friend class AstNode;

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
  int destinationChildNum_;

 protected:
  Decl(AstTag tag, Visibility visibility,
       Linkage linkage)
    : AstNode(tag),
      visibility_(visibility),
      linkage_(linkage),
      linkageNameChildNum_(NO_CHILD),
      destinationChildNum_(NO_CHILD) {
  }

  Decl(AstTag tag, AstList children, int attributeGroupChildNum,
       Visibility visibility,
       Linkage linkage,
       int linkageNameChildNum)
    : AstNode(tag, std::move(children), attributeGroupChildNum),
      visibility_(visibility),
      linkage_(linkage),
      linkageNameChildNum_(linkageNameChildNum),
      destinationChildNum_(NO_CHILD) {


    if (linkageNameChildNum_ >= 0) {
      CHPL_ASSERT(linkage_ != DEFAULT_LINKAGE);
    }

    CHPL_ASSERT(NO_CHILD <= linkageNameChildNum_ &&
                 linkageNameChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(NO_CHILD <= destinationChildNum_ &&
                 destinationChildNum_ < (ssize_t)children_.size());
  }

  void declSerializeInner(Serializer& ser) const {
    ser.write(visibility_);
    ser.write(linkage_);
    ser.writeVInt(linkageNameChildNum_);
    ser.writeVInt(destinationChildNum_);
  }

  Decl(AstTag tag, Deserializer& des)
    : AstNode(tag, des) {
      visibility_ = des.read<Decl::Visibility>();
      linkage_ = des.read<Decl::Linkage>();
      linkageNameChildNum_ = des.readVInt();
      destinationChildNum_ = des.readVInt();
  }

  bool declContentsMatchInner(const Decl* other) const {
    return this->visibility_ == other->visibility_ &&
           this->linkage_ == other->linkage_ &&
           this->linkageNameChildNum_ == other->linkageNameChildNum_ &&
           this->destinationChildNum_ == other->destinationChildNum_;
  }

  void declMarkUniqueStringsInner(Context* context) const { }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

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
    Returns the destination expression, like 'loc' from 'on loc var x = 1'.
  */
  const AstNode* destination() const {
    if (destinationChildNum_ < 0) return nullptr;
    return child(destinationChildNum_);
  }

  /**
    See also AstNode::attachAttributeGroup. Add a destination node
    (like 'loc' in 'on loc var x = 1') to this AST after it was initially
    built.
   */
  void attachDestination(owned<AstNode> destination) {
    CHPL_ASSERT(destinationChildNum_ == NO_CHILD);
    destinationChildNum_ = children_.size();
    children_.push_back(std::move(destination));
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

DECLARE_SERDE_ENUM(uast::Decl::Visibility, uint8_t);
DECLARE_SERDE_ENUM(uast::Decl::Linkage, uint8_t);

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
