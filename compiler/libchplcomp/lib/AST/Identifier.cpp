#include "chpl/AST/Identifier.h"

#include "chpl/AST/Builder.h"
#include "chpl/AST/Context.h"

namespace chpl {
namespace ast {

Identifier* Identifier::build(Builder* builder, const char* name) {
  UniqueString uname = UniqueString::build(builder->context(), name);
  return new Identifier(uname);
}

} // namespace ast
} // namespace chpl
