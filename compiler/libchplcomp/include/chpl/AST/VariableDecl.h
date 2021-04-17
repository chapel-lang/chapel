#ifndef CHPL_AST_VARIABLEDECL_H
#define CHPL_AST_VARIABLEDECL_H

#include "chpl/AST/Decl.h"
#include "chpl/AST/Location.h"
#include "chpl/AST/Variable.h"

#include <cassert>

namespace chpl {
namespace ast {

class Builder;

/**
  This class represents a variable declaration
  E.g. here are some examples

  \rst
  .. code-block:: chapel

      var a = 1;
      ref b = a;
      const c = 2;
      const ref d = c;
      param e = "hi";
  \endrst

  Each of these is a VariableDecl that refers to a Variable Symbol.
 */
class VariableDecl final : public Decl {
 friend class Builder;

 private:
  VariableDecl(owned<Variable> variable);
  bool contentsMatchInner(const BaseAST* other) const override;

 public:
  ~VariableDecl() override = default;
  static owned<VariableDecl> build(Builder* builder, Location loc,
                                   UniqueString name, Symbol::Visibility vis,
                                   Variable::Tag tag,
                                   owned<Expr> typeExpr, owned<Expr> initExpr);
  const Variable* variable() const {
    const Symbol* sym = this->symbol();
    assert(sym->isVariable());
    return (Variable*)sym;
  }
  const Variable::Tag tag() const {
    return this->variable()->tag();
  }
  const Expr* typeExpr() const {
    return this->variable()->typeExpr();
  }
  const Expr* initExpr() const {
    return this->variable()->initExpr();
  }
};

} // end namespace ast
} // end namespace chpl

#endif
