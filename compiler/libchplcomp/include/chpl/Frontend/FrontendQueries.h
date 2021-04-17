#ifndef CHPL_FRONTEND_FRONTENDQUERIES_H
#define CHPL_FRONTEND_FRONTENDQUERIES_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/ID.h"
#include "chpl/AST/Location.h"
#include "chpl/AST/ModuleDecl.h"
#include "chpl/Queries/Context.h"

#include <vector>

namespace chpl {

// This could alternatively be a class with static methods
namespace FrontendQueries {
  const std::string& fileText(Context* context, UniqueString path);
  const ast::Builder::Result* parseFile(Context* context, UniqueString path);

  typedef std::unordered_map<ID, Location> LocationsMap;
  const LocationsMap& fileLocations(Context* context, UniqueString path);
  Location locate(Context* context, ID id);

  // TODO: should this query "own" the ModuleDecl? Do we need to
  // use shared_ptr? There will probably be more queries down the line
  // that want to return AST pointers, so it would be nice if we don't
  // have to reference count everywhere. If we don't reference count,
  // we have to keep the depended-upon queries around.
  typedef std::vector<const ast::ModuleDecl*> ModuleDeclVec;
  const ModuleDeclVec& parse(Context* context, UniqueString path);

  const ast::BaseAST* ast(Context* context, ID id);
};

} // end namespace chpl

#endif
