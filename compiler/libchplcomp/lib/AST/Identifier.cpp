#include "chpl/AST/Identifier.h"

#include "chpl/AST/Builder.h"
#include "chpl/AST/Context.h"

namespace chpl {
namespace ast {

Identifier* Identifier::build(Builder* builder, UniqueString name) {
  return new Identifier(name);
}

} // namespace ast
} // namespace chpl
