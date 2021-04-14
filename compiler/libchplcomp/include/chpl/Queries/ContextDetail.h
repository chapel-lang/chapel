#ifndef CHPL_QUERIES_CONTEXT_DETAIL_H
#define CHPL_QUERIES_CONTEXT_DETAIL_H

#include <cstring>

/// \cond DO_NOT_DOCUMENT
namespace chpl {
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
} // end namespace detail
} // end namespace chpl
/// \endcond

#endif
