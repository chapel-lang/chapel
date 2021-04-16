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

} // end namespace ast
} // end namespace chpl
