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
};

} // end namespace ast
} // end namespace chpl

#endif
