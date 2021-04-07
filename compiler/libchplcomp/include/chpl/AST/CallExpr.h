#ifndef CHPL_AST_CALLEXPR_H
#define CHPL_AST_CALLEXPR_H

#include "chpl/AST/Expr.h"

#include <vector>

namespace chpl {
namespace ast {

/**
  This abstract class represents something call-like
 */
class CallExpr : public Expr {
 private:
  std::vector<Expr*> actuals_;

 public:
  ~CallExpr() override = 0;

  int numActuals() const { return actuals_.size(); }
  Expr* actual(int i) { return actuals_[i]; }
  const Expr* actual(int i) const { return actuals_[i]; }
};

} // end namespace ast
} // end namespace chpl

#endif
