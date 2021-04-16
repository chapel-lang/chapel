#ifndef CHPL_AST_VARDECL_H
#define CHPL_AST_VARDECL_H

#include "chpl/AST/Decl.h"
#include "chpl/AST/Location.h"

namespace chpl {
namespace ast {

class Builder;

/**
  This class represents a variable declaration
  E.g. here are some examples

  \rst
  .. code-block:: chapel

      var a = 1;
      ref b = a;
      const c = 2;
      const ref d = c;
      param e = "hi";
  \endrst
 */
// TODO: the name VarDecl does not seem ideal given that it can
// represent 'const' in addition to 'var'
// TODO: should we use this class also to represent 'type t = otherType' ?
class VarDecl final : public Decl {
 friend class Builder;

 public:
  enum Tag {
    VAR,
    CONST,
    CONST_REF,
    REF,
    PARAM
  };


 private:

  Tag tag_;
  int8_t typeExprChildNum;
  int8_t initExprChildNum;

  VarDecl(ExprList children,
          UniqueString name, Decl::Visibility visibility,
          Tag tag, int typeExprChildNum, int initExprChildNum);

 public:
  ~VarDecl() override = default;
  static owned<VarDecl> build(Builder* builder, Location loc,
                              UniqueString name, Decl::Visibility visibility,
                              Tag tag,
                              owned<Expr> typeExpr, owned<Expr> initExpr);
  const Tag tag() const { return this->tag_; }
  const Expr* typeExpr() const { return this->getChild(typeExprChildNum); }
  const Expr* initExpr() const { return this->getChild(initExprChildNum); }
};

} // end namespace ast
} // end namespace chpl

#endif
