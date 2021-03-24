#ifndef CHPL_AST_IDENTIFIER_H
#define CHPL_AST_IDENTIFIER_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {

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
 private:
  UniqueString name_;
 public:
  UniqueString name() const { return name_; }
};

}
#endif
