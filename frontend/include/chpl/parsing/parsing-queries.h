/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_PARSING_PARSING_QUERIES_H
#define CHPL_PARSING_PARSING_QUERIES_H

#include "chpl/framework/Context.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/ID.h"
#include "chpl/framework/Location.h"
#include "chpl/parsing/FileContents.h"
#include "chpl/parsing/parser-stats.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/Module.h"

#include <vector>

namespace chpl {
namespace parsing {

using ConfigSettingsList = std::vector<std::pair<std::string, std::string>>;
using AttributeToolNamesList = std::vector<UniqueString>;
/**
 This query returns the contents of a file as the string field in the
 FileContents.
 In case there is an error reading the file, the error is stored in the error
 field of the FileContents.
 */
const FileContents& fileText(Context* context, std::string path);

/**
 Convenience function to call fileText given a UniqueString.
 */
const FileContents& fileText(Context* context, UniqueString path);

/**
 This function sets the FileContents that will be returned by the fileText
 query above.
 */
void setFileText(Context* context, std::string path, FileContents result);

/**
 This function sets the string that will be returned by the fileText
 query above. The FileContents stored will have an empty ErrorMessage field.
 */
void setFileText(Context* context, std::string path, std::string text);

/**
 This function sets the string that will be returned by the fileText
 query above. The FileContents stored will have an empty ErrorMessage field.
 */
void setFileText(Context* context, UniqueString path, std::string text);


/**
 This function returns `true` if the current revision already has contents
 stored in the fileText query for the given path.
 */
bool hasFileText(Context* context, const std::string& path);

/**
  This query reads a file (with the fileText query) and then parses it.

  The 'parentSymbolPath' is relevant for submodules that are in separate files
  with 'module include'. When parsing the included module for a 'module
  include', 'parentSymbolPath' should match the symbolPath of the ID of the
  module containing the 'module include' statement.

  When parsing a toplevel module, 'parentSymbolPath' should be "".
 */
const uast::BuilderResult&
parseFileToBuilderResult(Context* context, UniqueString path,
                         UniqueString parentSymbolPath);

/**
  Like parseFileToBuilderResult but also runs post-parse checks on the resulting
  builder result.
 */
const uast::BuilderResult&
parseFileToBuilderResultAndCheck(Context* context, UniqueString path,
                                 UniqueString parentSymbolPath);

std::vector<const uast::AstNode*>
introspectParsedTopLevelExpressions(Context* context);

std::vector<UniqueString>
introspectParsedFiles(Context* context);

/**
  Like parseFileToBuilderResult but parses whatever file contained 'id'.
  Useful for projection queries.

  If setParentSymbolPath is not nullptr, sets it to the parentSymbolPath used
  when creating the BuilderResult.
 */
const uast::BuilderResult*
parseFileContainingIdToBuilderResult(Context* context, ID id,
                                     UniqueString* setParentSymbolPath=nullptr);

/**
  Fetch the BuilderResult storing compiler-generated uAST based on the given
  symbolPath.
 */
const uast::BuilderResult&
getCompilerGeneratedBuilder(Context* context, UniqueString symbolPath);

/**
  Set the BuilderResult storing compiler-generated uAST based on the given
  symbolPath.
 */
void setCompilerGeneratedBuilder(Context* context, UniqueString symbolPath,
                                 uast::BuilderResult result);

/**
  A function for counting the tokens when parsing
*/
void countTokens(Context* context, UniqueString path, ParserStats* parseStats);
// TODO: Expose this in a more reasonable manner


// These functions can't return the Location for a Comment
// because Comments don't have IDs. If Locations for Comments are needed,
// instead use the astToLocation field from the result of
// parseFileToBuilderResult.

/**
 This query returns the Location where a particular ID appeared.
 It cannot be used for Comments because Comments don't have IDs set.
 If Locations for Comments are needed, use uast::BuilderResult::commentToLocation
 */
const Location& locateId(Context* context, ID id);
/**
 This function just runs locateId on ast->id(). Similarly to locateID,
 it cannot be used to get a Location for a Comment.
 */
const Location& locateAst(Context* context, const uast::AstNode* ast);

/** Also define getters for additional locations such as e.g., dot fields.
    A complete list can be seen in "chpl/uast/all-location-maps.h".
    The form is e.g., `locateDotFieldWithId(Context* context, ID id)` or
    `locateDotFieldWithAst(Context* context, const Dot* dot)`.

    Additional location maps for things like dot fields are required because
    they are not themselves AST nodes.
*/
#define LOCATION_MAP(ast__, location__) \
  Location locate##location__##WithId(Context* context, ID id); \
  Location locate##location__##WithAst(Context* context, \
                                       const uast::ast__* ast);
#include "chpl/uast/all-location-maps.h"
#undef LOCATION_MAP

using ModuleVec = std::vector<const uast::Module*>;
/**
 This query returns a vector of parsed modules given a file path.

  The 'parentSymbolPath' is relevant for submodules that are in separate files
  with 'module include'. When parsing the included module for a 'module
  include', 'parentSymbolPath' should match the symbolPath of the ID of the
  module containing the 'module include' statement.

  When parsing a toplevel module, 'parentSymbolPath' should be "".

  Unlike 'parseFileToBuilderResult' this query will report any errors
  encountered while parsing to the context.
 */
const ModuleVec& parse(Context* context, UniqueString path,
                       UniqueString parentSymbolPath);

/**
  Convenience function to parse a file with parentSymbolPath="". Any errors
  encountered while parsing are reported to the context.
 */
const ModuleVec& parseToplevel(Context* context, UniqueString path);

/**
  Parse a file (with parentSymbolPath="", so not a submodule)
  and return the IDs of the top-level modules that it contains.
 */
const std::vector<ID>& toplevelModulesInFile(Context* context,
                                             UniqueString path);

/**
  Given the modules from files named on the command line,
  determine which module is the main module, and return it.

  requestedMainModuleName can be "", but if it is not, it should
  be the name of a module in commandLineModules.

  If libraryMode is set, most errors in determining the main
  module will be ignored. This mode is useful for library compilation,
  where the main module concept does not make sense.
 */
ID findMainModule(Context* context,
                  std::vector<ID> commandLineModules,
                  UniqueString requestedMainModuleName,
                  bool libraryMode);

/**
  Convenience function to compute the main module ID and command-line module IDs
  based upon paths provided at the command line.

  Returns the command-line module IDs and sets mainModule to the main module ID.
 */
std::vector<ID>
findMainAndCommandLineModules(Context* context,
                              std::vector<UniqueString> paths,
                              UniqueString requestedMainModuleName,
                              bool libraryMode,
                              ID& mainModule);

/**
  Return the current module search path.
 */
const std::vector<UniqueString>& moduleSearchPath(Context* context);

/**
  Sets the current module search path.
 */
void setModuleSearchPath(Context* context,
                         std::vector<UniqueString> searchPath);

/**
  Return a list of paths to be prepended to the internal module path. This is
  likely to be empty unless using --prepend-internal-module-dir when compiling
*/
const std::vector<UniqueString>& prependedInternalModulePath(Context* context);

/**
  Set a list of paths to be prepended to the internal module path. This is
  typically set using the compiler flag --prepend-internal-module-dir and will
  be called during setupModuleSearchPaths
*/
void setPrependedInternalModulePath(Context* context,
                                    std::vector<UniqueString> paths);

/**
  Return a list of paths to be prepended to the standard module path. This is
  likely to be empty unless using --prepend-standard-module-dir when compiling
*/
const std::vector<UniqueString>& prependedStandardModulePath(Context* context);


/**
  Set a list of paths to be prepended to the standard module path. This is
  typically set using the compiler flag --prepend-standard-module-dir and will
  be called during setupModuleSearchPaths
*/
void setPrependedStandardModulePath(Context* context,
                                    std::vector<UniqueString> paths);

/**
  Return the current internal module path, i.e. CHPL_HOME/modules/internal/
 */
UniqueString internalModulePath(Context* context);

/**
  Set the current internal modules directory, i.e. CHPL_HOME/modules/internal/
  This should be formed in a consistent manner with setModuleSearchPath,
  so that this is a prefix for some module search paths.
 */
void setInternalModulePath(Context* context, UniqueString path);

/**
  Return the current standard module path, i.e. CHPL_HOME/modules/
 */
UniqueString bundledModulePath(Context* context);

/**
  Set the current bundled modules directory, i.e. CHPL_HOME/modules/
  This should be formed in a consistent manner with setModuleSearchPath,
  so that this is a prefix for some module search paths.
 */
void setBundledModulePath(Context* context, UniqueString path);

/**
  Helper to call setModuleSearchPath, setInternalModulePath, standardModulePath.
  This function accepts the path to CHPL_HOME, and any additional
  module path components (from environment variable and command line).

  Most of these arguments have corresponding env / printchplenv settings:
    chplHome             -- CHPL_HOME
    chplLocaleModel      -- CHPL_LOCALE_MODEL
    chplTasks            -- CHPL_TASKS
    chplComm             -- CHPL_COMM
    chplSysModulesSubdir -- CHPL_SYS_MODULES_SUBDIR
    chplModulePath       -- CHPL_MODULE_PATH

  The arguments 'prependInternalModulePaths' and 'prependStandardModulePaths',
  if non-empty, allow one to override where the context will search for
  internal and standard modules, respectively. It will search each successive
  path in the vector before consulting the default locations.

  These arguments facilitate support for frontend flags with similar names,
  allowing users to diagnose problems with internal/standard modules in the
  field. Instead of upgrading their Chapel installation, patched versions
  of problematic modules can be swapped in instead.
 */
void setupModuleSearchPaths(
                  Context* context,
                  const std::string& chplHome,
                  bool minimalModules,
                  const std::string& chplLocaleModel,
                  bool enableTaskTracking,
                  const std::string& chplTasks,
                  const std::string& chplComm,
                  const std::string& chplSysModulesSubdir,
                  const std::string& chplModulePath,
                  const std::vector<std::string>& prependInternalModulePaths,
                  const std::vector<std::string>& prependStandardModulePaths,
                  const std::vector<std::string>& cmdLinePaths,
                  const std::vector<std::string>& inputFilenames);

/**
  Overload of the more general setupModuleSearchPaths that uses the
  context's stored chplHome and chplEnv to determine the values of most
  arguments.
*/
void setupModuleSearchPaths(Context* context,
                            bool minimalModules,
                            bool enableTaskTracking,
                            const std::vector<std::string>& cmdLinePaths,
                            const std::vector<std::string>& inputFilenames);

/**
 Returns true if the ID corresponds to something in an internal module.
 If the internal module path is empty, this function returns false.

 Also considers paths from --prepend-internal-module-dir, if any.
 */
bool idIsInInternalModule(Context* context, ID id);

/**
 Returns true if the ID corresponds to something in a bundled module.
 If the bundled module path is empty, this function returns false.

 Also considers paths, if any, from --prepend-internal-module-dir
 and --prepend-standard-module-dir.
 */
bool idIsInBundledModule(Context* context, ID id);

/**
 Returns true if the ID corresponds to something in a standard module.
 A standard module is a bundled module, but it is not a package module
 (which may be contributed by users and are not subject to the same
 constraints as standard modules) and not an internal module.

 If the bundled module path is empty, this function returns false.

 Also considers paths from --prepend-standard-module-dir, if any.
 */
bool idIsInStandardModule(Context* context, ID id);


/**
 Returns true if the file path refers to the internal modules.
 (Typically, that would be if it begins with $CHPL_HOME/modules/internal).
 If the internal module path is empty, this function returns false.

 Also considers paths from --prepend-internal-module-dir, if any.
 */
bool
filePathIsInInternalModule(Context* context, UniqueString filePath);

/**
 Returns true if the file path corresponds to the standard modules.
 A standard module is a bundled module, but it is not a package module
 (which may be contributed by users and are not subject to the same
 constraints as standard modules) and not an internal module.
 (So, in other words, it would typically be a standard module if
 it begins with $CHPL_HOME/modules/ but doesn't begin with
 $CHPL_HOME/modules/internal or $CHPL_HOME/modules/packages;
 this will include $CHPL_HOME/modules/standard and $CHPL_HOME/modules/dists).

 If the bundled module path is empty, this function returns false.

 Also considers paths from --prepend-standard-module-dir, if any.
 */
bool
filePathIsInStandardModule(Context* context, UniqueString filePath);

/**
 Returns true if the file path corresponds to the bundled modules.
 (Typically, that would be if it begins with $CHPL_HOME/modules/).
 If the bundled module path is empty, this function returns false.

 Also considers paths, if any, from --prepend-internal-module-dir
 and --prepend-standard-module-dir.
 */
bool
filePathIsInBundledModule(Context* context, UniqueString filePath);

/**
  Check if a file exists. This is a query, so that duplicate checks
  can be avoided.
  Returns true if the file exists, false otherwise.

  requireFileCaseMatches helps with case-insensitive filesystems:

  'requireFileCaseMatches=true' causes this function to work with
  a directory listing & if the filename component of 'path' isn't
  found in its parent directory listing, this function returns 'false'.

  'requireFileCaseMatches=false' allows a case-insensitive filesystem
  to indicate that 'AA' exists if the filesystem has the file 'Aa'.
 */
bool checkFileExists(Context* context,
                     std::string path,
                     bool requireFileCaseMatches);

/**
 This helper function checks if a file exists at a particular path.
 If it does, it returns a normalized form of the path to that file.
 If not, it returns the empty string.
 */
std::string getExistingFileAtPath(Context* context, std::string path);

/**
 This helper function checks the module search path for
 an existing file named fname.

 If multiple such files exist in different directories,
 it will choose the first and emit an ambiguity warning.

 This function uses case-sensitive matching against the directory listings,
 so 'use Module' refers to 'Module.chpl' even on case-insensitive filesystems.

 If nothing is found, returns the empty string.
 */
std::string getExistingFileInModuleSearchPath(Context* context,
                                              const std::string& fname);

/**
 This query parses a toplevel module by name. Returns nullptr
 if no such toplevel module can be found in the module search path.
 */
const uast::Module* getToplevelModule(Context* context, UniqueString name);

/**
 Given a particular (presumably standard) module, return the ID of a symbol
 with the given name in that module. Beyond creating the ID, this also ensures
 that the standard module is parsed, and thus, that 'idToAst' on the returned
 ID will return a non-null value.
 */
ID getSymbolFromTopLevelModule(Context* context,
                               const char* modName,
                               const char* symName);

/**
 This query parses a submodule for 'include submodule'.
 The ID passed should be the ID of an Include statement.
 Returns nullptr if no such file can be found.
 */
const uast::Module* getIncludedSubmodule(Context* context,
                                         ID includeModuleId);

/**
 Returns the uast node with the given ID.
 */
const uast::AstNode* idToAst(Context* context, ID id);

/**
 Returns the tag for the node with the given ID.
 */
uast::AstTag idToTag(Context* context, ID id);

/**
  Returns true if the ID is a module.
 */
bool idIsModule(Context* context, ID id);

/**
 Returns true if the ID is a parenless function.
 */
bool idIsParenlessFunction(Context* context, ID id);

/**
 Returns true if the ID is a nested function.
 */
bool idIsNestedFunction(Context* context, ID id);

/**
 Returns true if the ID refers to a private declaration.
 */
bool idIsPrivateDecl(Context* context, ID id);

/**
 Returns true if the ID is a function.
 */
bool idIsFunction(Context* context, ID id);

/**
 Returns true if the ID is marked 'extern'.
 */
bool idIsExtern(Context* context, ID id);

/**
 Returns true if the ID is marked 'export'.
 */
bool idIsExport(Context* context, ID id);

/**
 Returns true if the ID is a method.
 */
bool idIsMethod(Context* context, ID id);

/**
 If the ID represents a field in a record/class/union, returns
 the name of that field. Otherwise, returns the empty string.
 */
UniqueString fieldIdToName(Context* context, ID id);

/**
 Returns true if the ID is a field in a record/class/union.
 */
bool idIsField(Context* context, ID id);

/**
 Returns the parent ID given an ID
 */
const ID& idToParentId(Context* context, ID id);

/**
 Returns the parent function ID given an ID.
 */
ID idToParentFunctionId(Context* context, ID id);

/**
 Returns the parent AST node given an AST node
 */
const uast::AstNode* parentAst(Context* context, const uast::AstNode* node);

/**
  Returns the ID for the module containing the given ID,
  or the empty ID when given a toplevel module.
 */
ID idToParentModule(Context* context, ID id);

/**
  Returns 'true' if ID refers to a toplevel module.
 */
bool idIsToplevelModule(Context* context, ID id);

/**
 Given an ID that represents a Function, get the declared return
 intent for that function.
 */
uast::Function::ReturnIntent idToFnReturnIntent(Context* context, ID id);

/**
 Returns 'true' if the passed ID represents a Function with a where clause,
 and 'false' otherwise.
 */
bool idIsFunctionWithWhere(Context* context, ID id);

/**
  Given an ID for a Variable, returns the ID of the containing
  MultiDecl or TupleDecl, if any, and the ID of the variable otherwise.
 */
ID idToContainingMultiDeclId(Context* context, ID id);

/**
  Given an ID for a Record/Union/Class Decl,
  returns 'true' if the passed name is the name of a field contained in it.
 */
bool idContainsFieldWithName(Context* context, ID typeDeclId,
                             UniqueString fieldName);

/**
  Given an ID for a Record/Union/Class Decl,
  and a field name, returns the ID for the Variable declaring that field.
 */
ID fieldIdWithName(Context* context, ID typeDeclId,
                   UniqueString fieldName);

/**
  Given an ID for a Record/Union/Class Decl, returns 'true'
  if that Record/Union/Class Decl directly contains one or more
  'forwarding' declarations.
 */
bool aggregateUsesForwarding(Context* context, ID typeDeclId);

/**
 * Store config settings that were set from the command line using -s flags
 */
void setConfigSettings(Context* context, ConfigSettingsList keys);

/**
 * Get any config settings that were set from the command line and stored
 */
const
ConfigSettingsList& configSettings(Context* context);


void setAttributeToolNames(Context* context, AttributeToolNamesList keys);

const AttributeToolNamesList& AttributeToolNames(Context* context);

/**
  Given an ID, returns the attributes associated with the ID. This is important
  to use or else attributes for children of MultiDecls or TupleDecls will not
  be found.
 */
const uast::AttributeGroup* idToAttributeGroup(Context* context, ID id);

/**
  Given an AstNode, returns the attributes associated with it. This is important
  to use or else attributes for children of MultiDecls or TupleDecls will not
  be found.
 */
const uast::AttributeGroup*
astToAttributeGroup(Context* context, const uast::AstNode* node);

/**
  Given an ID 'idMention' representing a mention of a symbol, and an
  ID 'idTarget' representing the symbol, determine if a deprecation
  warning should be produced for 'idTarget' at 'idMention'. If so,
  the warning will be reported to the context.

  A warning will be reported to the context only once per revision.
  It may not be reported if the context or compiler is configured
  to suppress deprecation warnings.

  The 'idMention' may refer to any AST but will most often be an
  Identifier. The 'idTarget' should refer to a NamedDecl. If it does
  not, then nothing is reported.
*/
void reportDeprecationWarningForId(Context* context, ID idMention,
                                   ID idTarget);

/**
  Returns the state of --warn-unstable or -internal or -standard
  depending on which of these applies to 'filepath'.
*/
bool shouldWarnUnstableForPath(Context* context, UniqueString filepath);

/**
  Returns the state of --warn-unstable or -internal or -standard
  depending on which of these applies to 'id'.
*/
bool shouldWarnUnstableForId(Context* context, const ID& id);

/**
  Given an ID 'idMention' representing a mention of a symbol, and an
  ID 'idTarget' representing the symbol, determine if an unstable
  warning should be produced for 'idTarget' at 'idMention'. If so,
  the warning will be reported to the context.

  A warning will be reported to the context only once per revision.
  It may not be reported if the context or compiler is configured
  to suppress unstable warnings.

  The 'idMention' may refer to any AST but will most often be an
  Identifier. The 'idTarget' should refer to a NamedDecl. If it does
  not, then nothing is reported.
*/
void reportUnstableWarningForId(Context* context, ID idMention,
                                ID idTarget);

/*
  Given an ID, returns the module kind for the ID.
*/
uast::Module::Kind idToModuleKind(Context* context, ID id);

/*
  Given a unique string, determine if it matches the name of a known special
  method.
*/
bool isSpecialMethodName(UniqueString name);

} // end namespace parsing
} // end namespace chpl
#endif
