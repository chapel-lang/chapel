#include "chpl/AST/ErroneousExpr.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


ErroneousExpr::ErroneousExpr()
  : Expr(asttags::ErroneousExpr) {
}

bool ErroneousExpr::contentsMatchInner(const BaseAST* other) const {
  const ErroneousExpr* lhs = this;
  const ErroneousExpr* rhs = (const ErroneousExpr*) other;
  return lhs->exprContentsMatchInner(rhs);
}

owned<ErroneousExpr> ErroneousExpr::build(Builder* builder, Location loc) {
  ErroneousExpr* ret = new ErroneousExpr();
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace ast
} // namespace chpl
