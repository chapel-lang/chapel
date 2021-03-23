#ifndef CHPL_AST_COMMENT_H
#define CHPL_AST_COMMENT_H

#include "chpl/AST/UniqueString.h"

namespace chpl {

// This class represents a comment that might be used for documentation.
// Not all comments are represented in the AST (since the comments could
// go anywhere and that would be hard to parse). However, comments that
// are at a statement level will be represented with this type.
class Comment : final Expr {
 private:
  UniqueString comment_;
 public:
  ~DocComment() override = default;
  UniqueString comment() const { return comment_; }
};

}
#endif
