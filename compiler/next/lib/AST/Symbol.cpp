#include "chpl/AST/Symbol.h"

namespace chpl {
namespace ast {


Symbol::Symbol(asttags::ASTTag tag,
               UniqueString name, Symbol::Visibility visibility)
  : BaseAST(tag), name_(name), visibility_(visibility) {
}

Symbol::Symbol(asttags::ASTTag tag, ASTList children,
               UniqueString name, Symbol::Visibility visibility)
  : BaseAST(tag, std::move(children)),
    name_(name), visibility_(visibility) {
}

Symbol::~Symbol() {
}


} // namespace ast
} // namespace chpl
