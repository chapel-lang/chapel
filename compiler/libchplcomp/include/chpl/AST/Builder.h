#ifndef CHPL_AST_BUILDER_H
#define CHPL_AST_BUILDER_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/UniqueString.h"

#include <vector>
#include <unordered_map>
#include <utility>

namespace chpl {
namespace ast {

class Context;
class ErrorMessage;
class Expr;
class Location;

/**
  This class helps to build AST. It should only build AST
  from one file at a time.
 */
class Builder final {
 private:
  typedef std::vector<std::pair<UniqueString,int>> pathVecT;
  typedef std::unordered_map<UniqueString,int> declaredHereT;

  Context* context_;
  UniqueString inferredModuleName_;
  ExprList topLevelExprs_;
  std::vector<ErrorMessage> errors_;
  std::vector<std::pair<BaseAST*, Location>> locations_;

  Builder(Context* context, UniqueString inferredModuleName);
  void createImplicitModuleIfNeeded();
  void assignIDs();
  void assignIDs(BaseAST* ast, pathVecT& path, declaredHereT& decl);
  void assignIDsPostorder(BaseAST* ast, UniqueString symbolPath, int& i);

 public:
  static owned<Builder> build(Context* context, const char* filename);

  Context* context() const { return context_; }

  /**
    Save a toplevel expression in to the builder.
    This is called by the parser.
   */
  void addToplevelExpr(owned<Expr> e);

  /**
    Save an error.
   */
  void addError(ErrorMessage);

  /**
    Record the location of an AST element.
   */
  void noteLocation(BaseAST* ast, Location loc);

  /**
    This struct records the result of building some AST.
   */
  struct Result final {
    Context* context;
    ast::ExprList topLevelExprs;
    std::vector<ErrorMessage> errors;
    std::vector<std::pair<BaseAST*, Location>> locations;
  };

  /**
    Assign IDs to all of the AST elements added as toplevel expressions
    to this builder and return the result. This function clears
    these elements from the builder and it becomes empty.
   */
  Builder::Result result();

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
