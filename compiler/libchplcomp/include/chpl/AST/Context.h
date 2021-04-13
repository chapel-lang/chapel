#ifndef CHPL_AST_CONTEXT_H
#define CHPL_AST_CONTEXT_H

#include "chpl/AST/memory.h"
#include "chpl/AST/UniqueString.h"

#include <unordered_map>
#include <cstring>

namespace chpl {
namespace ast {

/// \cond DO_NOT_DOCUMENT
namespace detail {
  struct UniqueStrEqual {
    bool operator()(const char* lhs, const char* rhs) const {
      return strcmp(lhs, rhs) == 0;
    }
  };

  struct UniqueStrHash {
    std::size_t operator()(const char* s) const {
      // this hash is from StringHashFns in the old map.h
      unsigned int h = 0;
      while (*s) h = h * 27 + (unsigned char)*s++;
      return h;
    }
  };
}
/// \endcond

/**
  This class stores the compilation-wide context.
  In particular it handles unique'd strings.
 */
class Context {
 private:
  std::unordered_map<const char*, const char*, detail::UniqueStrHash, detail::UniqueStrEqual> uniqueStringsTable;
  int zero; //just an easy way to check the context is valid for debugging

  const char* getOrCreateUniqueString(const char* s);

  // Future Work: support marking used strings and garbage collecting the rest
  // Could store an atomic uint_8 just after the string for the mark.

  // Future Work: make the context thread-safe
  
  // Future Work: allow moving some AST to a different context
  //              (or, at least, that can handle the unique strings)

  Context();

 public:
  /**
    Create a new AST Context.
   */
  static owned<Context> build();
  ~Context();


  /**
    Get or create a unique string for a NULL-terminated C string
    and return it as a C string.

    Strings returned by this function will always be aligned to 2 bytes.

    The function `UniqueString::build` returns such a string
    with a wrapper type. It should be preferred for type safety
    and to reduce redundant checks.
   */
  const char* uniqueCString(const char* s);
  const char* uniqueCString(const std::string& s);
};

} // end namespace ast
} // end namespace chpl

#endif
