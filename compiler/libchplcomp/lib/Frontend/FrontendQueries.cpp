#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/Frontend/Parser.h"
#include "chpl/Queries/QueryImpl.h"

#include "../Util/files.h"

#include <cstdio>

namespace chpl {
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
    QUERY_ERROR(error);
  }

  return QUERY_END(result);
}

const ast::Builder::Result* parse(Context* context, UniqueString path) {
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
  for (ErrorMessage e : tmpResult.errors) {
    QUERY_ERROR(e);
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
  const ast::Builder::Result* p = parse(context, path);
  // Create a map of ID to Location
  std::unordered_map<ID, Location> result;
  for (auto pair : p->locations) {
    ast::BaseAST* ast = pair.first;
    Location loc = pair.second;
    result.insert({ast->id(), loc});
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

const ast::BaseAST* ast(Context* context, ID id) {
  return nullptr;
}

} // end namespace FrontendQueries
} // end namespace chpl
