#include "chpl/AST/UniqueString.h"

#include <unordered_map>
#include <cstring>

namespace chpl {

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

static std::unordered_map<const char*, const char*, UniqueStrHash, UniqueStrEqual> uniqueStringsTable;

static const char* uniquifyString(const char* str) {

  auto search = uniqueStringsTable.find(str);
  if (search != uniqueStringsTable.end()) {
    return search->second;
  }
  char* s = strdup(str);
  uniqueStringsTable.insert(search, {s, s});
  return s;
}

UniqueString::UniqueString(const char* str) {
  this->s = uniquifyString(str);
}

UniqueString::UniqueString(const std::string& str) {
  this->s = uniquifyString(str.c_str());
}

}
