#include "chpl/AST/Comment.h"

#include "chpl/AST/Builder.h"

#include <cstdlib>

namespace chpl {
namespace ast {

Comment::Comment(std::string s)
 : Expr(asttags::Comment), comment_(std::move(s)) {
}

owned<Comment> Comment::build(Builder* builder, Location loc, std::string c) {
  Comment* ret = new Comment(std::move(c));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

} // namespace ast
} // namespace chpl
