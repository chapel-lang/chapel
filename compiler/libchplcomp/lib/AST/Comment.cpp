#include "chpl/AST/Comment.h"

namespace chpl {
namespace ast {

Comment::~Comment() {
  delete comment_;
}

owned<Comment> Comment::build(Builder* builder, const char* data, long size) {
  return toOwned(new Comment(data, size));
}

} // namespace ast
} // namespace chpl
