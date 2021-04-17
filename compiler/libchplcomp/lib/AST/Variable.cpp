#include "chpl/AST/Variable.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


Variable::Variable(ASTList children,
                   UniqueString name, Symbol::Visibility vis,
                   Variable::Tag tag,
                   int8_t typeExprChildNum, int8_t initExprChildNum)
  : Symbol(asttags::Variable, std::move(children), name, vis),
    tag_(tag),
    typeExprChildNum(typeExprChildNum), initExprChildNum(initExprChildNum) {

  assert(-1 <= typeExprChildNum && typeExprChildNum <= 1);
  assert(-1 <= initExprChildNum && initExprChildNum <= 1);
  assert(numChildren() <= 2);
#ifndef NDEBUG
  // check that all children are exprs (and not, say, Symbols)
  for (int i = 0; i < this->numChildren(); i++) {
    assert(getChild(i)->isExpr());
  }
#endif
}

bool Variable::contentsMatchInner(const BaseAST* other) const {
  const Variable* lhs = this;
  const Variable* rhs = (const Variable*) other;
  return lhs->symbolContentsMatchInner(rhs) &&
         lhs->tag_ == rhs->tag_ &&
         lhs->typeExprChildNum == rhs->typeExprChildNum &&
         lhs->initExprChildNum == rhs->initExprChildNum;
}



} // namespace ast
} // namespace chpl
