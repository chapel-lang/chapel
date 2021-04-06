#ifndef CHPL_AST_BUILDER_H
#define CHPL_AST_BUILDER_H

#include <vector>

namespace chpl {
namespace ast {

class Context;
class Expr;

/**
  This class helps to build AST.
 */
class Builder final {
 private:
  Context* context_;
  std::vector<Expr*> topLevelStatements;

 public:
  Builder(Context* context) : context_(context) { }

  Context* context() const { return context_; }
  void addToplevelStmt(Expr* e) { topLevelStatements.push_back(e); }

  // For complex declarations, the builder supports
  //  enter/setBla/addBla/exit e.g. enterFnSymbol
  //  enterDecl/exitDecl
  // Parsing is easier if the name does not need to be set by the enter call.

  // Builder methods are actually type methods on the individual AST
  // elements. This prevents the Builder API from growing unreasonably large.
};

} // end namespace ast
} // end namespace chpl

#endif
