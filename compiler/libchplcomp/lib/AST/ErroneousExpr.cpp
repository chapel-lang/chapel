#include "chpl/AST/ErroneousExpr.h"

namespace chpl {
namespace ast {

ErroneousExpr::ErroneousExpr()
  : Expr(asttags::ErroneousExpr) {
}

owned<ErroneousExpr> ErroneousExpr::build(Builder* builder) {
  return toOwned(new ErroneousExpr());
}

} // namespace ast
} // namespace chpl
