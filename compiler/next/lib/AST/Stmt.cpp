#include "chpl/AST/Stmt.h"

namespace chpl {
namespace ast {


Stmt::Stmt(asttags::ASTTag tag)
  : Expr(tag) {
}

Stmt::Stmt(asttags::ASTTag tag, ASTList children)
  : Expr(tag, std::move(children)) {
}

Stmt::~Stmt() {
}


} // namespace ast
} // namespace chpl
