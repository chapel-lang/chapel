#ifndef CHPL_AST_PRIMCALLEXPR_H
#define CHPL_AST_PRIMCALLEXPR_H

#include "chpl/AST/CallExpr.h"

#include <vector>

namespace chpl {
namespace ast {

/**
  This class represents a primitive call
 */
class PrimCallExpr final : public CallExpr {
 private:
   std::vector<Expr*> actuals;
 public:
  ~PrimCallExpr() override = default;

  // TODO: which primitive?
  int numActuals() const override { return actuals.size(); }
  Expr* actual(int i) const override { return actuals[i].actual; }
};

} // end namespace ast
} // end namespace chpl

#endif
