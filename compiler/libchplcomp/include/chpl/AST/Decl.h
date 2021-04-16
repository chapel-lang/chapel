#ifndef CHPL_AST_DECL_H
#define CHPL_AST_DECL_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for declarations
 */
class Decl : public Expr {
 public:
  enum Visibility {
    VISIBILITY_DEFAULT,
    VISIBILITY_PUBLIC,
    VISIBILITY_PRIVATE,
  };

 private:
  UniqueString name_;
  Visibility visibility_;

 protected:
  Decl(asttags::ASTTag tag, UniqueString name, Visibility visibility);
  Decl(asttags::ASTTag tag, ExprList children,
       UniqueString name, Visibility visibility);

 public:
  virtual ~Decl() = 0; // this is an abstract base class

  UniqueString name() { return name_; }
  Visibility visibility() { return visibility_; }
};

} // end namespace ast
} // end namespace chpl

#endif
