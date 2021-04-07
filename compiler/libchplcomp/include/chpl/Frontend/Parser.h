#ifndef CHPL_FRONTEND_PARSER_H
#define CHPL_FRONTEND_PARSER_H

#include "chpl/AST/Stmt.h"

#include <vector>

namespace chpl {
namespace ast {

/**
  A class for parsing
 */
class Parser final {
 private:

   // TODO: stuff to do with module search paths
   // and then connect parsed modules to a query

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
