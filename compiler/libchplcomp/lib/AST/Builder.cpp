#include "chpl/AST/Builder.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Expr.h"

#include <cstring>
#include <string>

namespace chpl {
namespace ast {

Builder::Builder(Context* context, UniqueString inferredModuleName)
  : context_(context), inferredModuleName_(inferredModuleName),
    topLevelExprs_(), errors_(), locations_() {
}

static std::string filenameToModulename(const char* filename) {
  const char* moduleName = filename;
  const char* firstSlash = strrchr(moduleName, '/');

  if (firstSlash) {
    moduleName = firstSlash + 1;
  }

  const char* firstPeriod = strrchr(moduleName, '.');
  if (firstPeriod) {
    return std::string(moduleName, firstPeriod-moduleName);
  } else {
    return std::string(moduleName);
  }
}

owned<Builder> Builder::build(Context* context, const char* filename) {
  // compute the basename of filename to get the inferred module name
  std::string modname = filenameToModulename(filename);
  auto uniqueModname = UniqueString::build(context, modname);
  auto b = new Builder(context, uniqueModname);
  return toOwned(b);
}

void Builder::addToplevelExpr(owned<Expr> e) {
  this->topLevelExprs_.push_back(std::move(e));
}

void Builder::addError(ErrorMessage e) {
  this->errors_.push_back(std::move(e));
}

void Builder::noteLocation(BaseAST* ast, Location loc) {
  this->locations_.push_back(std::make_pair(ast, loc));
}

Builder::Result Builder::result() {
  this->createImplicitModuleIfNeeded();
  this->assignIDs();

  Builder::Result ret;
  ret.context = context_;
  ret.topLevelExprs.swap(topLevelExprs_);
  ret.errors.swap(errors_);
  ret.locations.swap(locations_);
  return ret;
}

void Builder::createImplicitModuleIfNeeded() {
  // TODO
}

void Builder::assignIDs() {
  pathVecT path;
  declaredHereT decl;

  path.push_back(std::make_pair(inferredModuleName_, 0));
  for (auto const& ownedExpr: topLevelExprs_) {
    Expr* e = ownedExpr.get();
    assignIDs(e, path, decl); 
  }
}

void Builder::assignIDs(BaseAST* ast, pathVecT& path, declaredHereT& decl) {
  // TODO: if it's a decl, adjust the path&decl
  // For now, we will just do the postorder traversal

  std::string pathStr;
  bool first = true;
  for (auto p : path) {
    UniqueString name = p.first;
    int repeat = p.second;
    if (first == false)
      pathStr += ".";
    first = false;
    pathStr += name.c_str();
    pathStr += "#";
    pathStr += std::to_string(repeat);
  }
  auto symbolPath = UniqueString::build(this->context(), pathStr);
  int curId = 0;
  assignIDsPostorder(ast, symbolPath, curId);
}

void Builder::assignIDsPostorder(BaseAST* ast, UniqueString symbolPath, int& i) {
  int firstChildID = i;
  for (int j = 0; j < ast->numChildren(); j++) {
    BaseAST* child = (BaseAST*) ast->getChild(j);
    this->assignIDsPostorder(child, symbolPath, i);
  }
  int afterChildID = i;
  int myID = afterChildID;
  i++; // count the ID for the node we are currently visiting
  int numContainedIDs = afterChildID - firstChildID;
  ast->setID(ID(symbolPath, myID, numContainedIDs));
}

} // namespace ast
} // namespace chpl
