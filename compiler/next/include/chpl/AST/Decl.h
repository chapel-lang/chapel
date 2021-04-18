#ifndef CHPL_AST_DECL_H
#define CHPL_AST_DECL_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/Symbol.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for declarations
 */
class Decl : public Expr {
 protected:
  Decl(asttags::ASTTag tag, owned<Symbol> symbol);
  bool declContentsMatchInner(const Decl* other) const {
    return true;
  }

 public:
  virtual ~Decl() = 0; // this is an abstract base class

  const Symbol* symbol() const {
    const BaseAST* ast = getChild(0);
    assert(ast->isSymbol());
    return (const Symbol*) ast;
  }
  UniqueString name() const {
    return symbol()->name();
  }
  Symbol::Visibility visibility() const {
    return symbol()->visibility();
  }
};

} // end namespace ast
} // end namespace chpl

#endif
