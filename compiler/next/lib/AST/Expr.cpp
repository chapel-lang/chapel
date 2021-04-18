#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {


Expr::Expr(asttags::ASTTag tag)
  : BaseAST(tag) {
}

Expr::Expr(asttags::ASTTag tag, ASTList children)
  : BaseAST(tag, std::move(children)) {
}

Expr::~Expr() {
}


} // namespace ast
} // namespace chpl
