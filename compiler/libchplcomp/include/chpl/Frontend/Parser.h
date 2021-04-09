#ifndef CHPL_FRONTEND_PARSER_H
#define CHPL_FRONTEND_PARSER_H

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Location.h"

#include <vector>
#include <unordered_map>

namespace chpl {
namespace ast {

class Expr;

class ParseResult final {
  std::vector<Expr*> topLevelExprs;
  std::vector<ErrorMessage> parseErrors;
  std::unordered_map<BaseAST*, Location> locations;
};

/**
  A class for parsing
 */
class Parser final {
 private:

   // TODO: stuff to do with module search paths
   // and then connect parsed modules to a query

   // TODO: compile-time configuration variable settings
   // need to be stored in here.

 public:
   /**
     Parse a file and return a vector of top-level statements.
    */
   std::vector<Expr*> parseFile(const char* path);
   /**
     Parse a string and return a vector of top-level statements.
    */
   std::vector<Expr*> parseString(const char* str);
};

} // end namespace ast
} // end namespace chpl

#endif
