#ifndef CHPL_AST_LITERAL_H
#define CHPL_AST_LITERAL_H

#include "chpl/AST/Expr.h"

namespace chpl {

// This class represents a literal
//  e.g. 1 2.0 3.0i "string" b"bytes" are all literals
class Literal final : public Expr {
  // TODO: move over 'ifa' code
};

}
#endif
