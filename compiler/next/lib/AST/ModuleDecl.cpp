#include "chpl/AST/ModuleDecl.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


ModuleDecl::ModuleDecl(owned<Module> module)
  : Decl(asttags::ModuleDecl, std::move(module)) {
}

bool ModuleDecl::contentsMatchInner(const BaseAST* other) const {
  const ModuleDecl* lhs = this;
  const ModuleDecl* rhs = (const ModuleDecl*) other;
  return lhs->declContentsMatchInner(rhs);
}

owned<ModuleDecl>
ModuleDecl::build(Builder* builder, Location loc,
                  UniqueString name, Symbol::Visibility vis,
                  Module::Tag tag, ASTList stmts) {

  Module* sym = new Module(std::move(stmts), name, vis, tag);
  ModuleDecl* ret = new ModuleDecl(toOwned(sym));
  builder->noteLocation(sym, loc);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace ast
} // namespace chpl
