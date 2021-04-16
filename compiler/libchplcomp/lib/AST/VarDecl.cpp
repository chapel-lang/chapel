#include "chpl/AST/VarDecl.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


VarDecl::VarDecl(ExprList children, 
                 UniqueString name, Decl::Visibility visibility,
                 VarDecl::Tag tag, int typeExprChildNum, int initExprChildNum)
  : Decl(asttags::VarDecl, std::move(children), name, visibility),
    tag_(tag),
    typeExprChildNum(typeExprChildNum), initExprChildNum(initExprChildNum) {
}


owned<VarDecl>
VarDecl::build(Builder* builder, Location loc,
               UniqueString name, Decl::Visibility visibility,
               Tag tag,
               owned<Expr> typeExpr, owned<Expr> initExpr) {
  ExprList lst;
  int typeExprChildNum = -1;
  int initExprChildNum = -1;
  if (typeExpr.get() != nullptr) {
    lst.push_back(std::move(typeExpr));
    typeExprChildNum = lst.size() - 1;
  }
  if (initExpr.get() != nullptr) {
    lst.push_back(std::move(initExpr));
    initExprChildNum = lst.size() - 1;
  }

  VarDecl* ret = new VarDecl(std::move(lst), name, visibility,
                             tag, typeExprChildNum, initExprChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace ast
} // namespace chpl
