#ifndef CHPL_AST_STMT_H
#define CHPL_AST_STMT_H

#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for statements
 */
class Stmt : public Expr {
 protected:
  Stmt(asttags::ASTTag tag);
  Stmt(asttags::ASTTag tag, ExprList children);
 public:
  virtual ~Stmt() = 0; // this is an abstract base class
};

} // end namespace ast
} // end namespace chpl

#endif
