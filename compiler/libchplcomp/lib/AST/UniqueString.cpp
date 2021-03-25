#include "chpl/AST/UniqueString.h"

#include <cstring>

namespace chpl {
namespace ast {

bool UniqueString::startsWith(const char* prefix) const {
  return (0 == strncmp(this->s, prefix, strlen(prefix)));
}
bool UniqueString::startsWith(const UniqueString prefix) const {
  return this->startsWith(prefix.c_str());
}
bool UniqueString::startsWith(const std::string& prefix) const {
  return this->startsWith(prefix.c_str());
}

} // end namespace ast
} // end namespace chpl
