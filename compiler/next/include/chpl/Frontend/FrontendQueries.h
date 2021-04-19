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

  typedef std::vector<const ast::ModuleDecl*> ModuleDeclVec;
  const ModuleDeclVec& parse(Context* context, UniqueString path);

  struct DefinedTopLevelNames {
    // the module
    const ast::Module* module;
    // these are in program order
    std::vector<UniqueString> topLevelNames;
    DefinedTopLevelNames(const ast::Module* module,
                         std::vector<UniqueString> topLevelNames)
      : module(module), topLevelNames(std::move(topLevelNames)) {
    }
  };
  typedef std::vector<DefinedTopLevelNames> DefinedTopLevelNamesVec;

  const DefinedTopLevelNamesVec& moduleLevelDeclNames(Context* context,
                                                      UniqueString path);


  /*struct ResolutionGroup {
    // index is the postorder ID
    std::vector<Symbol*> idToSymbol;
  };
  typedef std::unordered_map<Module*,owned<ResolutionGroup>>
    ModuleInitResolutionResult;*/
  /*typedef std::unordered_map<FnSymbol*,owned<ResolutionResult>>
    FunctionResolutionResult;*/

  //const ast::BaseAST* ast(Context* context, ID id);
};

} // end namespace chpl


#endif
