#ifndef CHPL_AST_LITERAL_H
#define CHPL_AST_LITERAL_H

#include "chpl/AST/Expr.h"

namespace chpl {
namespace ast {

/**
  This class represents a literal.

  Here are some example literals:

  \rst
  .. code-block:: chapel

    1 2.0 3.0i "string" b"bytes"
  \endrst
 */
class Literal final : public Expr {
  // TODO: move over 'ifa' code
};

} // end namespace ast
} // end namespace chpl

#endif
