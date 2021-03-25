#ifndef CHPL_AST_EXPR_H
#define CHPL_AST_EXPR_H

#include "chpl/AST/BaseAST.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for expressions
 */
class Expr : BaseAST {
 public:
  virtual ~Expr() = 0; // this is an abstract base class
};

} // end namespace ast
} // end namespace chpl

#endif
