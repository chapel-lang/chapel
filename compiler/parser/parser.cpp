/*
 * Copyright 2004-2017 Cray Inc.
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

#include "parser.h"

#include "bison-chapel.h"
#include "build.h"
#include "config.h"
#include "countTokens.h"
#include "docsDriver.h"
#include "expr.h"
#include "files.h"
#include "flex-chapel.h"
#include "insertLineNumbers.h"
#include "stringutil.h"
#include "symbol.h"

#include <cstdlib>

BlockStmt*           yyblock                       = NULL;
const char*          yyfilename                    = NULL;
int                  yystartlineno                 = 0;

ModTag               currentModuleType             = MOD_INTERNAL;

int                  chplLineno                    = 0;
bool                 chplParseString               = false;
const char*          chplParseStringMsg            = NULL;

bool                 currentFileNamedOnCommandLine = false;
bool                 parsed                        = false;

static bool          sFirstFile                    = true;
static bool          sHandlingInternalModulesNow   = false;

static void          countTokensInCmdLineFiles();



static void          parseInternalModules();

static void          parseCommandLineFiles();

static void          parseDependentModules(ModTag modtype);

static void          ensureRequiredStandardModulesAreParsed();

static ModuleSymbol* parseMod(const char* modname, ModTag modType);

static ModuleSymbol* parseFile(const char* filename,
                               ModTag      modType,
                               bool        namedOnCommandLine);

static void          addModulePaths();

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void parse() {
  yydebug = debugParserLevel;

  if (countTokens == true) {
    countTokensInCmdLineFiles();
  }

  parseInternalModules();

  addModulePaths();

  addDashMsToUserPath();

  if (printSearchDirs) {
    printModuleSearchPath();
  }

  parseCommandLineFiles();

  checkConfigs();

  convertForallExpressions();

  finishCountingTokens();

  parsed = true;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Vec<const char*> sModNameSet;
static Vec<const char*> sModNameList;
static Vec<const char*> sModDoneSet;
static Vec<UseStmt*>    sModReqdByInt;

void addModuleToParseList(const char* name, UseStmt* useExpr) {
  const char* modName = astr(name);

  if (sModDoneSet.set_in(modName) == NULL &&
      sModNameSet.set_in(modName) == NULL) {
    if (currentModuleType           == MOD_INTERNAL ||
        sHandlingInternalModulesNow == true) {
      sModReqdByInt.add(useExpr);
    }

    sModNameSet.set_add(modName);
    sModNameList.add(modName);
  }
}

static void addModulePaths() {
  int         fileNum       = 0;
  const char* inputFilename = 0;

  while ((inputFilename = nthFilename(fileNum++))) {
    if (isChplSource(inputFilename) == true) {
      addModulePathFromFilename(inputFilename);
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void countTokensInCmdLineFiles() {
  int         fileNum       = 0;
  const char* inputFilename = 0;

  while ((inputFilename = nthFilename(fileNum++))) {
    if (isChplSource(inputFilename) == true) {
      parseFile(inputFilename, MOD_USER, true);
    }
  }

  finishCountingTokens();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void gatherIteratorTags();
static void gatherWellKnownTypes();
static void gatherWellKnownFns();

static void parseInternalModules() {
  //
  // If we're running chpldoc on just a single file, we don't want to
  // bring in all the base, standard, etc. modules -- just the file
  // we're documenting.
  //
  if (fDocs == false || fDocsProcessUsedModules == true) {
    baseModule            = parseMod("ChapelBase",           MOD_INTERNAL);
    standardModule        = parseMod("ChapelStandard",       MOD_INTERNAL);
    printModuleInitModule = parseMod("PrintModuleInitOrder", MOD_INTERNAL);

    parseDependentModules(MOD_INTERNAL);

    gatherIteratorTags();
    gatherWellKnownTypes();
    gatherWellKnownFns();
  }
}

static void gatherIteratorTags() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (strcmp(ts->name, iterKindTypename) == 0) {
      if (EnumType* enumType = toEnumType(ts->type)) {
        for_alist(expr, enumType->constants) {
          if (DefExpr* def = toDefExpr(expr)) {
            const char* name = def->sym->name;

            if        (strcmp(name, iterKindLeaderTagname)     == 0) {
              gLeaderTag     = def->sym;

            } else if (strcmp(name, iterKindFollowerTagname)   == 0) {
              gFollowerTag   = def->sym;

            } else if (strcmp(name, iterKindStandaloneTagname) == 0) {
              gStandaloneTag = def->sym;
            }
          }
        }
      }
    }
  }
}

// This structure and the following array provide a list of types that must be
// defined in module code.  At this point, they are all classes.
struct WellKnownType
{
  const char*     name;
  AggregateType** type_;
  bool            isClass;
};

// These types are a required part of the compiler/module interface.
static WellKnownType sWellKnownTypes[] = {
  { "_array",                &dtArray,            false },
  { "_tuple",                &dtTuple,            false },
  { "locale",                &dtLocale,           true  },
  { "chpl_localeID_t",       &dtLocaleID,         false },
  { "BaseArr",               &dtBaseArr,          true  },
  { "BaseDom",               &dtBaseDom,          true  },
  { "BaseDist",              &dtDist,             true  },
  { "chpl_main_argument",    &dtMainArgument,     false },
  { "chpl_comm_on_bundle_t", &dtOnBundleRecord,   false },
  { "chpl_task_bundle_t",    &dtTaskBundleRecord, false },
  { "Error",                 &dtError,            true  }
};

// Gather well-known types from among types known at this point.
static void gatherWellKnownTypes() {
  int nEntries = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);

  // Harvest well-known types from among the global type definitions.
  // We check before assigning to the well-known type dt<typename>,
  // to ensure that it is null.  In that way we can flag duplicate
  // definitions.
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    for (int i = 0; i < nEntries; ++i) {
      WellKnownType& wkt = sWellKnownTypes[i];

      if (strcmp(ts->name, wkt.name) == 0) {
        if (*wkt.type_ != NULL) {
          USR_WARN(ts,
                   "'%s' defined more than once in Chapel internal modules.",
                   wkt.name);
        }

        INT_ASSERT(ts->type);

        if (wkt.isClass == true && isClass(ts->type) == false) {
          USR_FATAL_CONT(ts->type,
                         "The '%s' type must be a class.",
                         wkt.name);
        }

        *wkt.type_ = toAggregateType(ts->type);
      }
    }
  }

  //
  // When compiling for minimal modules, we don't require any specific
  // well-known types to be defined.
  //
  if (fMinimalModules == false) {
    // Make sure all well-known types are defined.
    for (int i = 0; i < nEntries; ++i) {
      WellKnownType& wkt = sWellKnownTypes[i];

      if (*wkt.type_ == NULL) {
        USR_FATAL_CONT("Type '%s' must be defined in the "
                       "Chapel internal modules.",
                       wkt.name);
      }
    }

    USR_STOP();

  } else {
    if (dtString->symbol == NULL) {
      // This means there was no declaration of the string type.
      gAggregateTypes.remove(gAggregateTypes.index(dtString));

      delete dtString;

      dtString = NULL;
    }
  }
}

struct WellKnownFn
{
  const char* name;
  FnSymbol**  fn;
  Flag        flag;
  FnSymbol*   lastNameMatchedFn;
};

// These functions are a required part of the compiler/module interface.
// They should generally be marked export so that they are always
// resolved.
static WellKnownFn sWellKnownFns[] = {
  {
    "chpl_here_alloc",
    &gChplHereAlloc,
    FLAG_LOCALE_MODEL_ALLOC
  },

  {
    "chpl_here_free",
    &gChplHereFree,
    FLAG_LOCALE_MODEL_FREE
  },

  {
    "chpl_doDirectExecuteOn",
    &gChplDoDirectExecuteOn,
    FLAG_UNKNOWN
  },

  {
    "_build_tuple",
    &gBuildTupleType,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "_build_tuple_noref",
    &gBuildTupleTypeNoRef,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "*",
    &gBuildStarTupleType,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "_build_star_tuple_noref",
    &gBuildStarTupleTypeNoRef,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "chpl_delete_error",
    &gChplDeleteError,
    FLAG_UNKNOWN
  }
};

static void gatherWellKnownFns() {
  int nEntries = sizeof(sWellKnownFns) / sizeof(sWellKnownFns[0]);

  // Harvest well-known functions from among the global fn definitions.
  // We check before assigning to the associated global to ensure that it
  // is null.  In that way we can flag duplicate definitions.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    for (int i = 0; i < nEntries; ++i) {
      WellKnownFn& wkfn = sWellKnownFns[i];

      if (strcmp(fn->name, wkfn.name) == 0) {
        wkfn.lastNameMatchedFn = fn;

        if (wkfn.flag == FLAG_UNKNOWN || fn->hasFlag(wkfn.flag) == true) {
          if (*wkfn.fn != NULL) {
            USR_WARN(fn,
                     "'%s' defined more than once in Chapel internal modules.",
                     wkfn.name);
          }

          *wkfn.fn = fn;
        }
      }
    }
  }

  //
  // When compiling for minimal modules, we don't require any specific
  // well-known functions to be defined.
  //
  if (fMinimalModules == false) {
    // Make sure all well-known functions are defined.
    for (int i = 0; i < nEntries; ++i) {
      WellKnownFn& wkfn        = sWellKnownFns[i];
      FnSymbol*    lastMatched = wkfn.lastNameMatchedFn;
      FnSymbol*    fn          = *wkfn.fn;

      if (lastMatched == NULL) {
        USR_FATAL_CONT("Function '%s' must be defined in the "
                       "Chapel internal modules.",
                       wkfn.name);

      } else if (fn == NULL) {
        USR_FATAL_CONT(fn,
                       "The '%s' function is missing a required flag.",
                       wkfn.name);
      }
    }

    USR_STOP();
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void parseCommandLineFiles() {
  int         fileNum       = 0;
  const char* inputFilename = 0;

  while ((inputFilename = nthFilename(fileNum++))) {
    if (isChplSource(inputFilename) == true) {
      parseFile(inputFilename, MOD_USER, true);
    }
  }

  if (fDocs == false || fDocsProcessUsedModules == true) {
    parseDependentModules(MOD_USER);
    ensureRequiredStandardModulesAreParsed();

    forv_Vec(ModuleSymbol, mod, allModules) {
      mod->addDefaultUses();
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void parseDependentModules(ModTag modtype) {
  forv_Vec(const char*, modName, sModNameList) {
    if (sModDoneSet.set_in(modName) == NULL &&
        parseMod(modName, modtype)  != NULL) {
      sModDoneSet.set_add(modName);
    }
  }

  // Clear the list of things we need.  On the first pass, this
  // will be the standard modules used by the internal modules which
  // are already captured in the modReqdByInt vector and will be dealt
  // with by the conditional below.  On the second pass, we're done
  // with these data structures, so clearing them out is just fine.
  sModNameList.clear();
  sModNameSet.clear();
}

/************************************* | **************************************
*                                                                             *
* Ensure all of the standard modules that are required for internal modules   *
* have been parsed.                                                           *
*                                                                             *
************************************** | *************************************/

