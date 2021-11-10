#ifndef CHPL_UAST_FORWARDING_H
#define CHPL_UAST_FORWARDING_H

#include "chpl/queries/Location.h"
#include "chpl/uast/VisibilityClause.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Variable.h"

namespace chpl {
namespace uast {


class ForwardingDecl final : public Decl {


private:

  ForwardingDecl(ASTList children)
    : Decl(asttags::ForwardingDecl, std::move(children),
                Decl::DEFAULT_VISIBILITY,
                Decl::DEFAULT_LINKAGE,
                /*linkageNameChildNum*/ -1) {

    assert(children_.size() == 0 || children_.size() == 1);
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const ForwardingDecl* lhs = (const ForwardingDecl*) this;
    const ForwardingDecl* rhs = (const ForwardingDecl*) other;
    return lhs->declContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    declMarkUniqueStringsInner(context);
  }



 public:
  ~ForwardingDecl() override = default;


  static owned<ForwardingDecl> build(Builder* builder, Location loc,
                                     owned<Expression> expr);

  /**
    Returns the child for this ForwardingDecl or nullptr if there was none.
  */

  const Expression* expr() const {
    if (children_.size() > 0) {
      const ASTNode* ast = this->child(0);
      assert(ast->isExpression());
      return (const Expression*)ast;
    } else {
      return nullptr;
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif