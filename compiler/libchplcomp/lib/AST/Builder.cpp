#include "chpl/AST/Builder.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/ModuleDecl.h"

#include <cstring>
#include <string>

namespace chpl {
namespace ast {

Builder::Builder(Context* context,
                 UniqueString filepath,
                 UniqueString inferredModuleName)
  : context_(context),
    filepath_(filepath),
    inferredModuleName_(inferredModuleName),
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

owned<Builder> Builder::build(Context* context, const char* filepath) {
  // compute the basename of filename to get the inferred module name
  std::string modname = filenameToModulename(filepath);
  auto uniqueModname = UniqueString::build(context, modname);
  auto uniqueFilename = UniqueString::build(context, filepath);
  auto b = new Builder(context, uniqueFilename, uniqueModname);
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
  UniqueString inferredName = this->createImplicitModuleIfNeeded();
  this->assignIDs(inferredName);

  // Performance: We could consider copying all of these AST
  // nodes to a newly allocated buffer big enough to hold them
  // all contiguously. The reason to do so would be to ensure
  // that a postorder traversal of the AST has good data locality
  // (i.e. good cache behavior).

  Builder::Result ret;
  ret.topLevelExprs.swap(topLevelExprs_);
  ret.errors.swap(errors_);
  ret.locations.swap(locations_);
  return ret;
}

// Returns the name of the implicit module, or "" if there is none
// If the implicit module is needed, moves the statements in to it.
UniqueString Builder::createImplicitModuleIfNeeded() {
  bool containsOnlyModules = true;
  bool containsAnyModules = false;
  for (auto const& ownedExpr: topLevelExprs_) {
    if (ownedExpr->isModuleDecl()) {
      containsAnyModules = true;
    } else {
      containsOnlyModules = false;
    }
  }
  if (containsAnyModules && containsOnlyModules) {
    UniqueString empty;
    return empty;
  } else {
    // create a new module containing all of the statements
    ASTList stmts;
    stmts.swap(topLevelExprs_);
    auto implicitModule = ModuleDecl::build(this, Location(filepath_),
                                            inferredModuleName_,
                                            Symbol::VISIBILITY_DEFAULT,
                                            Module::IMPLICIT,
                                            std::move(stmts));
    topLevelExprs_.push_back(std::move(implicitModule));
    // return the name of the module
    return inferredModuleName_;
  }
}

void Builder::assignIDs(UniqueString inferredModule) {
  pathVecT path;
  declaredHereT decl;

  if (!inferredModule.isEmpty())
    path.push_back(std::make_pair(inferredModule, 0));

  for (auto const& ownedExpr: topLevelExprs_) {
    const BaseAST* ast = ownedExpr.get();
    assignIDs((BaseAST*)ast, path, decl); 
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
  // Don't consider comments when computing AST ids.
  if (ast->isComment())
    return;

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

bool Builder::Result::matches(const Builder::Result* other) const {
  // First, check the sizes
  if (this->topLevelExprs.size() != other->topLevelExprs.size() ||
      this->errors.size()        != other->errors.size() ||
      this->locations.size()     != other->locations.size())
    return false;

  // Otherwise, check the contents. We can assume the sizes match.
  for (size_t i = 0; i < this->topLevelExprs.size(); i++) {
    BaseAST* lhsAst = this->topLevelExprs[i].get();
    BaseAST* rhsAst = other->topLevelExprs[i].get();
    if (!lhsAst->contentsMatch(rhsAst))
      return false;
  }
  for (size_t i = 0; i < this->errors.size(); i++) {
    if (this->errors[i] != other->errors[i])
      return false;
  }
  for (size_t i = 0; i < this->locations.size(); i++) {
    BaseAST* lhsAst = this->locations[i].first;
    BaseAST* rhsAst = other->locations[i].first;
    Location lhsLoc = this->locations[i].second;
    Location rhsLoc = other->locations[i].second;
    if (lhsAst->id() != rhsAst->id() || lhsLoc != rhsLoc)
      return false;
  }
  return true;
}

} // namespace ast
} // namespace chpl
