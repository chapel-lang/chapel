#ifndef CHPL_AST_CONTEXT_H
#define CHPL_AST_CONTEXT_H

#include "UniqueString.h"

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

  const char* getOrCreateUniqueString(const char* s);

  // Future Work: support marking used strings and garbage collecting the rest
  // Could store an atomic uint_8 just after the string for the mark.

 public:
  Context();
  ~Context();

  /**
    Get or create a unique string for a NULL-terminated C string
    and return it as a C string.

    The function `uniqueString` should be preferred for type safety
    and to reduce redundant checks.
   */
  const char* uniqueCString(const char* s);

  /**
    Get or create a unique string for a NULL-terminated C string.
    If NULL is provided, this function will return uniqueString("").
   */
  UniqueString uniqueString(const char* s);

  /**
    Get or create a unique string for a C++ string
    \rst
    .. note::

      will not handle strings with embedded ``'\0'`` bytes
    \endrst
   */
  UniqueString uniqueString(const std::string& s);
};

} // end namespace ast
} // end namespace chpl

#endif
