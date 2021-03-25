#ifndef CHPL_AST_DECL_H
#define CHPL_AST_DECL_H

#include "chpl/AST/BaseAST.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for declarations
 */
class Decl : BaseAST {
 public:
  virtual ~Decl() = 0; // this is an abstract base class
};

} // end namespace ast
} // end namespace chpl

#endif
