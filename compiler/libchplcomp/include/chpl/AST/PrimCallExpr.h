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
 public:
  ~PrimCallExpr() override = default;

  // TODO: which primitive?
  int numActuals() const override { return this->numChildren(); }
  Expr* actual(int i) const override { return this->getChild(i); }
};

} // end namespace ast
} // end namespace chpl

#endif
