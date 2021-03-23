#ifndef CHPL_AST_ASTTAG_H
#define CHPL_AST_ASTTAG_H

namespace chpl {

namespace asttags {

// Define the type that represents a tag to identify which AST class
enum ASTTag {
  // define the enum for all of the non-virtual AST nodes
  // using macros and ASTList.h
  #define AST_NODE(NAME) NAME ,
  #define AST_LEAF(NAME) NAME ,
  #define AST_BEGIN_SUBCLASSES(NAME) START_##NAME ,
  #define AST_END_SUBCLASSES(NAME) END_##NAME ,
  // Apply the above macros to ASTList.h
  #include "ASTList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
};

// define is___ for leaf and regular nodes
// (not yet for abstract parent classes)
#define IS_AST(NAME) \
  static inline bool is##NAME(ASTTag tag) { \
    return tag == NAME;
  }
#define AST_NODE(NAME) IS_AST(NAME)
#define AST_LEAF(NAME) IS_AST(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME)
// Apply the above macros to ASTList.h
#include "ASTList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_AST

// define is___ for abstract parent classes
#define IS_AST(NAME) \
  static inline bool is##NAME(ASTTag tag) { \
    return START_##NAME < tag && tag < END_##NAME; \
  }
#define AST_NODE(NAME)
#define AST_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) IS_AST(NAME)
#define AST_END_SUBCLASSES(NAME)
// Apply the above macros to ASTList.h
#include "ASTList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_AST

} // end namespace tags

} // end namespace chpl

#endif
