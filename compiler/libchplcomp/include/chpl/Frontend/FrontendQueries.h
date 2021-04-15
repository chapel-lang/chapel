#ifndef CHPL_FRONTEND_FRONTENDQUERIES_H
#define CHPL_FRONTEND_FRONTENDQUERIES_H

#include "chpl/Queries/Context.h"
#include "chpl/AST/BaseAST.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Expr.h"

namespace chpl {

struct FrontendQueries {
  static const std::string& readFile(Context* context, UniqueString path);
  static ast::Builder::Result* parseFile(Context* context, UniqueString path);
};

} // end namespace chpl

#endif
