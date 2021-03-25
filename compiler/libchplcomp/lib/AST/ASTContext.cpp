#include "chpl/AST/ASTContext.h"

#include <cstdlib>

namespace chpl {
namespace ast {

ASTContext::~ASTContext() {
  // free all of the unique'd strings
  for (auto& item: this->uniqueStringsTable) {
    free((void*)item.second);
  }
}

const char* ASTContext::getOrCreateUniqueString(const char* str) {
  auto search = this->uniqueStringsTable.find(str);
  if (search != this->uniqueStringsTable.end()) {
    return search->second;
  }
  char* s = strdup(str);
  this->uniqueStringsTable.insert(search, {s, s});
  return s;
}

UniqueString ASTContext::uniqueString(const char* s) {
  if (s == NULL) s = "";
  return UniqueString(this->getOrCreateUniqueString(s));
}

UniqueString ASTContext::uniqueString(const std::string& s) {
  return UniqueString(this->getOrCreateUniqueString(s.c_str()));
}

} // end namespace ast
} // end namespace chpl
