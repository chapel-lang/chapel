#ifndef CHPL_AST_COMMENT_H
#define CHPL_AST_COMMENT_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

class Builder;

/**
  This class represents a comment that might be used for documentation.
  Not all comments are represented in the AST (since the comments could
  go anywhere and that would be hard to represent). However, comments that
  are at a statement level will be represented with this type.
 */
class Comment final : public Expr {
 friend class Builder;

 private:
  Comment(const char* comment, long size)
    : Expr(asttags::Comment), comment_(comment), size_(size) { }
  const char* comment_; // not a unique string; allocated here
  long size_;

 public:
  ~Comment() override;
  static owned<Comment> build(Builder* builder, const char* data, long size);
  const char* comment() const { return comment_; }
  long size() const { return size_; }
};

} // end namespace ast
} // end namespace chpl

#endif
