#ifndef CHPL_AST_MODULEDECL_H
#define CHPL_AST_MODULEDECL_H

#include "chpl/AST/Decl.h"
#include "chpl/AST/Location.h"
#include "chpl/AST/Module.h"

namespace chpl {
namespace ast {

class Builder;

/**
  This class represents a module declaration.

  \rst
  .. code-block:: chapel

      module M { }
  \endrst

  contains a module declaration referring to the symbol for module M.
 */
class ModuleDecl final : public Decl {
 friend class Builder;

 private:
  ModuleDecl(owned<Module> module);
  bool contentsMatchInner(const BaseAST* other) const override;

 public:
  ~ModuleDecl() override = default;
  static owned<ModuleDecl> build(Builder* builder, Location loc,
                                 UniqueString name, Symbol::Visibility vis,
                                 Module::Tag tag, ASTList stmts);
  const Module* module() const {
    assert(this->symbol()->isModule());
    return (Module*)this->symbol();
  }
  const Module::Tag tag() const { return this->module()->tag(); }
};

} // end namespace ast
} // end namespace chpl

#endif
