#include "chpl/AST/ErroneousExpr.h"

namespace chpl {
namespace ast {

ErroneousExpr* ErroneousExpr::build(Builder* builder) {
  return new ErroneousExpr();
}

} // namespace ast
} // namespace chpl
