#include "chpl/AST/Identifier.h"

#include "chpl/AST/Builder.h"
#include "chpl/AST/Context.h"

namespace chpl {
namespace ast {

Identifier* Identifier::build(Builder* builder, const char* name) {
  UniqueString uname = builder->context()->uniqueString(name);
  return new Identifier(uname);
}

} // namespace ast
} // namespace chpl
