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

#ifndef CHPL_PARSING_PARSING_QUERIES_H
#define CHPL_PARSING_PARSING_QUERIES_H

#include "chpl/parsing/FileContents.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/ID.h"
#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/Module.h"
#include "chpl/parsing/parser-stats.h"

#include <vector>

namespace chpl {
namespace parsing {

using ConfigSettingsList = std::vector<std::pair<std::string, std::string>>;

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

/**
  Like parseFileToBuilderResult but parses whatever file contained 'id'.
  Useful for projection queries.
 */
const uast::BuilderResult*
parseFileContainingIdToBuilderResult(Context* context, ID id);

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
  Return the current module search path.
 */
const std::vector<UniqueString>& moduleSearchPath(Context* context);

/**
  Sets the current module search path.
 */
void setModuleSearchPath(Context* context,
                         std::vector<UniqueString> searchPath);

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
 */
bool idIsInInternalModule(Context* context, ID id);

/**
 Returns true if the ID corresponds to something in a bundled module.
 */
bool idIsInBundledModule(Context* context, ID id);

/**
 This query parses a toplevel module by name. Returns nullptr
 if no such toplevel module can be found in the module search path.
 */
const uast::Module* getToplevelModule(Context* context, UniqueString name);

/**
 This query parses a submodule for 'include submodule'.
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
 Returns true if the ID is a parenless function.
 */
bool idIsParenlessFunction(Context* context, ID id);

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
 Returns the parent AST node given an AST node
 */
const uast::AstNode* parentAst(Context* context, const uast::AstNode* node);

/**
  Returns the ID for the module containing the given ID,
  or the empty ID when given a toplevel module.
 */
ID idToParentModule(Context* context, ID id);

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

/**
  Given an ID, returns the attributes associated with the ID. Only
  declarations can have associated attributes.
 */
const uast::AttributeGroup* idToAttributeGroup(Context* context, ID id);

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

} // end namespace parsing
} // end namespace chpl
#endif
