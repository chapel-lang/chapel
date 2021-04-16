#ifndef CHPL_FRONTEND_FRONTENDQUERIES_H
#define CHPL_FRONTEND_FRONTENDQUERIES_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/ID.h"
#include "chpl/AST/Location.h"
#include "chpl/Queries/Context.h"

namespace chpl {

// This could alternatively be a class with static methods
namespace FrontendQueries {
  const std::string& fileText(Context* context, UniqueString path);
  const ast::Builder::Result* parse(Context* context, UniqueString path);
  typedef std::unordered_map<ID, Location> LocationsMap;
  const LocationsMap& fileLocations(Context* context, UniqueString path);
  Location locate(Context* context, ID id);
  const ast::BaseAST* ast(Context* context, ID id);
};

} // end namespace chpl

#endif
