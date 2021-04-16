#include "chpl/AST/Decl.h"

#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {


Decl::Decl(asttags::ASTTag tag, UniqueString name, Decl::Visibility visibility)
  : Expr(tag), name_(name), visibility_(visibility) {
}

Decl::Decl(asttags::ASTTag tag, ExprList children,
           UniqueString name, Decl::Visibility visibility)
  : Expr(tag, std::move(children)),
    name_(name), visibility_(visibility) {
}

Decl::~Decl() {
}


} // namespace ast
} // namespace chpl
