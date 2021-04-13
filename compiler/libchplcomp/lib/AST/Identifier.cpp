#include "chpl/AST/Identifier.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {

Identifier::Identifier(UniqueString name)
  : Expr(asttags::Identifier), name_(name) {
}

owned<Identifier> Identifier::build(Builder* builder,
                                    Location loc, UniqueString name) {
  Identifier* ret = new Identifier(name);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

} // namespace ast
} // namespace chpl
