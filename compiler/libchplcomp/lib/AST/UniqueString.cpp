#include "chpl/AST/UniqueString.h"

#include "chpl/AST/Context.h"

#include <cassert>
#include <cstring>

namespace chpl {
namespace ast {

namespace detail {
  InlinedString InlinedString::buildUsingContextTable(Context* context,
                                                      const char* s, int len) {
    const char* u = context->uniqueCString(s);
    // assert that the address returned is even
    assert( (((uintptr_t)u)&1)==0 );
    return InlinedString::buildFromAligned(u, len);
  }
} // end namespace detail

} // end namespace ast
} // end namespace chpl
