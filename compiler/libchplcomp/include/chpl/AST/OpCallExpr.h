#ifndef CHPL_AST_OPCALLEXPR_H
#define CHPL_AST_OPCALLEXPR_H

#include "chpl/AST/CallExpr.h"

namespace chpl {
namespace ast {

/**
  This class represents a call to an operator.
 */
class OpCallExpr final : public CallExpr {
 private:
  // which operator
  UniqueString op_;

  bool matchesInner(const BaseAST* other) const override;
 public:
  ~OpCallExpr() override = default;

  /** Returns the name of the operator called */
  UniqueString operatorName() const { return op_; }
};

} // end namespace ast
} // end namespace chpl

#endif
