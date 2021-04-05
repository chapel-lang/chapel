#include "chpl/AST/Context.h"

#include <cstdlib>

namespace chpl {
namespace ast {

Context::Context() {
  // create a string for "" / NULL that matches
  // the UniqueString default constructor
  UniqueString str;
  const char* s = str.c_str();
  this->uniqueStringsTable.insert({s, s});
}

Context::~Context() {
  UniqueString emptyStr;
  const char* empty = emptyStr.c_str();

  // free all of the unique'd strings
  for (auto& item: this->uniqueStringsTable) {
    const char* s = item.second;
    if (s != empty) {
      free((void*)item.second);
    }
  }
}

const char* Context::getOrCreateUniqueString(const char* str) {
  auto search = this->uniqueStringsTable.find(str);
  if (search != this->uniqueStringsTable.end()) {
    return search->second;
  }
  char* s = strdup(str);
  this->uniqueStringsTable.insert(search, {s, s});
  return s;
}

const char* Context::uniqueCString(const char* s) {
  if (s == NULL) s = "";
  return this->getOrCreateUniqueString(s);
}

UniqueString Context::uniqueString(const char* s) {
  if (s == NULL) s = "";
  return UniqueString(this->getOrCreateUniqueString(s));
}

UniqueString Context::uniqueString(const std::string& s) {
  return UniqueString(this->getOrCreateUniqueString(s.c_str()));
}

} // end namespace ast
} // end namespace chpl
