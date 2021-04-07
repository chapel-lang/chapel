#include "chpl/AST/UniqueString.h"

#include "chpl/AST/Context.h"

#include <cstring>

namespace chpl {
namespace ast {

// TODO: extend this idea to other well-known strings.
// That will make it easy to initialize them and have
// them as global (constant) variables.
// We can have a WellKnownStrings.h file that
// calls makeWellKnownUniqueString e.g.
static const char* const emptyString = "";

UniqueString::UniqueString()
  : s(emptyString) {
}

UniqueString UniqueString::build(Context* context, const char* s) {
  if (s == NULL) s = "";
  return UniqueString(context->uniqueCString(s));
}

UniqueString UniqueString::build(Context* context, const std::string& s) {
  return UniqueString(context->uniqueCString(s));
}

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
