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

bool BaseAST::shallowMatch(const BaseAST* other) const {
  const BaseAST* lhs = this;
  const BaseAST* rhs = other;
  return lhs->tag() == rhs->tag() &&
         lhs->id() == rhs->id() &&
         lhs->numChildren() == rhs->numChildren() &&
         lhs->contentsMatchInner(rhs);
}

/*
bool BaseAST::contentsMatch(const BaseAST* other) const {
  const BaseAST* lhs = this;
  const BaseAST* rhs = other;

  // check basics of the node itself
  if (!lhs->shallowMatch(other))
    return false;

  // check the children match
  int lhsN = lhs->numChildren();
  for (int i = 0; i < lhsN; i++) {
    if (!lhs->getChild(i)->contentsMatch(rhs->getChild(i)))
      return false;
  }

  return true;
}
*/

bool BaseAST::combineAST(owned<BaseAST>& keep, owned<BaseAST>& addin) {
  if (keep->shallowMatch(addin.get())) {
    // run combineAST on the child list
    return combineASTLists(keep->children_, addin->children_);
  } else {
    // swap the AST
    keep.swap(addin);
    return false;
  }
}

static void dumpHelper(const BaseAST* ast, int depth) {
  for (int i = 0; i < depth; i++) {
    printf(" ");
  }
  if (ast == nullptr) {
    printf("nullptr\n");
    return;
  }
  printf("%s %p\n", asttags::tagToString(ast->tag()), ast);
  int nChildren = ast->numChildren();
  for (int i = 0; i < nChildren; i++) {
    dumpHelper(ast->child(i), depth+1);
  }
}

void BaseAST::dump(const BaseAST* ast) {
  dumpHelper(ast, 0);
}


} // end namespace ast
} // end namespace chpl