static void ensureRequiredStandardModulesAreParsed() {
  do {
    Vec<UseStmt*> modReqdByIntCopy = sModReqdByInt;

    sModReqdByInt.clear();

    sHandlingInternalModulesNow = true;

    forv_Vec(UseStmt*, moduse, modReqdByIntCopy) {
      BaseAST*           moduleExpr     = moduse->src;
      UnresolvedSymExpr* oldModNameExpr = toUnresolvedSymExpr(moduleExpr);

      if (oldModNameExpr == NULL) {
        INT_FATAL("It seems an internal module is using a mod.submod form");
      }

      const char* modName  = oldModNameExpr->unresolved;
      bool        foundInt = false;
      bool        foundUsr = false;

      forv_Vec(ModuleSymbol, mod, allModules) {
        if (strcmp(mod->name, modName) == 0) {
          if (mod->modTag == MOD_STANDARD || mod->modTag == MOD_INTERNAL) {
            foundInt = true;
          } else {
            foundUsr = true;
          }
        }
      }

      // if we haven't found the standard version of the module,
      // then we need to parse it
      if (foundInt == false) {
        if (const char* filename = stdModNameToFilename(modName)) {
          ModuleSymbol* mod = parseFile(filename, MOD_STANDARD, false);

          // If we also found a user module by the same name,
          // we need to rename the standard module and the use of it
          if (foundUsr == true) {
            SET_LINENO(oldModNameExpr);

            UnresolvedSymExpr* newModNameExpr = NULL;

            if (mod == NULL) {
              INT_FATAL("Trying to rename a standard module that's part of\n"
                        "a file defining multiple\nmodules doesn't work yet;\n"
                        "see "
                        "test/modules/bradc/modNamedNewStringBreaks.future "
                        "for details");
            }

            mod->name      = astr("chpl_", modName);

            newModNameExpr = new UnresolvedSymExpr(mod->name);

            oldModNameExpr->replace(newModNameExpr);
          }
        }
      }
    }
  } while (sModReqdByInt.n != 0);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static ModuleSymbol* parseMod(const char* modName, ModTag modType) {
  bool          isInternal = (modType == MOD_INTERNAL) ? true : false;
  bool          isStandard = false;
  ModuleSymbol* retval     = NULL;

  if (const char* path = modNameToFilename(modName, isInternal, &isStandard)) {
    if (isInternal == false && isStandard == true) {
      modType = MOD_STANDARD;
    }

    retval = parseFile(path, modType, false);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void addModuleToDoneList(ModuleSymbol* module);
static bool containsOnlyModules(BlockStmt* block, const char* filename);

static ModuleSymbol* parseFile(const char* fileName,
                               ModTag      modType,
                               bool        namedOnCommandLine) {
  ModuleSymbol* retval = NULL;




  if (FILE* fp = openInputFile(fileName)) {
    gFilenameLookup.push_back(fileName);

    // State for the lexer
    int           lexerStatus  = 100;

    // State for the parser
    yypstate*     parser       = yypstate_new();
    int           parserStatus = YYPUSH_MORE;
    YYLTYPE       yylloc;
    ParserContext context;

    currentFileNamedOnCommandLine = namedOnCommandLine;

    currentModuleType             = modType;

    yyblock                       = NULL;
    yyfilename                    = fileName;
    yystartlineno                 = 1;

    yylloc.first_line             = 1;
    yylloc.first_column           = 0;
    yylloc.last_line              = 1;
    yylloc.last_column            = 0;

    chplLineno                    = 1;

    if (printModuleFiles && (modType != MOD_INTERNAL || developer)) {
      if (sFirstFile) {
        fprintf(stderr, "Parsing module files:\n");

        sFirstFile = false;
      }

      fprintf(stderr, "  %s\n", cleanFilename(fileName));
    }

    if (namedOnCommandLine == true) {
      startCountingFileTokens(fileName);
    }

    yylex_init(&context.scanner);

    stringBufferInit();

    yyset_in(fp, context.scanner);

    while (lexerStatus != 0 && parserStatus == YYPUSH_MORE) {
      YYSTYPE yylval;

      lexerStatus = yylex(&yylval, &yylloc, context.scanner);

      if        (lexerStatus >= 0) {
        parserStatus          = yypush_parse(parser,
                                             lexerStatus,
                                             &yylval,
                                             &yylloc,
                                             &context);

      } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
        context.latestComment = yylval.pch;
      }
    }

    if (namedOnCommandLine == true) {
      stopCountingFileTokens(context.scanner);
    }

    // Cleanup after the parser
    yypstate_delete(parser);

    // Cleanup after the lexer
    yylex_destroy(context.scanner);

    closeInputFile(fp);

    if (yyblock == NULL) {
      INT_FATAL("yyblock should always be non-NULL after yyparse()");

    } else if (yyblock->body.head                     == NULL ||
               containsOnlyModules(yyblock, fileName) == false) {
      const char* moduleName = filenameToModulename(fileName);

      retval = buildModule(moduleName, yyblock, yyfilename, false, NULL);

      theProgram->block->insertAtTail(new DefExpr(retval));

      addModuleToDoneList(retval);

    } else {
      ModuleSymbol* moduleLast  = 0;
      int           moduleCount = 0;

      for_alist(stmt, yyblock->body) {
        if (BlockStmt* block = toBlockStmt(stmt))
          stmt = block->body.first();

        if (DefExpr* defExpr = toDefExpr(stmt)) {
          if (ModuleSymbol* modSym = toModuleSymbol(defExpr->sym)) {

            theProgram->block->insertAtTail(defExpr->remove());

            addModuleToDoneList(modSym);

            moduleLast  = modSym;
            moduleCount = moduleCount + 1;
          }
        }
      }

      if (moduleCount == 1)
        retval = moduleLast;
    }

    yyfilename                    =  NULL;

    yylloc.first_line             =    -1;
    yylloc.first_column           =     0;
    yylloc.last_line              =    -1;
    yylloc.last_column            =     0;

    yystartlineno                 =    -1;
    chplLineno                    =    -1;

    currentFileNamedOnCommandLine = false;

  } else {
    fprintf(stderr,
            "ParseFile: Unable to open \"%s\" for reading\n",
            fileName);
  }



  return retval;
}

static void addModuleToDoneList(ModuleSymbol* module) {
  sModDoneSet.set_add(astr(module->name));
}

static bool containsOnlyModules(BlockStmt* block, const char* filename) {
  int           moduleDefs     =     0;
  bool          hasUses        = false;
  bool          hasOther       = false;
  ModuleSymbol* lastmodsym     =  NULL;
  BaseAST*      lastmodsymstmt =  NULL;

  for_alist(stmt, block->body) {
    if (BlockStmt* block = toBlockStmt(stmt))
      stmt = block->body.first();

    if (DefExpr* defExpr = toDefExpr(stmt)) {
      ModuleSymbol* modsym = toModuleSymbol(defExpr->sym);

      if (modsym != NULL) {
        lastmodsym     = modsym;
        lastmodsymstmt = stmt;
        moduleDefs++;
      } else {
        hasOther = true;
      }

    } else if (toCallExpr(stmt)) {
      hasOther = true;
    } else if (toUseStmt(stmt)) {
      hasUses = true;
    } else {
      hasOther = true;
    }
  }

  if (hasUses && !hasOther && moduleDefs == 1) {
    USR_WARN(lastmodsymstmt,
             "as written, '%s' is a sub-module of the module created for "
             "file '%s' due to the file-level 'use' statements.  If you "
             "meant for '%s' to be a top-level module, move the 'use' "
             "statements into its scope.",
             lastmodsym->name,
             filename,
             lastmodsym->name);

  }

  return !hasUses && !hasOther && moduleDefs > 0;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

BlockStmt* parseString(const char* string,
                       const char* filename,
                       const char* msg) {
  // State for the lexer
  YY_BUFFER_STATE handle       =   0;
  int             lexerStatus  = 100;
  YYLTYPE         yylloc;

  // State for the parser
  yypstate*       parser       = yypstate_new();
  int             parserStatus = YYPUSH_MORE;
  ParserContext   context;

  yylex_init(&(context.scanner));

  stringBufferInit();

  handle              = yy_scan_string(string, context.scanner);

  yyblock             = NULL;
  yyfilename          = filename;

  chplParseString     = true;
  chplParseStringMsg  = msg;

  yylloc.first_line   = 1;
  yylloc.first_column = 0;

  yylloc.last_line    = 1;
  yylloc.last_column  = 0;

  while (lexerStatus != 0 && parserStatus == YYPUSH_MORE) {
    YYSTYPE yylval;

    lexerStatus  = yylex(&yylval, &yylloc, context.scanner);

    if (lexerStatus >= 0) {
      parserStatus          = yypush_parse(parser,
                                           lexerStatus,
                                           &yylval,
                                           &yylloc,
                                           &context);

    } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
      context.latestComment = yylval.pch;
    }
  }

  chplParseString    = false;
  chplParseStringMsg = NULL;

  // Cleanup after the parser
  yypstate_delete(parser);

  // Cleanup after the lexer
  yy_delete_buffer(handle, context.scanner);
  yylex_destroy(context.scanner);

  return yyblock;
}
