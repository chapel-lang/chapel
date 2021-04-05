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
  go anywhere and that would be hard to parse). However, comments that
  are at a statement level will be represented with this type.
 */
class Comment final : public Expr {
 friend class Builder;

 static Comment* build(Builder& builder, UniqueString comment);

 private:
  UniqueString comment_;
 public:
  ~DocComment() override = default;
  UniqueString comment() const { return comment_; }
};

} // end namespace ast
} // end namespace chpl

#endif
