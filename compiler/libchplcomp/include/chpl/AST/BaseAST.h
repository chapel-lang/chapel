#ifndef CHPL_AST_BASEAST_H
#define CHPL_AST_BASEAST_H

#include "chpl/AST/ASTTag.h"
#include "chpl/AST/ID.h"

namespace chpl {

// This is the base class for AST types
class BaseAST {
 public:
  using asttags;

 private:
  ASTTag tag_;
  ID id_;

 public:
  ~BaseAST() = default;

  ASTTag tag() {
    return tag_;
  }

  ID id() {
    return id_; 
  }

  // Returns 'true' if this symbol contains another AST node.
  // This is an operation on the IDs.
  bool contains(const BaseAST* other) const {
    return this->id_.contains(other->id_);
  }

  #define AST_IS(NAME) \
    bool is##NAME() { \
      return this->tag == NAME; \
    }
  // define is__ methods for the various AST types
  // using macros and ASTList.h
  #define AST_NODE(NAME) AST_IS(NAME)
  #define AST_LEAF(NAME) AST_IS(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_IS(NAME)
  #define AST_END_SUBCLASSES(NAME)
  // Apply the above macros to ASTList.h
  #include "ASTList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_IS

  // define to__ methods for the various AST types
  // using macros and ASTList.h
  // Note: these offer equivalent functionality to C++ dynamic_cast<DstType*>
  #define AST_TO(NAME) \
    NAME * to##NAME() { \
      return this->is##NAME() ? (NAME *)this : nullptr; \
    }
  #define AST_NODE(NAME) AST_TO(NAME)
  #define AST_LEAF(NAME) AST_TO(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_TO(NAME)
  #define AST_END_SUBCLASSES(NAME)
  // Apply the above macros to ASTList.h
  #include "ASTList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_TO
};

} // end namespace chpl

namespace std {

// define std::less for the various AST types
// using macros and ASTList.h
#define AST_LESS(NAME) \
  template<> struct less<chpl::NAME*> { \
    bool operator()(const chpl::NAME* lhs, const chpl::NAME* rhs) const { \
      if (lhs == nullptr && rhs != nullptr) return true; \
      if (rhs == nullptr) return false; \
      std::less<chpl::ID> lessID; \
      return lessID(lhs->id(), rhs->id()); \
    } \
  };
#define AST_NODE(NAME) AST_LESS(NAME)
#define AST_LEAF(NAME) AST_LESS(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_LESS(NAME)
#define AST_END_SUBCLASSES(NAME)
// Apply the above macros to ASTList.h
#include "ASTList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_LESS

} // end namespace std

#endif
