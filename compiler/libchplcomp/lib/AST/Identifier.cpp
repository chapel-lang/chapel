#include "chpl/AST/Identifier.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {

Identifier::Identifier(UniqueString name)
  : Expr(asttags::Identifier), name_(name) {
}

bool Identifier::contentsMatchInner(const BaseAST* other) const {
  const Identifier* lhs = this;
  const Identifier* rhs = (const Identifier*) other;
  return lhs->exprContentsMatchInner(rhs) &&
         lhs->name_ == rhs->name_;
}

owned<Identifier> Identifier::build(Builder* builder,
                                    Location loc, UniqueString name) {
  Identifier* ret = new Identifier(name);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

} // namespace ast
} // namespace chpl
