#ifndef CHPL_AST_DECL_H
#define CHPL_AST_DECL_H

#include "chpl/AST/BaseAST.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for declarations
 */
class Decl : public BaseAST {
 public:
  virtual ~Decl() = 0; // this is an abstract base class

  enum Visibility {
    DEFAULT,
    PUBLIC,
    PRIVATE,
  };

 private:
  Visibility visibility_;

 public:
  Visibility visibility() { return visibility_; }
  //extern/config
};

} // end namespace ast
} // end namespace chpl

#endif
