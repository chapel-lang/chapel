#ifndef CHPL_AST_EXPR_H
#define CHPL_AST_EXPR_H

#include "chpl/AST/BaseAST.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for expressions
 */
class Expr : public BaseAST {
 protected:
  Expr(asttags::ASTTag tag);
  Expr(asttags::ASTTag tag, ASTList children);
 public:
  virtual ~Expr() = 0; // this is an abstract base class
};

} // end namespace ast
} // end namespace chpl

#endif
