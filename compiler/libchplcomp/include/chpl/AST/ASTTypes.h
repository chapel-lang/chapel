#ifndef CHPL_AST_ASTTYPES_H
#define CHPL_AST_ASTTYPES_H

#include "chpl/Util/memory.h"

#include <vector>

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
#include "chpl/AST/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_DECL

// forward declare other classes
class BaseAST;
class Builder;

/**
  ASTList is just a list that owns some AST nodes.
 */
typedef std::vector<owned<BaseAST>> ASTList;

static inline ASTList makeASTList(owned<BaseAST> ast) {
  ASTList lst;
  lst.push_back(std::move(ast));
  return lst;
}

} // end namespace ast
} // end namespace chpl

#endif
