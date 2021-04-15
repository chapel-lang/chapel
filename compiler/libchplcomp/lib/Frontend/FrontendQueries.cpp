#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/Frontend/Parser.h"
#include "chpl/Queries/QueryImpl.h"

#include "../Util/files.h"

#include <cstdio>

namespace chpl {

const std::string& FrontendQueries::readFile(Context* context, UniqueString path) {
  QUERY_BEGIN(context, std::string, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  ast::ErrorMessage error;
  std::string result;
  bool ok = readfile(path.c_str(), result, error);
  if (!ok) {
    QUERY_ERROR(error);
  }

  return QUERY_END(result);
}

static ast::Builder::Result* parseFile(Context* context, UniqueString path) {
  QUERY_BEGIN(context, owned<ast::Builder::Result>, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED().get();
  }

  ast::Builder::Result* result = new ast::Builder::Result();
  auto parser = Parser::build(context);
  ast::Builder::Result tmpResult = parser->parseFile(path.c_str());
  result->topLevelExprs.swap(tmpResult.topLevelExprs);
  result->locations.swap(tmpResult.locations);
  for (ast::ErrorMessage e : tmpResult.errors) {
    QUERY_ERROR(e);
  }

  return QUERY_END(toOwned(result)).get();
}

} // end namespace chpl
