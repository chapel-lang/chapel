#include "chpl/AST/Decl.h"

#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {

Decl::Decl(asttags::ASTTag tag, owned<Symbol> symbol)
  : Expr(tag, makeASTList(std::move(symbol))) {
}

Decl::~Decl() {
}


} // namespace ast
} // namespace chpl
