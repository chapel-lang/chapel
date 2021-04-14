#ifndef CHPL_UTIL_MEMORY_H
#define CHPL_UTIL_MEMORY_H

#include <memory>

namespace chpl {

/**
 owned<T> is just a synonym for 'std::unique_ptr<T>'.
 It is shorter and uses the Chapel term for it.
 */
template<typename T>
  using owned = std::unique_ptr<T>;
/**
 give a raw pointer to an owned<T> to manage it.
 */
template<typename T>
static inline owned<T> toOwned(T* takeFrom) {
  return owned<T>(takeFrom);
}

} // end namespace chpl

#endif
