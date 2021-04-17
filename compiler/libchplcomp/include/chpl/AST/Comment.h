#ifndef CHPL_AST_COMMENT_H
#define CHPL_AST_COMMENT_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/Location.h"

#include <string>

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
  std::string comment_;

  Comment(std::string s);
  bool contentsMatchInner(const BaseAST* other) const override;

 public:
  ~Comment() override = default;
  static owned<Comment> build(Builder* builder, Location loc, std::string c);
  const char* c_str() const { return comment_.c_str(); }
  const std::string& str() const { return comment_; }
};

} // end namespace ast
} // end namespace chpl

#endif
