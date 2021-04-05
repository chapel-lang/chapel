#ifndef CHPL_AST_ERRONEOUSEXPR_H
#define CHPL_AST_ERRONEOUSEXPR_H

#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {

/**
  This class represents some missing AST due to an error.
 */
class ErroneousExpr final : public Expr {
};

} // end namespace ast
} // end namespace chpl

#endif
