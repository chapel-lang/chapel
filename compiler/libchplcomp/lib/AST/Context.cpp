#include "chpl/AST/Context.h"

#include <cstdlib>

namespace chpl {
namespace ast {

Context::Context() : uniqueStringsTable(), zero(0) {
}

owned<Context> Context::build() {
  return toOwned(new Context());
}

Context::~Context() {
  // free all of the unique'd strings
  for (auto& item: this->uniqueStringsTable) {
    const char* s = item.second;
    free((void*)s);
  }
}

#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)

static char* allocateEvenAligned(size_t amt) {
  char* buf = (char*) malloc(amt);
  // Normally, malloc returns something that is aligned to 16 bytes,
  // but it's technically possible that a platform library
  // could not do so. So, here we check.
  // We only need even alignment here.
  if ((((uintptr_t)buf) & 1) != 0) {
    free(buf);
    // try again with an aligned allocation
    size_t alignment = sizeof(void *);
    size_t align_up_len = ALIGN_UP(amt, sizeof(void*));
    buf = (char*) aligned_alloc(alignment, align_up_len);
  }
  assert(buf);
  assert((((uintptr_t)buf) & 1) == 0);
  return buf;
}

const char* Context::getOrCreateUniqueString(const char* str) {
  auto search = this->uniqueStringsTable.find(str);
  if (search != this->uniqueStringsTable.end()) {
    return search->second;
  }
  size_t len = strlen(str)+1;
  char* buf = allocateEvenAligned(len);
  // copy the string data, including the null terminator
  memcpy(buf, str, len);
  this->uniqueStringsTable.insert(search, {buf, buf});
  return buf;
}

const char* Context::uniqueCString(const char* s) {
  if (s == NULL) s = "";
  return this->getOrCreateUniqueString(s);
}
const char* Context::uniqueCString(const std::string& s) {
  return this->getOrCreateUniqueString(s.c_str());
}

} // end namespace ast
} // end namespace chpl
