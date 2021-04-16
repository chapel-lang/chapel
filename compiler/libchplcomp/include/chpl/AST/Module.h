#ifndef CHPL_AST_MODULE_H
#define CHPL_AST_MODULE_H

#include "chpl/AST/Symbol.h"
#include "chpl/AST/Location.h"

namespace chpl {
namespace ast {

/**
  This class represents a module. For example:

  \rst
  .. code-block:: chapel

      module M { }
  \endrst

  contains a ModuleDecl that refers to a Module Symbol.
 */
class Module final : public Symbol {
 friend class Builder;
 friend class ModuleDecl;

 public:
  enum Tag {
    DEFAULT,
    PROTOTYPE,
  };

 private:
  Tag tag_;

  Module(ASTList children,
         UniqueString name, Symbol::Visibility vis,
         Module::Tag tag);

 public:
  ~Module() override = default;
  const Tag tag() const { return this->tag_; }
  int numStmts() const {
    return this->numChildren();
  }
  const Expr* stmt(int i) const {
    const BaseAST* ast = this->getChild(i);
    assert(ast->isExpr());
    return (Expr*) ast;
  }
};

} // end namespace ast
} // end namespace chpl

#endif
