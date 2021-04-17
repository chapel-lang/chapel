#include "chpl/AST/BaseAST.h"

// A complete definition of Expr is required in order
// to have the ExprList field compile (used in BaseAST() and ~BaseAST()).
#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {

BaseAST::BaseAST(asttags::ASTTag tag)
  : tag_(tag), id_(), children_() {
}

BaseAST::BaseAST(asttags::ASTTag tag, ASTList children)
  : tag_(tag), id_(), children_(std::move(children)) {
}


BaseAST::~BaseAST() {
}

bool BaseAST::contentsMatch(const BaseAST* other) const {
  const BaseAST* lhs = this;
  const BaseAST* rhs = other;

  // check basics of the node itself
  if (lhs->tag() != rhs->tag())
    return false;
  if (lhs->id() != rhs->id())
    return false;

  int lhsN = lhs->numChildren();
  int rhsN = rhs->numChildren();
  if (lhsN != rhsN)
    return false; 

  if (!lhs->contentsMatchInner(rhs))
    return false;

  // check the children match
  for (int i = 0; i < lhsN; i++) {
    if (!lhs->getChild(i)->contentsMatch(rhs->getChild(i)))
      return false;
  }

  return true;
}

} // end namespace ast
} // end namespace chpl
