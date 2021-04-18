#include "chpl/AST/Module.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


Module::Module(ASTList children, UniqueString name, Symbol::Visibility vis,
               Module::Tag tag)
  : Symbol(asttags::Module, std::move(children), name, vis), tag_(tag) {

#ifndef NDEBUG
  // check that all children are exprs (and not, say, Symbols)
  for (int i = 0; i < this->numChildren(); i++) {
    assert(getChild(i)->isExpr());
  }
#endif
}

bool Module::contentsMatchInner(const BaseAST* other) const {
  const Module* lhs = this;
  const Module* rhs = (const Module*) other;
  return lhs->symbolContentsMatchInner(rhs) &&
         lhs->tag_ == rhs->tag_;
}

} // namespace ast
} // namespace chpl
