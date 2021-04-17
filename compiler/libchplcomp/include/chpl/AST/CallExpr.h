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
 public:
  ~CallExpr() override = 0;

  // note: the reason for the +/- 1 below is that the
  // 0'th child is the called expression, which does
  // not count as an "actual".

  int numActuals() const {
    return this->numChildren() - 1;
  }
  const Expr* actual(int i) const {
    return this->getChild(i+1);
  }
  Expr* baseExpr() const {
    return this->getChild(0);
  }
};

} // end namespace ast
} // end namespace chpl

#endif
