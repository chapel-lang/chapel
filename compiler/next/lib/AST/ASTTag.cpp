#include "chpl/AST/ASTTag.h"

namespace chpl {
namespace ast {
namespace asttags {


static const char* tagToStringTable[NUM_AST_TAGS] = {
// define tag to string conversion
#define NAMESTR(NAME) \
  #NAME,
#define AST_NODE(NAME) NAMESTR(NAME)
#define AST_LEAF(NAME) NAMESTR(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) NAMESTR(START_##NAME)
#define AST_END_SUBCLASSES(NAME) NAMESTR(END_##NAME)
// Apply the above macros to ASTClassesList.h
#include "chpl/AST/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef NAMESTR
};

const char* tagToString(ASTTag tag) {
  if (0 <= tag && tag < NUM_AST_TAGS)
    return tagToStringTable[tag];
  else
    return "<unknown-tag>";
}


} // end namespace asttags
} // end namespace ast
} // end namespace chpl
