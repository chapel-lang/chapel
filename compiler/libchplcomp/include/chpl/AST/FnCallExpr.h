#ifndef CHPL_AST_FNCALLEXPR_H
#define CHPL_AST_FNCALLEXPR_H

#include "chpl/AST/CallExpr.h"

#include <vector>

namespace chpl {
namespace ast {

/**
  This class represents a function call or method call, including virtual and
  non-virtual calls (since the difference is not known before resolution).
 */
class FnCallExpr : public CallExpr {
 private:
   // this represents the called expression e.g. 'f' in 'f(1,2,3)'
   Expr* baseExpr_;
   // for each actual (matching CallExpr's actuals), what are the names?
   // if the actual is unnamed, it is the empty string.
   std::vector<UniqueString> actualNames_;
   // TODO: do we need partialTag / methodTag?
 public:
  ~FnCallExpr() override = default;

  /** Returns the base expression e.g. 'f' in 'f(1,2,3)' */
  Expr* baseExpr() const { return baseExpr_; }
  /** Returns whether actual i is named as with 'f(a=3)' */
  bool actualIsNamed(int i) const { return !actualNames_[i].isEmpty(); }
  /** Returns the name of the actual, if used; otherwise the empty string */
  UniqueString actualName(int i) const { return actualNames_[i]; }
};

} // end namespace ast
} // end namespace chpl

#endif
