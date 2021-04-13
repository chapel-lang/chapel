#ifndef CHPL_AST_ERRONEOUSEXPR_H
#define CHPL_AST_ERRONEOUSEXPR_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/Location.h"

namespace chpl {
namespace ast {

/**
  This class represents some missing AST due to an error.
 */
class ErroneousExpr final : public Expr {
 private:
  ErroneousExpr();

 public:
  ~ErroneousExpr() = default;
  static owned<ErroneousExpr> build(Builder* builder, Location loc);
};

} // end namespace ast
} // end namespace chpl

#endif
