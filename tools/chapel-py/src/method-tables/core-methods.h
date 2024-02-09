/*
 * Copyright 2023-2024 Hewlett Packard Enterprise Development LP
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

// See method-tables.h for a top-level description of what the X-macros
// in this file do.

//
// Inside each method table, methods should be listed in alphabetical order
//

CLASS_BEGIN(Context)
  PLAIN_GETTER(Context, introspect_parsed_files, "Inspect the list of files that have been parsed by the Context",
               std::vector<chpl::UniqueString>, return parsing::introspectParsedFiles(node))
  PLAIN_GETTER(Context, track_errors, "Return a context manager that tracks errors emitted by this Context",
               PyObject*, (void) node; return createNewErrorManager(contextObject))
  PLAIN_GETTER(Context, _get_pyi_file, "Generate a stub file for the Chapel AST nodes",
               std::string, (void) node; return generatePyiFile())

  METHOD(Context, parse, "Parse a top-level AST node from the given file",
         std::vector<const chpl::uast::AstNode*>(chpl::UniqueString),

         auto fileNameUS = std::get<0>(args);
         auto parentPathUS = chpl::UniqueString();
         auto& builderResult =
           parsing::parseFileToBuilderResultAndCheck(node, fileNameUS, parentPathUS);
         std::vector<const chpl::uast::AstNode*> topLevelNodes;
         for (auto i = 0; i < builderResult.numTopLevelExpressions(); i++) {
           topLevelNodes.push_back(builderResult.topLevelExpression(i));
         }
         return topLevelNodes)
  METHOD(Context, set_module_paths, "Set the module path arguments to the given lists of module paths and filenames",
         void(std::vector<std::string>, std::vector<std::string>),

         auto& paths = std::get<0>(args);
         auto& filenames = std::get<1>(args);
         parsing::setupModuleSearchPaths(node, false, false, paths, filenames))
  METHOD(Context, is_bundled_path, "Check if the given file path is within the bundled (built-in) Chapel files",
         bool(chpl::UniqueString),

         auto pathUS = std::get<0>(args);
         return parsing::filePathIsInInternalModule(node, pathUS) ||
                parsing::filePathIsInStandardModule(node, pathUS) ||
                parsing::filePathIsInBundledModule(node, pathUS))
  METHOD(Context, advance_to_next_revision, "Advance the context to the next revision",
         void(bool),

         auto prepareToGc = std::get<0>(args);
         node->advanceToNextRevision(prepareToGc))
CLASS_END(Context)

CLASS_BEGIN(Location)
  PLAIN_GETTER(Location, start, "Get the line-column pair where this Location starts",
               LineColumnPair, return std::make_tuple(node->firstLine(), node->firstColumn()))
  PLAIN_GETTER(Location, end, "Get the line-column pair where this Location ends",
               LineColumnPair, return std::make_tuple(node->lastLine(), node->lastColumn()))
  PLAIN_GETTER(Location, path, "Get the file path of this Location",
               chpl::UniqueString, return node->path())
CLASS_END(Location)

CLASS_BEGIN(Scope)
  PLAIN_GETTER(Scope, used_imported_modules, "Get the modules that were used or imported in this scope",
               std::vector<const chpl::uast::AstNode*>,

               auto& moduleIds = resolution::findUsedImportedModules(context, node);
               std::set<ID> reportedIds;
               std::vector<const chpl::uast::AstNode*> toReturn;
               for (size_t i = 0; i < moduleIds.size(); i++) {
                 auto& id = moduleIds[i];
                 if (!reportedIds.insert(id).second) continue;
                 toReturn.push_back(parsing::idToAst(context, id));
               }
               return toReturn)
CLASS_END(Scope)

CLASS_BEGIN(Error)
  PLAIN_GETTER(Error, location, "Get the location at which this error occurred",
               chpl::Location, return node->location(context))
  PLAIN_GETTER(Error, message, "Retrieve the contents of this error message",
               std::string, return node->message())
  PLAIN_GETTER(Error, kind, "Retrieve the kind ('error', 'warning') of this type of error",
               const char*, return chpl::ErrorBase::getKindName(node->kind()))
  PLAIN_GETTER(Error, type, "Retrieve the unique name of this type of error",
               const char*, return chpl::ErrorBase::getTypeName(node->type()))
CLASS_END(Error)

CLASS_BEGIN(ErrorManager)
  PLAIN_GETTER(ErrorManager, __enter__, "The context manager 'enter' method for this ErrorManager object",
               PyObject*,

               (void) node;
               auto list = ((PythonErrorHandler*) context->errorHandler())->pushList();
               Py_INCREF(list);
               return list)

  METHOD(ErrorManager, __exit__, "The context manager 'enter' method for this ErrorManager object",
         void(PyObject*, PyObject*, PyObject*),

         (void) node;
         ((PythonErrorHandler*) context->errorHandler())->popList())
CLASS_END(ErrorManager)
