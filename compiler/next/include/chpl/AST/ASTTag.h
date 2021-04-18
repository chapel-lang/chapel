#ifndef CHPL_AST_ASTTAG_H
#define CHPL_AST_ASTTAG_H

namespace chpl {
namespace ast {
namespace asttags {


/**

  This enum is used to identify which AST class a node is.
 */
enum ASTTag {
  // define the enum for all of the non-virtual AST nodes
  // using macros and ASTClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define AST_NODE(NAME) NAME ,
  #define AST_LEAF(NAME) NAME ,
  #define AST_BEGIN_SUBCLASSES(NAME) START_##NAME ,
  #define AST_END_SUBCLASSES(NAME) END_##NAME ,
  /// \endcond
  // Apply the above macros to ASTClassesList.h
  #include "chpl/AST/ASTClassesList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  NUM_AST_TAGS
};

// define is___ for leaf and regular nodes
// (not yet for abstract parent classes)
/// \cond DO_NOT_DOCUMENT
#define IS_AST(NAME) \
  static inline bool is##NAME(ASTTag tag) { \
    return tag == NAME; \
  }
#define AST_NODE(NAME) IS_AST(NAME)
#define AST_LEAF(NAME) IS_AST(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/AST/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_AST

// define is___ for abstract parent classes
/// \cond DO_NOT_DOCUMENT
#define IS_AST(NAME) \
  static inline bool is##NAME(ASTTag tag) { \
    return START_##NAME < tag && tag < END_##NAME; \
  }
#define AST_NODE(NAME)
#define AST_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) IS_AST(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/AST/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_AST

const char* tagToString(ASTTag tag);


} // end namespace tags
} // end namespace ast
} // end namespace chpl

#endif
