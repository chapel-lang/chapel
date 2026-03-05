/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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
               ErrorManagerObject*, std::ignore = node; return ErrorManagerObject::create(contextObject, std::make_tuple()))
  PLAIN_GETTER(Context, _get_pyi_file, "Generate a stub file for the Chapel AST nodes",
               std::string, std::ignore = node; return generatePyiFile())

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
         parsing::setupModuleSearchPaths(node, false, paths, filenames);
         if (auto autoUseScope = resolution::scopeForAutoModule(node)) {
           std::ignore = resolution::resolveVisibilityStmts(node, autoUseScope, false);
         })
  METHOD(Context, _set_module_paths, "Set the module path arguments to the given lists of module paths and filenames, using a potentially different module root",
          void(std::string, std::vector<std::string>, std::vector<std::string>),

          auto& modRoot = std::get<0>(args);
          auto& paths = std::get<1>(args);
          auto& filenames = std::get<2>(args);
          parsing::setupModuleSearchPaths(node, modRoot, false, paths, filenames);
          if (auto autoUseScope = resolution::scopeForAutoModule(node)) {
            std::ignore = resolution::resolveVisibilityStmts(node, autoUseScope, false);
          })
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
  METHOD(Context, get_file_text, "Get the text of the file at the given path",
         std::string(chpl::UniqueString), return parsing::fileText(node, std::get<0>(args)).text())
  METHOD(Context, get_compiler_version, "Get the version of the Chapel compiler",
         std::string(), std::ignore = node; return chpl::getVersion())
  METHOD(Context, get_chpl_home, "Get the CHPL_HOME path for this Context",
         std::string(), std::ignore = node; return context->chplHome())
CLASS_END(Context)

CLASS_BEGIN(Location)
  PLAIN_GETTER(Location, start, "Get the line-column pair where this Location starts",
               LineColumnPair, return std::make_tuple(node->firstLine(), node->firstColumn()))
  PLAIN_GETTER(Location, end, "Get the line-column pair where this Location ends",
               LineColumnPair, return std::make_tuple(node->lastLine(), node->lastColumn()))
  PLAIN_GETTER(Location, path, "Get the file path of this Location",
               chpl::UniqueString, return node->path())
  METHOD(Location, clamp_left, "Get a new Location removes the left part of the current Location based on another Location",
         chpl::Location(chpl::Location),
         auto left = node;
         auto right = std::get<0>(args);

         return Location(left->path(), std::max(left->start(), right.start()), left->end());
  )
  METHOD(Location, modify_start, "Get a new Location with the same end as this Location but with the start adjusted by the given line and column offsets",
         chpl::Location(LineColumnPair),
         auto start = std::get<0>(args);
         auto newStart = std::make_tuple(node->firstLine() + std::get<0>(start),
                                         node->firstColumn() + std::get<1>(start));
         return Location(node->path(), newStart, node->end());
  )
  METHOD(Location, modify_end, "Get a new Location with the same start as this Location but with the end adjusted by the given line and column offsets",
         chpl::Location(LineColumnPair),
         auto end = std::get<0>(args);
         auto newEnd = std::make_tuple(node->lastLine() + std::get<0>(end),
                                       node->lastColumn() + std::get<1>(end));
         return Location(node->path(), node->start(), newEnd);
  )
  OPERATOR_PROTOTYPE(Location, __add__, "Get a new Location that spans from the start of this Location to the end of another Location", chpl::Location(chpl::Location))
  OPERATOR_PROTOTYPE(Location, __iadd__, "", chpl::Location(chpl::Location))
  OPERATOR_PROTOTYPE(Location, __sub__, "Get a new Location is the result of removing the part of this Location that overlaps with another Location", chpl::Location(chpl::Location))
  OPERATOR_PROTOTYPE(Location, __isub__, "", chpl::Location(chpl::Location))
CLASS_END(Location)

CLASS_BEGIN(Scope)
  PLAIN_GETTER(Scope, modules_named_in_use_or_import, "Get the modules that were named in use or import statements directly within this scope",
               std::vector<const chpl::uast::Module*>,

               std::set<ID> reportedIds;
               std::vector<const chpl::uast::Module*> toReturn;
               if (node != nullptr && node->containsUseImport()) {
                 if (auto r = resolveVisibilityStmts(context, node)) {
                   for (const auto& id: r->modulesNamedInUseOrImport()) {
                     if (!reportedIds.insert(id).second) continue;
                     auto ast = parsing::idToAst(context, id);
                     if (auto mod = ast->toModule()) toReturn.push_back(mod);
                   }
                 }
               }
               return toReturn)
  PLAIN_GETTER(Scope, parent_scope, "Get the parent (outer) scope of this scope",
               Nilable<const chpl::resolution::Scope*>, return node->parentScope())
  PLAIN_GETTER(Scope, visible_nodes, "Get the nodes corresponding to declarations exported from this scope",
               std::vector<VisibleSymbol>,
               std::vector<VisibleSymbol> toReturn;
               for (auto& pair : getSymbolsAvailableInScope(context, node)) {
                 std::vector<const chpl::uast::AstNode*> into;
                 for (auto id : pair.second) {
                    if (id.isEmpty()) continue;
                    into.push_back(parsing::idToAst(context, id));
                 }

                 if (!into.empty()) {
                   toReturn.emplace_back(pair.first, std::move(into));
                 }
               }

               return toReturn)
