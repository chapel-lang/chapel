#include "chpl/AST/Comment.h"

namespace chpl {
namespace ast {

Comment::~Comment() {
  delete comment_;
}

Comment* Comment::build(Builder* builder, const char* data, long size) {
  return new Comment(data, size);
}

} // namespace ast
} // namespace chpl
