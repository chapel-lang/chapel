#ifndef CHPL_UAST_FORWARDING_H
#define CHPL_UAST_FORWARDING_H

#include "chpl/queries/Location.h"
#include "chpl/uast/VisibilityClause.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Variable.h"

namespace chpl {
namespace uast {


/**
  This class represents a forwarding statement. Forwarding allows a record
  or class to specify that certain method calls will be forwarded to a
  particular expression. For example:

  \rst
  .. code-block:: chapel

    // This record defines two methods, area() and circumference().
    class MyCircleImpl {
      var radius:real;
      proc area() {
        return pi*radius*radius;
      }
      proc circumference() {
        return 2.0*pi*radius;
      }
    }

    // This record implements the Circle class and then forwards its calls
    // to area and circumference.
    record MyCircle {
      var impl: MyCircleImpl;

      // forwarding methods
      proc area() {
        return impl.area();
      }
      proc circumference() {
        return impl.circumference();
      }
    }

    // This record implements the Circle class as above, but the forwarding
    // statement makes this short declaration functionally the same as the more
    // verbose declaration of MyCircle, above.
    record MyCircle {
      forwarding var impl: MyCircleImpl;
    }

    // We can also limit which methods are forwarded by using the only or except
    // limiters, as with the import statement. Both of the following record
    // declarations will only forward the area() method on MyCircle to the
    // MyCircleImpl class method.
    record MyCircle {
      var impl: MyCircleImpl;

      forwarding impl only area;
    }

    record MyCircle {
      var impl: MyCircleImpl;

      forwarding impl except circumference;
    }

  \endrst
 */

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
