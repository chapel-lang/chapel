/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chpl/uast/Builder.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/parsing/Parser.h"
#include "chpl/parsing/parser-error.h"
#include "chpl/uast/OpCall.h"
#include "chpl/framework/query-impl.h"

#include <cstring>
#include <string>

namespace chpl {
namespace uast {

// Query to check if config was used already
static const ID& nameToConfigSettingId(Context* context, std::string name) {
  QUERY_BEGIN_INPUT(nameToConfigSettingId, context, name);

  // return empty ID if ID not already set using useConfigSetting
  ID result;
  return QUERY_END(result);
}

// Input query to store the used configs
static void
useConfigSetting(Context* context, std::string name, ID id) {
  QUERY_STORE_INPUT_RESULT(nameToConfigSettingId, context, id, name);
}

bool Builder::checkAllConfigVarsAssigned(Context* context) {
   // check that all config vars that were set from the command line were assigned
   bool anyBadConfigs = false;
   auto configs = parsing::configSettings(context);
   for (auto config : configs) {
     auto usedId = nameToConfigSettingId(context, config.first);
     if (usedId.isEmpty()) {
       auto loc = Location();
       context->error(loc,"Trying to set unrecognized config '%s' via -s flag", config.first.c_str());
       anyBadConfigs = true;
     }
   }
   return anyBadConfigs;
 }

std::string Builder::filenameToModulename(const char* filename) {
  const char* moduleName = filename;
  const char* lastSlash = strrchr(moduleName, '/');

  if (lastSlash) {
    moduleName = lastSlash + 1;
  }

  const char* lastPeriod = strrchr(moduleName, '.');
  if (lastPeriod) {
    return std::string(moduleName, lastPeriod-moduleName);
  } else {
    return std::string(moduleName);
  }
}

owned<Builder> Builder::createForTopLevelModule(Context* context,
                                                const char* filepath) {
  auto uniqueFilename = UniqueString::get(context, filepath);
  UniqueString startingSymbolPath;
  auto b = new Builder(context, uniqueFilename, startingSymbolPath);
  return toOwned(b);
}

owned<Builder> Builder::createForIncludedModule(Context* context,
                                                const char* filepath,
                                                UniqueString parentSymbolPath) {
  auto uniqueFilename = UniqueString::get(context, filepath);
  auto b = new Builder(context, uniqueFilename, parentSymbolPath);
  return toOwned(b);
}

void Builder::addToplevelExpression(owned<AstNode> e) {
  this->topLevelExpressions_.push_back(std::move(e));
}

void Builder::noteLocation(AstNode* ast, Location loc) {
  notedLocations_[ast] = loc;
}

BuilderResult Builder::result() {
  this->createImplicitModuleIfNeeded();
  this->assignIDs();

  // Performance: We could consider copying all of these AST
  // nodes to a newly allocated buffer big enough to hold them
  // all contiguously. The reason to do so would be to ensure
  // that a postorder traversal of the AST has good data locality
  // (i.e. good cache behavior).

  BuilderResult ret(filepath_);
  ret.topLevelExpressions_.swap(topLevelExpressions_);
  ret.idToAst_.swap(idToAst_);
  ret.idToLocation_.swap(idToLocation_);
  ret.commentIdToLocation_.swap(commentToLocation_);

  return ret;
}

bool Builder::astTagIndicatesNewIdScope(asttags::AstTag tag) {
  if (asttags::isNamedDecl(tag)) {
    return (asttags::isFunction(tag) ||
            asttags::isModule(tag) ||
            asttags::isInterface(tag) ||
            asttags::isTypeDecl(tag));
  } else if (asttags::isExternBlock(tag)) {
    return true;
  }

  return false;
}

// If the implicit module is needed, moves the statements in to it.
void Builder::createImplicitModuleIfNeeded() {
  bool containsOnlyModules = true;
  bool containsAnyModules = false;
  bool containsOther = false;
  int nModules = 0;
  const Module* lastModule = nullptr;
  const AstNode* firstNonModule = nullptr;
  const AstNode* firstUseImportOrRequire = nullptr;

  for (auto const& ownedExpression: topLevelExpressions_) {
    const AstNode* ast = ownedExpression.get();
    if (ast->isComment()) {
      // ignore comments for this analysis
    } else if (ast->isModule()) {
      containsAnyModules = true;
      lastModule = ast->toModule();
      nModules++;
    } else {
      containsOnlyModules = false;
      if (firstNonModule == nullptr) {
        firstNonModule = ast;
      }

      if (!firstUseImportOrRequire && (ast->isUse() || ast->isImport() || ast->isRequire())) {
        firstUseImportOrRequire = ast;
      } else {
        containsOther = true;
      }
    }
  }
  if (containsAnyModules && containsOnlyModules) {
    // no inferred module is needed.
    return;
  } else {
    // compute the basename of filename to get the inferred module name
    std::string modname = Builder::filenameToModulename(filepath_.c_str());
    auto inferredModuleName = UniqueString::get(context_, modname);
    // create a new module containing all of the statements
    AstList stmts;
    stmts.swap(topLevelExpressions_);
    auto loc = Location(filepath_, 1, 1, 1, 1);
    auto ownedModule = Module::build(this, std::move(loc),
                                     /*attributeGroup*/ nullptr,
                                     Decl::DEFAULT_VISIBILITY,
                                     inferredModuleName,
                                     Module::IMPLICIT,
                                     std::move(stmts));
    const Module* implicitModule = ownedModule.get();
    topLevelExpressions_.push_back(std::move(ownedModule));

    // emit warnings as needed
    if (firstUseImportOrRequire && !containsOther && nModules == 1) {
      CHPL_REPORT(context(), ImplicitFileModule,
                  firstUseImportOrRequire, lastModule, implicitModule);
    } else if (nModules >= 1 && !containsOnlyModules) {
      CHPL_REPORT(context(), ImplicitFileModule,
                  firstNonModule, lastModule, implicitModule);
    }
  }
}

void Builder::assignIDs() {
  pathVecT pathVec;
  declaredHereT duplicates;
  int i = 0;
  int commentIndex = 0;

  if (!startingSymbolPath_.isEmpty()) {
    // start from the starting symbol path if it exists
    pathVec = ID::expandSymbolPath(context_, startingSymbolPath_);
  }

  for (auto const& ownedExpression: topLevelExpressions_) {
    AstNode* ast = ownedExpression.get();
    if (ast->isModule() || ast->isComment()) {
      UniqueString emptyString;
      doAssignIDs(ast, emptyString, i, commentIndex, pathVec, duplicates);
    } else {
      CHPL_ASSERT(false && "topLevelExpressions should only be module decls or comments");
    }
  }
}

/* A note about ID assignment

  This ID assignment tries to balance several competing goals:
   * would like postorder Ids to be available to make it easy to store e.g.
     resolution results for a function in a vector
   * would like incremental recompilation to minimize recomputation if code is
     added -- in particular this means that for say a function we don't want
     that function's ID to include the postOrderId in the parent scope

  The ID assignment uses the strategy of having functions, type decls, and
  modules create a new ID scope (with a new postOrderId counter). These uAST
  nodes have an ID based upon the path to that symbol and have a postOrderId
  that is just after the last element contained within.

  When printing IDs we use the notation of putting the symbolPath
  part first and then '@' and then the postOrderId. We leave out the
  @ and postOrderId when the postOrderId is -1. That is the case
  with AST nodes that create a new scope (Modules, Functions, Type decls).

  For example:

  M         module M {
  M.Inner     module Inner {
  M.Inner@0     a;
  M.Inner@1     b;
  M.Inner@2     c;
              }
  M@0         x;
            }


  Comments are assigned a separate incrementing ID, but they don't
  store any information that lets them map back to their module or file.
  We think this is acceptable because a documentation tool processing
  Comments can work with the parse result and look up comments with
  BuilderResult::commentToLocation
 */
void Builder::doAssignIDs(AstNode* ast, UniqueString symbolPath, int& i,
                          int& commentIndex, pathVecT& pathVec,
                          declaredHereT& duplicates) {
  if (Comment* comment = ast->toComment()) {
    comment->setCommentId(commentIndex);
    commentIndex += 1;

    auto search = notedLocations_.find(ast);
    if (search != notedLocations_.end()) {
      CHPL_ASSERT(!search->second.isEmpty());
      commentToLocation_.push_back(search->second);
    } else {
      CHPL_ASSERT(false && "Location for all ast should be set by noteLocation");
    }
    return;
  }

  // check if this is a config var/param/type and if a value was set from the command line
  // and update the initExpr for this node if so
  std::string configName;
  std::string configValue;
  AstNode* ieNode = nullptr;
  if (auto var = ast->toVariable()) {
    if (var->isConfig()) {
     lookupConfigSettingsForVar(var, pathVec, configName, configValue);
      if (!configName.empty())
        ieNode = updateConfig(var, configName, configValue);
    }
  }

  int firstChildID = i;

  bool newScope = Builder::astTagIndicatesNewIdScope(ast->tag());

  if (newScope) {
    // for scoping constructs, adjust the symbolPath and
    // then visit the defined symbol
    UniqueString declName;

    if (auto nd = ast->toNamedDecl()) {
      declName = nd->name();
    } else if (ast->isExternBlock()) {
      declName = UniqueString::get(context_, "-externblock");
    }

    // For anonymous functions, just use the 'kind' as the name.
    if (auto fn = ast->toFunction()) {
      if (fn->isAnonymous()) {
        assert(declName.isEmpty());
        auto str = Function::kindToString(fn->kind());
        declName = UniqueString::get(context_, str);
      }
    }

    int repeat = 0;
    auto search = duplicates.find(declName);
    if (search != duplicates.end()) {
      // it's already there, so increment the repeat counter
      repeat = search->second;
      repeat++;
      search->second = repeat;
    } else {
      duplicates.insert(search, std::make_pair(declName, 0));
    }

    // compute an escaped version of the name
    // (in case it contains special characters used in ID like . and #)
    std::string quotedNameString = escapeStringId(declName.c_str());
    auto quotedName = UniqueString::get(context_, quotedNameString);

    // push the path component
    pathVec.push_back(std::make_pair(quotedName, repeat));

    // compute the string representing the path
    std::string pathStr;
    bool first = true;
    for (const auto& p : pathVec) {
      UniqueString name = p.first;
      int repeat = p.second;
      if (first == false)
        pathStr += ".";
      first = false;
      pathStr += name.c_str();
      if (repeat != 0) {
        pathStr += "#";
        pathStr += std::to_string(repeat);
      }
    }
    auto newSymbolPath = UniqueString::get(context_, pathStr);
    CHPL_ASSERT(ID::expandSymbolPath(context_, newSymbolPath) == pathVec);

    // get a fresh postorder traversal counter and duplicates map
    int freshId = 0;
    declaredHereT freshMap;
    for (auto & child : ast->children_) {
      AstNode* ptr = child.get();
      this->doAssignIDs(ptr, newSymbolPath, freshId, commentIndex, pathVec, freshMap);
    }

    int numContainedIds = freshId;
    ast->setID(ID(newSymbolPath, -1, numContainedIds));

    // Note: when creating a new symbol (e.g. fn), we're not incrementing i.
    // The new symbol ID has the updated path (e.g. function name)
    // and other IDs in the parent scope don't consider the position
    // of this function.

    // pop the path component we just added
    pathVec.pop_back();

  } else {
    // not a new scope

    // visit the children now to get integer part of ids in postorder
    for (auto & child : ast->children_) {
      AstNode* ptr = child.get();
      this->doAssignIDs(ptr, symbolPath, i, commentIndex, pathVec, duplicates);
    }

    int afterChildID = i;
    int myID = afterChildID;
    i++; // count the ID for the node we are currently visiting
    int numContainedIDs = afterChildID - firstChildID;
    ast->setID(ID(symbolPath, myID, numContainedIDs));
  }

  // update idToAst_ for the visited AST node
  idToAst_[ast->id()] = ast;

  // update locations_ for the visited ast
  auto search = notedLocations_.find(ast);
  if (search != notedLocations_.end()) {
    CHPL_ASSERT(!search->second.isEmpty());
    idToLocation_[ast->id()] = search->second;
    // if a config's initExpr was updated, mark it as used and make sure it wasn't used previously
    if (ieNode) {
      CHPL_ASSERT(ast->isVariable());
      checkConfigPreviouslyUsed(ast->toVariable(), configName);
    }
  } else {
    CHPL_ASSERT(false && "Location for all ast should be set by noteLocation");
  }
}

void Builder::checkConfigPreviouslyUsed(const Variable* var, std::string& configNameUsed) {
  // If you're reading this and confused about how we can call useConfigSetting
  // and then call nameToConfigSetting, essentially setting a value and then
  // asking for it back and comparing against the value we just used, you're not alone.
  // See the docs in query-impl.h (QUERY_STORE_INPUT_RESULT) that describes
  // why this works.
  // An important aspect is that calling a "Getter" type input query also stores the results and will
  // return those saved results on subsequent calls to during the same revision.
  // "If called multiple times __within the same revision__, only the first
  // stored result in that revision will be saved."
  useConfigSetting(context(), configNameUsed, var->id());
  auto usedId = nameToConfigSettingId(context(), configNameUsed);

  if (usedId != var->id()) {
    CHPL_REPORT(context(), AmbiguousConfigName, configNameUsed, var, usedId);
  }
}

/**
 * Check if a config var has a setting passed from the command line and save the name/value into ref args
 */
void Builder::lookupConfigSettingsForVar(Variable* var, pathVecT& pathVec, std::string& name, std::string& value) {
  std::pair<std::string, std::string> configMatched;
  CHPL_ASSERT(var->isConfig());
  const auto &configs = parsing::configSettings(this->context());
  // inspect pathVec to build possible matching module prefix
  std::string possibleModule;
  if (pathVec.size() > 1) {
    for (size_t i = 1; i < pathVec.size(); i++) {
      possibleModule += pathVec[i].first.str();
      possibleModule += ".";
    }
  } else if (pathVec.size() == 1) {
    possibleModule = pathVec[0].first.str();
    possibleModule += ".";
  }
  // for config vars, check if they were set from the command line
  for (auto configPair: configs) {
    if ((var->name().str() == configPair.first && var->visibility() != Decl::PRIVATE)
        || configPair.first == possibleModule + var->name().str()) {
      // found a config that was set via cmd line
      // handle deprecations
      if (auto attribs = var->attributeGroup()) {
        if (attribs->isDeprecated()) {
          // TODO: Need proper message handling here
          std::string msg = "'" + var->name().str() + "' was set via a compiler flag";
          if (attribs->deprecationMessage().isEmpty()) {
            std::cerr << "warning: " + var->name().str() + " is deprecated" << std::endl;
          } else {
            std::cerr << "warning: " + attribs->deprecationMessage().str() << std::endl;
          }
          std::cerr << "note: " + msg << std::endl;
        }
      }
      if (!configMatched.first.empty() &&
          configMatched.first != configPair.first) {

        CHPL_REPORT(context(), AmbiguousConfigSet,
                    var, configMatched.first, configPair.first);
      }
      configMatched = configPair;
    }
  }
  name = configMatched.first;
  value = configMatched.second;
}

/*
 * Update the initExpr for a config var/param/type
 */
AstNode* Builder::updateConfig(Variable* var, std::string configName, std::string configVal) {
  AstNode* ret = nullptr;
  CHPL_ASSERT(var->isConfig());
  CHPL_ASSERT(!configName.empty());
  // TODO: how to handle nested module configs e.g., -sFoo.Baz.bar=10
  owned<AstNode> initNode = parseDummyNodeForInitExpr(var, configVal);
  ret = initNode.get();
  // create a last column value, add 1 for the initial column and 1 for the `=`
  int lastColumn = configName.length() + configVal.length() + 2;
  auto loc = Location(ret->id().symbolPath(), 1,1,1,lastColumn);
  noteChildrenLocations(ret, loc);
  addOrReplaceInitExpr(var->toVariable(), std::move(initNode));
  return ret;
}

/**
 * Create a dummy input for a variable and parse it to extract the initExpr
 */
owned <AstNode> Builder::parseDummyNodeForInitExpr(Variable* var, std::string value) {
  std::string inputText;
  // for types, it's important for the parser to see that it's a type
  if (var->kind() == uast::Variable::TYPE) {
    inputText = "type " + var->name().str() + "=" + value + ";";
  } else {
    inputText = "const " + var->name().str() + "=";
    inputText += (!value.empty()) ? value : "true";
    inputText += ";";
  }
  auto parser = parsing::Parser::createForTopLevelModule(context());
  std::string path = "Command-line arg (";
  path += var->name().str();
  path += ")";
  auto parseResult = parser.parseString(path.c_str(), inputText.c_str());
  auto mod = parseResult.singleModule();
  CHPL_ASSERT(mod);
  owned<AstNode> initNode;
  if (mod->stmt(0)->isVariable()) {
    // steal the init expression, children_ will have nullptr in place
    initNode = std::move(mod->children_[0]->children_.back());
    // clean out the nullptr
    mod->children_[0]->children_.pop_back();
  } else {
    CHPL_ASSERT(false && "should only be an assignment or type initializer");
  }
  return initNode;
}

/**
 * recursively set child locations to `loc`
 */
void Builder::noteChildrenLocations(AstNode* ast, Location loc) {
  notedLocations_[ast] = loc;
  for (auto &child : this->mutableRefToChildren(ast)) {
    noteChildrenLocations(child.get(), loc);
  }
}

AstList Builder::flattenTopLevelBlocks(AstList lst) {
  AstList ret;

  for (auto& ast : lst) {
    if (ast->isBlock()) {
      for (auto& child : takeChildren(std::move(ast))) {
        ret.push_back(std::move(child));
      }
    } else {
      ret.push_back(std::move(ast));
    }
  }

  lst.clear();

  return ret;
}

Location Builder::getLocation(const AstNode* node) {
  auto got = notedLocations_.find(node);
  if (got != notedLocations_.end()) return got->second;
  chpl::Location empty;
  return empty;
}

} // namespace uast
} // namespace chpl
