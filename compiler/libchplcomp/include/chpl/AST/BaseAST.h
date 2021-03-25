#ifndef CHPL_AST_BASEAST_H
#define CHPL_AST_BASEAST_H

#include "chpl/AST/ASTTag.h"
#include "chpl/AST/ID.h"

namespace chpl {

namespace ast {

// forward declare the various AST types
// using macros and ASTClassesList.h
/// \cond DO_NOT_DOCUMENT
#define AST_DECL(NAME) class NAME;
#define AST_NODE(NAME) AST_DECL(NAME)
#define AST_LEAF(NAME) AST_DECL(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_DECL(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_DECL

/**
  This is the base class for AST types
 */
class BaseAST {
 public:

 private:
  asttags::ASTTag tag_;
  ID id_;

 public:
  ~BaseAST() = default;

  asttags::ASTTag tag() const {
    return tag_;
  }

  ID id() const {
    return id_; 
  }

  // Returns 'true' if this symbol contains another AST node.
  // This is an operation on the IDs.
  bool contains(const BaseAST* other) const {
    return this->id_.contains(other->id_);
  }

  // define is__ methods for the various AST types
  // using macros and ASTClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define AST_IS(NAME) \
    bool is##NAME() const { \
      return asttags::is##NAME(this->tag_); \
    }
  #define AST_NODE(NAME) AST_IS(NAME)
  #define AST_LEAF(NAME) AST_IS(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_IS(NAME)
  #define AST_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to ASTClassesList.h
  #include "ASTClassesList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_IS

  // define to__ methods for the various AST types
  // using macros and ASTClassesList.h
  // Note: these offer equivalent functionality to C++ dynamic_cast<DstType*>
  /// \cond DO_NOT_DOCUMENT
  #define AST_TO(NAME) \
    const NAME * to##NAME() const { \
      return this->is##NAME() ? (NAME *)this : nullptr; \
    }
  #define AST_NODE(NAME) AST_TO(NAME)
  #define AST_LEAF(NAME) AST_TO(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_TO(NAME)
  #define AST_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to ASTClassesList.h
  #include "ASTClassesList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_TO
};

} // end namespace ast

} // end namespace chpl

/// \cond DO_NOT_DOCUMENT
namespace std {

// define std::less for the various AST types
// using macros and ASTClassesList.h
/// \cond DO_NOT_DOCUMENT
#define AST_LESS(NAME) \
  template<> struct less<chpl::ast::NAME*> { \
    bool operator()(const chpl::ast::NAME* lhs, \
                    const chpl::ast::NAME* rhs) const { \
      if (lhs == nullptr && rhs != nullptr) return true; \
      if (rhs == nullptr) return false; \
      std::less<chpl::ast::ID> lessID; \
      /* cast in the next line is so it compiles with only forward decls */ \
      return lessID(((const chpl::ast::BaseAST*)lhs)->id(), \
                    ((const chpl::ast::BaseAST*)rhs)->id()); \
    } \
  };
#define AST_NODE(NAME) AST_LESS(NAME)
#define AST_LEAF(NAME) AST_LESS(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_LESS(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_LESS

} // end namespace std
/// \endcond

#endif
