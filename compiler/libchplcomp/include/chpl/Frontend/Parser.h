#ifndef CHPL_FRONTEND_PARSER_H
#define CHPL_FRONTEND_PARSER_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Location.h"
#include "chpl/AST/memory.h"

#include <vector>

namespace chpl {

/**
  A class for parsing
 */
class Parser final {
  public:
    struct ParseResult final {
      ast::ExprList topLevelExprs;
      std::vector<ast::ErrorMessage> parseErrors;
      std::vector<std::pair<ast::BaseAST*, ast::Location>> locations;
    };

 private:
   // TODO: stuff to do with module search paths
   // and then connect parsed modules to a query

   // TODO: compile-time configuration variable settings
   // need to be stored in here.

   Context* context_;
   Parser(Context* context);

 public:
   static owned<Parser> build(Context* context);
   ~Parser() = default;

   /**
     Return the AST Context used by this Parser.
    */
   Context* context() { return context_; }

   /**
     Parse a file at a particular path.
    */
   ast::Builder::Result parseFile(const char* path);
   /**
     Parse source code in a string.
     'path' is only used for certain errors.
    */
   ast::Builder::Result parseString(const char* path, const char* str);
};

} // end namespace chpl

#endif
