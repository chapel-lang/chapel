#ifndef CHPL_AST_IDENTIFIER_H
#define CHPL_AST_IDENTIFIER_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

class Builder;

/**
  This class represents a reference to a symbol by name.
  E.g. in

  \rst
  .. code-block:: chapel

      var x = 1; // here, 'x' is not an Identifier (it is the declared symbol)
      f(x);      // here, 'f' and 'x' are Identifiers
  \endrst
 */
class Identifier final : public Expr {
 friend class Builder;

 private:
  Identifier(UniqueString name)
    : Expr(asttags::Identifier), name_(name) { }
  UniqueString name_;
 public:
  ~Identifier() override = default;
  static Identifier* build(Builder* builder, UniqueString name);
  UniqueString name() const { return name_; }
};

} // end namespace ast
} // end namespace chpl

#endif
