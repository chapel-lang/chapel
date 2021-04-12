#include "chpl/AST/UniqueString.h"

#include "chpl/AST/Context.h"

#include <cassert>
#include <cstring>

namespace chpl {
namespace ast {

namespace detail {

  char* InlinedString::dataAssumingTag(void* vptr) {
    char* ptr = (char*) vptr;
    // assuming the tag is present, where is the string data?
    // on a little endian system, need to pass the tag.
    // on big endian systems, the tag is after the null terminator,
    // so no action is necessary.
    #if __BYTE_ORDER == __LITTLE_ENDIAN
      ptr += 1; // pass the tag
    #endif
    return ptr;
  }


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
