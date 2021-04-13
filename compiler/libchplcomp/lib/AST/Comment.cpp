#include "chpl/AST/Comment.h"

#include <cstdlib>

namespace chpl {
namespace ast {

Comment::Comment(const char* comment, long size)
 : Expr(asttags::Comment), comment_(comment), size_(size) {
}

Comment::~Comment() {
  free((void*)comment_);
}

owned<Comment> Comment::build(Builder* builder, const char* data, long size) {
  return toOwned(new Comment(data, size));
}

} // namespace ast
} // namespace chpl
