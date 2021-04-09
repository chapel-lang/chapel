#ifndef CHPL_FRONTEND_PARSER_H
#define CHPL_FRONTEND_PARSER_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Location.h"

#include <vector>
#include <unordered_map>

namespace chpl {

namespace ast { class Expr; }

using ast::owned;

/**
  A class for parsing
 */
class Parser final {
  public:
    struct ParseResult final {
      ast::ExprList topLevelExprs;
      std::vector<ast::ErrorMessage> parseErrors;
      std::unordered_map<ast::BaseAST*, ast::Location> locations;
    };

 private:

   // TODO: stuff to do with module search paths
   // and then connect parsed modules to a query

   // TODO: compile-time configuration variable settings
   // need to be stored in here.

    ast::Context* context_;

   Parser(ast::Context* context) : context_(context) { }

 public:
   static owned<Parser> build(ast::Context* context);
   ~Parser() = default;

   /**
     Return the AST Context used by this Parser.
    */
   ast::Context* context() { return context_; }

   /**
     Parse a file at a particular path.
    */
   ParseResult parseFile(const char* path);
   /**
     Parse source code in a string.
     'path' is only used for certain errors.
    */
   ParseResult parseString(const char* path, const char* str);
};

} // end namespace chpl

#endif
