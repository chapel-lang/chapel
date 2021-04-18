#include "chpl/AST/VariableDecl.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


VariableDecl::VariableDecl(owned<Variable> variable)
  : Decl(asttags::VariableDecl, std::move(variable)) {
}

bool VariableDecl::contentsMatchInner(const BaseAST* other) const {
  const VariableDecl* lhs = this;
  const VariableDecl* rhs = (const VariableDecl*) other;
  return lhs->declContentsMatchInner(rhs);
}

owned<VariableDecl>
VariableDecl::build(Builder* builder, Location loc,
                    UniqueString name, Symbol::Visibility vis,
                    Variable::Tag tag,
                    owned<Expr> typeExpr, owned<Expr> initExpr) {
  ASTList lst;
  int8_t typeExprChildNum = -1;
  int8_t initExprChildNum = -1;
  if (typeExpr.get() != nullptr) {
    lst.push_back(std::move(typeExpr));
    typeExprChildNum = lst.size() - 1;
  }
  if (initExpr.get() != nullptr) {
    lst.push_back(std::move(initExpr));
    initExprChildNum = lst.size() - 1;
  }
  Variable* sym = new Variable(std::move(lst), name, vis, tag,
                               typeExprChildNum, initExprChildNum);
  builder->noteLocation(sym, loc);
  VariableDecl* ret = new VariableDecl(toOwned(sym));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace ast
} // namespace chpl
