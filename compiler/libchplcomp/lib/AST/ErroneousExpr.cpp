#include "chpl/AST/ErroneousExpr.h"

namespace chpl {
namespace ast {

owned<ErroneousExpr> ErroneousExpr::build(Builder* builder) {
  return toOwned(new ErroneousExpr());
}

} // namespace ast
} // namespace chpl
