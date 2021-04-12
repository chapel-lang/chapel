#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {

Expr::Expr(asttags::ASTTag tag)
  : BaseAST(tag) {
}

Expr::Expr(asttags::ASTTag tag, ExprList stmts)
  : BaseAST(tag, std::move(stmts)) {
}

Expr::~Expr() {
}

} // namespace ast
} // namespace chpl
