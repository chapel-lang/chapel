#ifndef CHPL_AST_BUILDER_H
#define CHPL_AST_BUILDER_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/UniqueString.h"

#include <vector>
#include <unordered_map>
#include <utility>

namespace chpl {

class Context;

namespace ast {

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
  UniqueString filepath_;
  UniqueString inferredModuleName_;
  ASTList topLevelExprs_;
  std::vector<ErrorMessage> errors_;
  std::vector<std::pair<ID, Location>> locations_;

  Builder(Context* context,
          UniqueString filepath, UniqueString inferredModuleName);
  UniqueString createImplicitModuleIfNeeded();
  void assignIDs(UniqueString inferredModule);
  void assignIDs(BaseAST* ast, pathVecT& path, declaredHereT& decl);
  void assignIDsPostorder(BaseAST* ast, UniqueString symbolPath, int& i);

 public:
  static owned<Builder> build(Context* context, const char* filepath);

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
    ast::ASTList topLevelExprs;
    std::vector<ErrorMessage> errors;
    std::vector<std::pair<ID, Location>> locations;

    static bool combine(Result& keep, Result& addin);
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

template<> struct combine<owned<chpl::ast::Builder::Result>> {
  bool operator()(owned<chpl::ast::Builder::Result>& keep,
                  owned<chpl::ast::Builder::Result>& addin) const {
    return chpl::ast::Builder::Result::combine(*keep, *addin);
  }
};


} // end namespace chpl


#endif