CLASS_END(Scope)

CLASS_BEGIN(ResolvedExpression)
  PLAIN_GETTER(ResolvedExpression, most_specific_candidate, "If this node is a call, return the most specific overload selected by call resolution.",
               std::optional<MostSpecificCandidateObject*>,

               if (auto& msc = node->mostSpecific().only()) {
                 return MostSpecificCandidateObject::create(contextObject, {&msc, node->poiScope()});
               }
               return {})

  // Note: calling node.resolve().type() -- thus using the below method --
  // should be equivalent to calling node.type(). The latter is just a more
  // convenient shortcut if additional information isn't needed.

  PLAIN_GETTER(ResolvedExpression, type, "Retrieve the type of the expression.",
               std::optional<QualifiedTypeTuple>,

               auto qt = node->type();
               if (qt.isUnknown()) {
                 return {};
               }

               return std::make_tuple(intentToString(qt.kind()), qt.type(), qt.param()))
CLASS_END(ResolvedExpression)

CLASS_BEGIN(MostSpecificCandidate)
  PLAIN_GETTER(MostSpecificCandidate, function, "Get the signature of the function called by this candidate.",
               TypedSignatureObject*, return TypedSignatureObject::create(contextObject, {node->candidate->fn(), node->poiScope }))

  // Note: calling node.resolve().formal_actual_mapping() -- thus using the
  // below method -- should be equivalent to calling node.formal_actual_mapping().
  // The latter is just a more convenient shortcut if additional information
  // isn't needed.

  PLAIN_GETTER(MostSpecificCandidate, formal_actual_mapping, "Get the index of the function's formal for each of the call's actuals.",
               std::vector<int>,

               std::vector<int> res;

               int i = 0;
               while (auto formalActual = node->candidate->formalActualMap().byActualIdx(i++)) {
                 res.push_back(formalActual->formalIdx());
               }
               return res)
CLASS_END(MostSpecificCandidate)

CLASS_BEGIN(TypedSignature)
  METHOD(TypedSignature, formal_type, "Get the type of the nth formal of this function signature",
         std::optional<QualifiedTypeTuple>(int),

         auto arg = std::get<int>(args);
         if (arg < 0 && arg >= node->signature->numFormals()) {
           return {};
         }

         auto& qt = node->signature->formalType(arg);
         if (qt.isUnknown()) {
           return {};
         }

         return std::make_tuple(intentToString(qt.kind()), qt.type(), qt.param()))
  PLAIN_GETTER(TypedSignature, is_instantiation, "Check if this function is an instantiation of a generic function",
               bool, return node->signature->instantiatedFrom() != nullptr)
  PLAIN_GETTER(TypedSignature, ast, "Get the AST from which this function signature is computed",
               Nilable<const chpl::uast::AstNode*>, return chpl::parsing::idToAst(context, node->signature->id()))
CLASS_END(TypedSignature)

CLASS_BEGIN(ApplicabilityResult)
  PLAIN_GETTER(ApplicabilityResult, candidate_failure_reason, "If this candidate was inapplicable, the reason why",
               const char*, return resolution::candidateFailureReasonToString(node->reason()))
  PLAIN_GETTER(ApplicabilityResult, formal_failure_reason, "If this candidate was inapplicable due to a particular formal parameter, the reason why",
               const char*, return resolution::passingFailureReasonToString(node->formalReason()))
  PLAIN_GETTER(ApplicabilityResult, formal_idx, "If this candidate was inapplicable due to a particular formal parameter, the index of that formal parameter",
               int, return node->formalIdx())
  PLAIN_GETTER(ApplicabilityResult, actual_idx, "If this candidate was inapplicable due to a particular formal parameter, the index of the corresponding actual parameter",
               int, return node->actualIdx())
  PLAIN_GETTER(ApplicabilityResult, candidate_is_method, "Check whether this candidate is a method",
               bool,

               auto errId = node->idForErr();
               if (!errId.isEmpty()) return parsing::idIsMethod(context, errId);

               if (auto candidate = node->candidate()) {
                 return candidate->isMethod();
               }
               return false)
CLASS_END(ApplicabilityResult)

CLASS_BEGIN(CallInfo)
  PLAIN_GETTER(CallInfo, is_method_call, "Check if this CallInfo represents a method call",
               bool, return node->isMethodCall())
  PLAIN_GETTER(CallInfo, has_question_arg, "Check if this CallInfo represents a call with a question argument",
               bool, return node->hasQuestionArg())
CLASS_END(CallInfo)
