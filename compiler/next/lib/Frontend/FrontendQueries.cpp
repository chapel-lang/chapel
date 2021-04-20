#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Module.h"
#include "chpl/Frontend/Parser.h"
#include "chpl/Queries/QueryImpl.h"

#include "../Util/files.h"

#include <cstdio>

namespace chpl {

template<> struct combine<FrontendQueries::LocationsMap> {
  bool operator()(FrontendQueries::LocationsMap& keep,
                  FrontendQueries::LocationsMap& addin) const {
    return defaultCombine(keep, addin);
  }
};
template<> struct combine<FrontendQueries::ModuleDeclVec> {
  bool operator()(FrontendQueries::ModuleDeclVec& keep,
                  FrontendQueries::ModuleDeclVec& addin) const {
    return defaultCombine(keep, addin);
  }
};
template<> struct combine<FrontendQueries::DefinedTopLevelNamesVec> {
  bool operator()(FrontendQueries::DefinedTopLevelNamesVec& keep,
                  FrontendQueries::DefinedTopLevelNamesVec& addin) const {

    size_t nKeep = keep.size();
    size_t nAddin = addin.size();
    bool match = true;
    if (nKeep == nAddin) {
      for (size_t i = 0; i < nKeep; i++) {
        FrontendQueries::DefinedTopLevelNames& keepElt = keep[i];
        FrontendQueries::DefinedTopLevelNames& addinElt = addin[i];
        if (keepElt.module != addinElt.module ||
            keepElt.topLevelNames != addinElt.topLevelNames) {
          match = false;
          break;
        }
      }
    } else {
      match = false;
    }

    if (match) {
      return true;
    } else {
      keep.swap(addin);
      return false;
    }
  }
};

namespace FrontendQueries {

const std::string& fileText(Context* context, UniqueString path) {
  QUERY_BEGIN_NAMED(context, std::string, "fileText", path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  ErrorMessage error;
  std::string result;
  bool ok = readfile(path.c_str(), result, error);
  if (!ok) {
    QUERY_ERROR(std::move(error));
  }

  return QUERY_END(result);
}

const ast::Builder::Result* parseFile(Context* context, UniqueString path) {
  QUERY_BEGIN(context, owned<ast::Builder::Result>, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED().get();
  }

  // Run the fileText query to get the file contents
  const std::string& text = fileText(context, path);

  ast::Builder::Result* result = new ast::Builder::Result();
  auto parser = Parser::build(context);
  const char* pathc = path.c_str();
  const char* textc = text.c_str();
  ast::Builder::Result tmpResult = parser->parseString(pathc, textc);
  result->topLevelExprs.swap(tmpResult.topLevelExprs);
  result->locations.swap(tmpResult.locations);
  for (ErrorMessage& e : tmpResult.errors) {
    ErrorMessage tmp;
    tmp.swap(e);
    QUERY_ERROR(std::move(tmp));
  }

  // Update the filePathForModuleName query
  for (auto & topLevelExpr : result->topLevelExprs) {
    UniqueString moduleName = context->moduleNameForID(topLevelExpr->id());
    context->setFilePathForModuleName(moduleName, path);
  }

  return QUERY_END(toOwned(result)).get();
}

const LocationsMap& fileLocations(Context* context, UniqueString path) {
  QUERY_BEGIN(context, LocationsMap, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  // Get the result of parsing
  const ast::Builder::Result* p = parseFile(context, path);
  // Create a map of ID to Location
  std::unordered_map<ID, Location> result;
  for (auto pair : p->locations) {
    ID id = pair.first;
    Location loc = pair.second;
    result.insert({id, loc});
  }
  
  return QUERY_END(result);
}

Location locate(Context* context, ID id) {
  QUERY_BEGIN(context, Location, id);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForID(id);
  // Get the map of ID to Location
  Location result(path);
  const LocationsMap& map = fileLocations(context, path);
  auto search = map.find(id);
  if (search != map.end()) {
    result = search->second;
  }

  return QUERY_END(result);
}

const ModuleDeclVec& parse(Context* context, UniqueString path) {
  QUERY_BEGIN(context, ModuleDeclVec, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  // Get the result of parsing
  const ast::Builder::Result* p = parseFile(context, path);
  // Compute a vector of ModuleDecls
  ModuleDeclVec result;
  for (auto & topLevelExpr : p->topLevelExprs) {
    if (const ast::ModuleDecl* modDecl = topLevelExpr->toModuleDecl()) {
      result.push_back(modDecl);
    }
  }

  return QUERY_END(result);
}

static std::vector<UniqueString> getTopLevelNames(const ast::Module* module) {
  std::vector<UniqueString> result;
  int nStmts = module->numStmts();
  for (int i = 0; i < nStmts; i++) {
    const ast::Expr* expr = module->stmt(i);
    if (const ast::Decl* decl = expr->toDecl()) {
      const ast::Symbol* sym = decl->symbol();
      result.push_back(sym->name());
    }
  }
  return result;
}

const DefinedTopLevelNamesVec& moduleLevelDeclNames(Context* context,
                                                    UniqueString path) {
  QUERY_BEGIN(context, DefinedTopLevelNamesVec, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  DefinedTopLevelNamesVec result;

  // Get the result of parsing modules
  const ModuleDeclVec& p = parse(context, path);
  for (const ast::ModuleDecl* modDecl : p) {
    const ast::Module* module = modDecl->module();
    result.push_back(DefinedTopLevelNames(module, getTopLevelNames(module)));
  }

  return QUERY_END(result);
}


/*const ast::BaseAST* ast(Context* context, ID id) {
  return nullptr;
}*/

} // end namespace FrontendQueries
} // end namespace chpl
