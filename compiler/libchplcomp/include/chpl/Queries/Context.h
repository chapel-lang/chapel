#ifndef CHPL_QUERIES_CONTEXT_H
#define CHPL_QUERIES_CONTEXT_H

#include "chpl/Queries/ContextDetail.h"
#include "chpl/Util/memory.h"

#include <unordered_map>

namespace chpl {

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
    and return it as a C string. If the passed string is NULL,
    this function will return an empty string.

    Strings returned by this function will always be aligned to 2 bytes.

    The function `UniqueString::build` returns such a string
    with a wrapper type. It should be preferred for type safety
    and to reduce redundant checks.
   */
  const char* uniqueCString(const char* s);
};

} // end namespace chpl

#endif
