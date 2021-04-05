#ifndef CHPL_AST_STMT_H
#define CHPL_AST_STMTEXPR_H

#include "chpl/AST/BaseAST.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for statements
 */
class Stmt : Expr {
 public:
  virtual ~Stmt() = 0; // this is an abstract base class
};

} // end namespace ast
} // end namespace chpl

#endif
