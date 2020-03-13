/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "build.h"

#include "astutil.h"
#include "stlUtil.h"
#include "baseAST.h"
#include "CatchStmt.h"
#include "config.h"
#include "DeferStmt.h"
#include "docsDriver.h"
#include "driver.h"
#include "files.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "ImportStmt.h"
#include "LoopExpr.h"
#include "ParamForLoop.h"
#include "parser.h"
#include "stringutil.h"
#include "TryStmt.h"
#include "wellknown.h"

#include <map>
#include <utility>

static BlockStmt* findStmtWithTag(PrimitiveTag tag, BlockStmt* blockStmt);

void checkControlFlow(Expr* expr, const char* context) {
  Vec<const char*> labelSet; // all labels in expr argument
  Vec<BaseAST*> loopSet;     // all asts in a loop in expr argument
  Vec<BaseAST*> innerFnSet;  // all asts in a function in expr argument
  std::vector<BaseAST*> asts;
  collect_asts(expr, asts);

  //
  // compute labelSet and loopSet
  //
  for_vector(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (LabelSymbol* ls = toLabelSymbol(def->sym))
        labelSet.set_add(ls->name);
      else if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (!innerFnSet.set_in(fn)) {
          std::vector<BaseAST*> innerAsts;
          collect_asts(fn, innerAsts);
          for_vector(BaseAST, ast, innerAsts) {
            innerFnSet.set_add(ast);
          }
        }
      }
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->isLoopStmt() && !loopSet.set_in(block)) {
        if (block->userLabel != NULL) {
          labelSet.set_add(block->userLabel);
        }
        std::vector<BaseAST*> loopAsts;
        collect_asts(block, loopAsts);
        for_vector(BaseAST, ast, loopAsts) {
          loopSet.set_add(ast);
        }
      }
    }
  }

  //
  // check for illegal control flow
  //
  for_vector(BaseAST, ast1, asts) {
    if (CallExpr* call = toCallExpr(ast1)) {
      if (innerFnSet.set_in(call))
        continue; // yield or return is in nested function/iterator
      if (call->isPrimitive(PRIM_RETURN)) {
        USR_FATAL_CONT(call, "return is not allowed in %s", context);
      } else if (call->isPrimitive(PRIM_YIELD)) {
        if (!strcmp(context, "begin statement"))
          USR_FATAL_CONT(call, "yield is not allowed in %s", context);
      }
    } else if (GotoStmt* gs = toGotoStmt(ast1)) {
      if (labelSet.set_in(gs->getName()))
        continue; // break or continue target is in scope
      if (toSymExpr(gs->label) && toSymExpr(gs->label)->symbol() == gNil && loopSet.set_in(gs))
        continue; // break or continue loop is in scope
      if (!strcmp(context, "on statement")) {
        USR_PRINT(gs, "the following error is a current limitation");
      }
      if (gs->gotoTag == GOTO_BREAK) {
        USR_FATAL_CONT(gs, "break is not allowed in %s", context);
      } else if (gs->gotoTag == GOTO_CONTINUE) {
        // see also resolveGotoLabels() -> handleForallGoto()
        if (!strcmp(context, "forall statement")) {
          if (!strcmp(gs->getName(), "nil"))
            ; // plain 'continue' is OK in a forall
          else
            USR_FATAL_CONT(gs, "continue to a named loop outside of a forall is not allowed from inside the forall");
        } else {
          USR_FATAL_CONT(gs, "continue is not allowed in %s", context);
        }
      } else {
        USR_FATAL_CONT(gs, "illegal 'goto' usage; goto is deprecated anyway");
      }
    }
  }
}


static void addPragmaFlags(Symbol* sym, Vec<const char*>* pragmas) {
  forv_Vec(const char, str, *pragmas) {
    Flag flag = pragma2flag(str);
    if (flag == FLAG_UNKNOWN)
      USR_FATAL_CONT(sym, "unknown pragma: \"%s\"", str);
    else {
      sym->addFlag(flag);

      if (flag == FLAG_RUNTIME_TYPE_INIT_FN) {
        //
        // These functions must be marked as type functions early in
        // compilation, as calls to them are inserted by the compiler
        // at the declaration points for arrays and domains.  In the
        // past, they had to be defined as type functions in the
        // modules, but most of us found that very confusing because
        // the code in the functions actually returns a value.  See
        // buildRuntimTypeToValueFns() in functionResolution.cpp for
        // more info on what happens to these functions.
        //
        FnSymbol* fn = toFnSymbol(sym);
        INT_ASSERT(fn);
        if (fn->retTag != RET_VALUE) {
          USR_WARN(fn, "function's return type is not a value type.  Ignoring.");
        }
        fn->retTag = RET_TYPE;
      }
    }
  }
}

DefExpr* buildPragmaDefExpr(Vec<const char*>* pragmas, DefExpr* def) {
  addPragmaFlags(def->sym, pragmas);
  delete pragmas;
  return def;
}

BlockStmt* buildPragmaStmt(Vec<const char*>* pragmas,
                           BlockStmt* stmt) {
  bool error = false;
  for_alist(expr, stmt->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      addPragmaFlags(def->sym, pragmas);
    } else if (isEndOfStatementMarker(expr)) {
      // ignore it
    } else {
      error = true;
      break;
    }
  }
  if (error && pragmas->n > 0) {
    USR_FATAL_CONT(stmt, "cannot attach pragmas to this statement");
    USR_PRINT(stmt, "   %s \"%s\"",
              pragmas->n == 1 ? "pragma" : "starting with pragma",
              pragmas->v[0]);
  }
  delete pragmas;

  return stmt;
}


//
// A helper function that's useful in a few places in this file to
// conditionally convert an Expr to a 'const char*' in the event that
// it's an immediate string.
//
static const char* toImmediateString(Expr* expr) {
  if (SymExpr* se = toSymExpr(expr)) {
    if (VarSymbol* var = toVarSymbol(se->symbol())) {
      if (var->isImmediate()) {
        Immediate* imm = var->immediate;
        if (imm->const_kind == CONST_KIND_STRING) {
          return imm->v_string;
        }
      }
    }
  }
  return NULL;
}


//
// This is a simple utility function that converts new-style
// PRIM_ZIP() expressions to old-style _build_tuple() expressions for
// code that is not ready for the new style yet.
//
static void zipToTuple(Expr* zipPrim) {
  CallExpr* zipExpr = toCallExpr(zipPrim);
  assert(zipExpr->isPrimitive(PRIM_ZIP));
  zipExpr->primitive = NULL;
  zipExpr->baseExpr = new UnresolvedSymExpr("_build_tuple");
}


CallExpr* buildOneTuple(Expr* elem) {
  return new CallExpr("_build_tuple", elem);
}

CallExpr* buildTuple(CallExpr* call) {
  // The call is expected to be a PRIM_ACTUALS_LIST.
  INT_ASSERT(call->isPrimitive(PRIM_ACTUALS_LIST));
  return new CallExpr("_build_tuple", call);
}


Expr* buildSquareCallExpr(Expr* base, CallExpr* args) {
  CallExpr* call = new CallExpr(base, args);
  call->square = true;
  return call;
}


Expr* buildNamedActual(const char* name, Expr* expr) {
  return new NamedExpr(name, expr);
}


Expr* buildFormalArrayType(Expr* iterator, Expr* eltType, Expr* index) {
  if (index) {
    CallExpr* indexCall = toCallExpr(index);
    INT_ASSERT(indexCall);
    if (indexCall->numActuals() != 1)
      USR_FATAL(iterator, "invalid index expression");
    return new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", iterator),
             eltType, indexCall->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", iterator->copy()));
  } else {
    CallExpr* call = toCallExpr(iterator);
    if (call->numActuals() == 1 && isDefExpr(call->get(1))) {
      return new CallExpr("chpl__buildArrayRuntimeType", call->get(1)->remove(), eltType);
    } else
      return new CallExpr("chpl__buildArrayRuntimeType",
               new CallExpr("chpl__ensureDomainExpr", iterator), eltType);
  }
}

Expr* buildIntLiteral(const char* pch, const char* file, int line) {
  uint64_t ull;
  int len = strlen(pch);
  char* noUnderscores = (char*)malloc(len+1);

  // remove all underscores from the number
  int j = 0;
  for (int i=0; i<len; i++) {
    if (pch[i] != '_') {
      noUnderscores[j++] = pch[i];
    }
  }
  noUnderscores[j] = '\0';

  if (!strncmp("0b", pch, 2) || !strncmp("0B", pch, 2))
    ull = binStr2uint64(noUnderscores, true, file, line);
  else if (!strncmp("0o", pch, 2) || !strncmp("0O", pch, 2))
    // The second case is difficult to read, but is zero followed by a capital
    // letter 'o'
    ull = octStr2uint64(noUnderscores, true, file, line);
  else if (!strncmp("0x", pch, 2) || !strncmp("0X", pch, 2))
    ull = hexStr2uint64(noUnderscores, true, file, line);
  else {
    ull = str2uint64(noUnderscores, true, file, line);
  }

  free(noUnderscores);

  if (ull <= 9223372036854775807ull)
    return new SymExpr(new_IntSymbol(ull, INT_SIZE_64));
  else
    return new SymExpr(new_UIntSymbol(ull, INT_SIZE_64));
}


Expr* buildRealLiteral(const char* pch) {
  return new SymExpr(new_RealSymbol(pch));
}


Expr* buildImagLiteral(const char* pch) {
  char* str = strdup(pch);
  str[strlen(pch)-1] = '\0';
  SymExpr* se = new SymExpr(new_ImagSymbol(str));
  free(str);
  return se;
}


Expr* buildStringLiteral(const char* pch) {
  return new SymExpr(new_StringSymbol(pch));
}

Expr* buildBytesLiteral(const char* pch) {
  return new SymExpr(new_BytesSymbol(pch));
}

Expr* buildCStringLiteral(const char* pch) {
  return new SymExpr(new_CStringSymbol(pch));
}


Expr* buildDotExpr(BaseAST* base, const char* member) {
  // The following optimization was added to avoid calling
  // chpl_localeID_to_locale when all we end up doing is extracting
  // the locale id or the node id, thus: OPTIMIZATION:
  //   chpl_localeID_to_locale(_get_locale_id(x)).localeid ==>
  //      _get_locale_id(x)
  // AND
  //   chpl_localeID_to_locale(_get_locale_id(x)).id ==>
  //      _get_locale_node_id(x)
  //
  // <hilde complaints paraphrased by sungeun, who agrees>
  // - We shouldn't have optimizations in the parser
  // - These strcmps suck
  if (!strcmp("id", member) || !strcmp("localeid", member)) {
    if (CallExpr* intToLocale = toCallExpr(base))
      if (intToLocale->isNamed("chpl_localeID_to_locale"))
        if (CallExpr* getLocale = toCallExpr(intToLocale->get(1)))
          if (getLocale->isPrimitive(PRIM_WIDE_GET_LOCALE)) {
            if (!strcmp("id", member))
              return new CallExpr(PRIM_WIDE_GET_NODE, getLocale->get(1)->remove());
            else
              return new CallExpr(PRIM_WIDE_GET_LOCALE, getLocale->get(1)->remove());
          }
  }

  if (!strcmp("locale", member))
    // MAGIC: "x.locale" member access expressions are rendered as
    // chpl_localeID_to_locale(_wide_get_node(x)).
    return new CallExpr("chpl_localeID_to_locale",
                        new CallExpr(PRIM_WIDE_GET_LOCALE, base));
  else
    return new CallExpr(".", base, new_CStringSymbol(member));
}


Expr* buildDotExpr(const char* base, const char* member) {
  return buildDotExpr(new UnresolvedSymExpr(base), member);
}


BlockStmt* buildChapelStmt(Expr* expr) {
  return new BlockStmt(expr, BLOCK_SCOPELESS);
}

BlockStmt* buildErrorStandin() {
  return new BlockStmt(new CallExpr(PRIM_ERROR), BLOCK_SCOPELESS);
}

static void addModuleToSearchList(VisibilityStmt* newStmt, BaseAST* module) {
  UnresolvedSymExpr* modNameExpr = toUnresolvedSymExpr(module);
  if (modNameExpr) {
    addModuleToParseList(modNameExpr->unresolved, newStmt);
  } else if (CallExpr* callExpr = toCallExpr(module)) {
    addModuleToSearchList(newStmt, callExpr->argList.first());
  }
}


static BlockStmt* buildUseList(BaseAST* module, const char* newName,
                               BlockStmt* list, bool privateUse) {
  UseStmt* newUse = new UseStmt(module, newName, privateUse);
  addModuleToSearchList(newUse, module);
  if (list == NULL) {
    return buildChapelStmt(newUse);
  } else {
    list->insertAtTail(newUse);
    return list;
  }
}

//
// Given an expression from a 'require' statement, process it.  We
// assume it's going to either be a "-llib" flag, or a source filename
// like "foo.h", "foo.c", "foo.o", "foo.chpl", etc.  If this call is
// made at parseTime, we only look for ".chpl" files and add them to
// the list of source files we need to parse; if it's made later
// (i.e., function resolution time) then we add the string to our list
// of library information or to our list of source files.
//
bool processStringInRequireStmt(const char* str, bool parseTime,
                                const char* modFilename) {
  if (strncmp(str, "-l", 2) == 0) {
    if (!parseTime) {
      addLibFile(str+2); // skip past '-l'
      return true;
    }
  } else {
    if (isChplSource(str)) {
      if (parseTime) {
        addSourceFile(str, NULL);
        return true;
      } else {
        USR_FATAL("'require' cannot handle non-literal '.chpl' files");
        return false;
      }
    } else {
      if (!parseTime) {
        addSourceFile(str, modFilename);
        return true;
      }
    }
  }
  return false;
}

// UseStmt builder's helper.  If the Expr* provided was wrong in some way,
// generate the appropriate error message.
static void useListError(Expr* expr, bool except) {
  if (except) {
    USR_FATAL(expr, "incorrect expression in 'except' list, identifier expected");
  } else {
    USR_FATAL(expr, "incorrect expression in 'only' list, identifier expected");
  }
}

//
// Build a 'use' statement with an 'except'/'only' list
//
BlockStmt* buildUseStmt(Expr* mod, const char * rename,
                        std::vector<PotentialRename*>* names, bool except,
                        bool privateUse) {
  std::vector<const char*> namesList;
  std::map<const char*, const char*> renameMap;

  // Catch the 'except *' case and turn it into 'only <nothing>'.  This
  // case will have a single UnresolvedSymExpr named "".
  if (except && names->size() == 1) {
    PotentialRename* listElem = (*names)[0];
    if (UnresolvedSymExpr* name = toUnresolvedSymExpr(listElem->elem)) {
      if (name->unresolved[0] == '\0') {
        except = false;
      }
    }
  }

  // Iterate through the list of names to exclude when using mod
  for_vector(PotentialRename, listElem, *names) {
    switch (listElem->tag) {
      case PotentialRename::SINGLE:
        if (UnresolvedSymExpr* name = toUnresolvedSymExpr(listElem->elem)) {
          namesList.push_back(name->unresolved);
        } else {
          // Currently we expect only unresolved sym exprs
          useListError(listElem->elem, except);
        }
        break;
      case PotentialRename::DOUBLE:
        std::pair<Expr*, Expr*>* elem = listElem->renamed;
        // Need to check that we aren't renaming in an 'except' list
        if (except) {
          USR_FATAL(elem->first, "cannot rename in 'except' list");
        }
        UnresolvedSymExpr* old_name = toUnresolvedSymExpr(elem->first);
        UnresolvedSymExpr* new_name = toUnresolvedSymExpr(elem->second);
        if (old_name != NULL && new_name != NULL) {
          // Verify that the new name isn't already in the renameMap
          if (renameMap.count(new_name->unresolved) == 0) {
            renameMap[new_name->unresolved] = old_name->unresolved;
          } else {
            USR_FATAL_CONT(elem->first, "already renamed '%s' to '%s', renaming '%s' would conflict", renameMap[new_name->unresolved], new_name->unresolved, old_name->unresolved);
          }
        } else {
          useListError(elem->first, except);
        }
        break;
    }

  }

  UseStmt* newUse = new UseStmt(mod, rename, &namesList, except, &renameMap,
                                privateUse);
  addModuleToSearchList(newUse, mod);

  delete names;

  return buildChapelStmt(newUse);
}

BlockStmt* buildUseStmt(Expr* mod, Expr* rename,
                        std::vector<PotentialRename*>* names, bool except,
                        bool privateUse) {
  if (UnresolvedSymExpr* usym = toUnresolvedSymExpr(rename)) {
    return buildUseStmt(mod, usym->unresolved, names, except, privateUse);
  } else {
    USR_FATAL(rename, "incorrect expression in use statement rename, identifier expected");
    return NULL; // should never be reached, the USR_FATAL will halt execution
  }
}

//
// Build a 'use' statement
//
BlockStmt* buildUseStmt(std::vector<PotentialRename*>* args, bool privateUse) {
  BlockStmt* list = NULL;

  //
  // Iterate over the expressions being 'use'd, processing them
  //
  for_vector(PotentialRename, maybeRename, *args) {
    Expr* useArg = NULL;
    switch (maybeRename->tag) {
      case PotentialRename::SINGLE:
        useArg = maybeRename->elem;
        list = buildUseList(useArg, "", list, privateUse);
        break;
      case PotentialRename::DOUBLE:
        useArg = maybeRename->renamed->first;
        Expr* newNameExpr = maybeRename->renamed->second;
        UnresolvedSymExpr* newName = toUnresolvedSymExpr(newNameExpr);
        if (newName != NULL)
          list = buildUseList(useArg, newName->unresolved, list, privateUse);
        else
          USR_FATAL(newNameExpr, "incorrect expression in use statement rename, identifier expected");
        break;
    }
  }

  //
  // If all of them are consumed, replace the use statement by a no-op
  //
  if (list == NULL) {
    list = buildChapelStmt(new CallExpr(PRIM_NOOP));
  }

  return list;
}

//
// Build an 'import' statement
//
BlockStmt* buildImportStmt(Expr* mod) {
  ImportStmt* newImport = new ImportStmt(mod, /* isPrivate =*/ true);
  addModuleToSearchList(newImport, mod);

  return buildChapelStmt(newImport);
}

//
// Build an 'import' statement
//
BlockStmt* buildImportStmt(Expr* mod, const char* rename) {
  ImportStmt* newImport = new ImportStmt(mod, rename, /* isPrivate =*/ true);
  addModuleToSearchList(newImport, mod);

  return buildChapelStmt(newImport);
}

//
// Build a 'require' statement
//
BlockStmt* buildRequireStmt(CallExpr* args) {
  BlockStmt* list = NULL;

  //
  // Iterate over the expressions being 'require'd, processing them
  //
  for_actuals(expr, args) {
    Expr* useArg = expr->remove();

    //
    // if this is a string literal, process it if we should
    //
    if (const char* str = toImmediateString(useArg)) {
      if (processStringInRequireStmt(str, true, yyfilename)) {
        continue;
      }
    }
    //
    // otherwise, store it in a require statement to handle later
    // (during resolution)
    //
    CallExpr* requireCall = new CallExpr(PRIM_REQUIRE, useArg);
    if (list == NULL) {
      list = buildChapelStmt(requireCall);
    } else {
      list->insertAtTail(requireCall);
    }
  }

  //
  // If we didn't create anything, return a no-op
  //
  if (list == NULL) {
    list = buildChapelStmt(new CallExpr(PRIM_NOOP));
  }

  return list;
}

//
// Build a queried expression like `?t`
//
DefExpr* buildQueriedExpr(const char *expr) {
  return new DefExpr(new VarSymbol(&(expr[1])));
}

static void
buildTupleVarDeclHelp(Expr* base, BlockStmt* decls, Expr* insertPoint) {
  int count = 1;
  for_alist(expr, decls->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (strcmp(def->sym->name, "chpl__tuple_blank")) {
        def->init = new CallExpr(base->copy(), new_IntSymbol(count));
        insertPoint->insertBefore(def->remove());
      } else {
        def->remove();
      }
    } else if (BlockStmt* blk = toBlockStmt(expr)) {
      buildTupleVarDeclHelp(new CallExpr(base, new_IntSymbol(count)),
                            blk, insertPoint);
    } else {
      INT_FATAL(expr, "unexpected expression in buildTupleVarDeclHelp");
    }
    count++;
  }
  decls->remove();
}


BlockStmt*
buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init) {
  VarSymbol* tmp = newTemp();
  int count = 1;
  for_alist(expr, tupleBlock->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (strcmp(def->sym->name, "chpl__tuple_blank")) {
        def->init = new CallExpr(tmp, new_IntSymbol(count));
      } else {
        def->remove();
      }
    } else if (BlockStmt* blk = toBlockStmt(expr)) {
      buildTupleVarDeclHelp(new CallExpr(tmp, new_IntSymbol(count)), blk, expr);
    }
    count++;
  }
  //
  // Add compiler errors if tmp is not a tuple or if tmp.size is not the
  // same as the number of variables.  These checks will get inserted in
  // buildVarDecls after it asserts that only DefExprs are in this block.
  //
  tupleBlock->blockInfoSet(new CallExpr("_check_tuple_var_decl", tmp, new_IntSymbol(count-1)));
  tupleBlock->insertAtHead(new DefExpr(tmp, init, type));
  return tupleBlock;
}


BlockStmt*
buildLabelStmt(const char* name, Expr* stmt) {
  BlockStmt* block = toBlockStmt(stmt);

  if (block) {
    Expr* breakLabelStmt = block->body.tail;

    if (!isDefExpr(breakLabelStmt) && isDefExpr(breakLabelStmt->prev)) {
      // the last statement in the block could be a call to _freeIterator()
      breakLabelStmt = breakLabelStmt->prev;
    }

    if (LoopStmt* loop = toLoopStmt(breakLabelStmt->prev)) {
      if (loop->breakLabelGet() == 0 || loop->continueLabelGet() == 0) {
        USR_FATAL(stmt, "cannot label parallel loop");
      } else {
        loop->userLabel = astr(name);
      }
    } else {
      USR_FATAL(stmt, "can only label for-, while-do- and do-while-statements");
    }

  } else {
    USR_FATAL(stmt, "cannot label non-loop statement");
  }

  return block;
}


BlockStmt*
buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr) {
  return buildChapelStmt(new CondStmt(new CallExpr("_cond_test", condExpr), thenExpr, elseExpr));
}

BlockStmt*
buildExternBlockStmt(const char* c_code) {
  BlockStmt* useSysCTypes = buildUseList(new UnresolvedSymExpr("SysCTypes"),
                                         "", NULL, /* private = */ false);
  useSysCTypes->insertAtTail(new ExternBlockStmt(c_code));
  BlockStmt* ret = buildChapelStmt(useSysCTypes);

  // Check that the compiler supports extern blocks
  // but skip these checks for chpldoc.
  if (fDocs == false) {
#ifdef HAVE_LLVM
    // Chapel was built with LLVM
    // Just bring up an error if extern blocks are disabled
    if (externC == false)
      USR_FATAL(ret, "extern block syntax is turned off. Use "
                     "--extern-c flag to turn on.");
#else
    // If Chapel wasn't built with LLVM, we can't handle extern blocks
    USR_FATAL(ret, "Chapel must be built with llvm in order to "
                    "use the extern block syntax");
#endif
  }

  return ret;
}

ModuleSymbol* buildModule(const char* name,
                          ModTag      modTag,
                          BlockStmt*  block,
                          const char* filename,
                          bool        priv,
                          bool        prototype,
                          const char* docs) {
  ModuleSymbol* mod = new ModuleSymbol(name, modTag, block);

  if (currentFileNamedOnCommandLine) {
    mod->addFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE);
  }

  if (priv == true) {
    mod->addFlag(FLAG_PRIVATE);
  }

  if (prototype == true) {
    mod->addFlag(FLAG_PROTOTYPE_MODULE);
  }

  mod->filename = astr(filename);
  mod->doc      = docs;

  return mod;
}



CallExpr* buildPrimitiveExpr(CallExpr* exprs) {
  INT_ASSERT(exprs->isPrimitive(PRIM_ACTUALS_LIST));
  if (exprs->argList.length == 0)
    INT_FATAL("primitive has no name");
  Expr* expr = exprs->get(1);
  expr->remove();
  if (const char* primname = toImmediateString(expr)) {
    if (PrimitiveOp* prim = primitives_map.get(primname)) {
      return new CallExpr(prim, exprs);
    } else {
      INT_FATAL(expr, "primitive not found '%s'", primname);
    }
  } else {
    INT_FATAL(expr, "primitive with non-literal string name");
  }
  return NULL;
}


CallExpr* buildLetExpr(BlockStmt* decls, Expr* expr) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(astr("chpl_let_fn", istr(uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  fn->addFlag(FLAG_INLINE);
  fn->insertAtTail(decls);
  fn->insertAtTail(new CallExpr(PRIM_RETURN, expr));
  if (fWarnUnstable) {
    USR_WARN(decls, "Let expressions are currently unstable and are expected to change in ways that will break their current uses.");
  }
  return new CallExpr(new DefExpr(fn));
}

BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  BlockStmt *sbody = new BlockStmt();
  VarSymbol *serial_state = newTemp();
  sbody->insertAtTail(new DefExpr(serial_state, new CallExpr(PRIM_GET_SERIAL)));
  sbody->insertAtTail(new CondStmt(cond, new CallExpr(PRIM_SET_SERIAL, gTrue)));
  sbody->insertAtTail(body);
  sbody->insertAtTail(new CallExpr(PRIM_SET_SERIAL, serial_state));
  return sbody;
}


//
// check validity of indices in loops and expressions
//
void
checkIndices(BaseAST* indices) {
  if (CallExpr* call = toCallExpr(indices)) {
    if (!call->isNamed("_build_tuple"))
      USR_FATAL(indices, "invalid index expression");
    for_actuals(actual, call)
      checkIndices(actual);
  } else if (!isSymExpr(indices) && !isUnresolvedSymExpr(indices))
    USR_FATAL(indices, "invalid index expression");
}

static Expr* destructureIndicesAfter(Expr* insertAfter,
                                     BaseAST* indices,
                                     Expr* init,
                                     bool coforall);

void destructureIndices(BlockStmt* block,
                        BaseAST* indices,
                        Expr* init,
                        bool coforall) {
  Expr* insertPt = new CallExpr(PRIM_NOOP);
  block->insertAtHead(insertPt);
  destructureIndicesAfter(insertPt, indices, init, coforall);
  insertPt->remove();
}

// Returns the next value for insertAfter
static Expr* destructureIndicesAfter(Expr* insertAfter,
                                     BaseAST* indices,
                                     Expr* init,
                                     bool coforall) {
  if (CallExpr* call = toCallExpr(indices)) {
    if (call->isNamed("_build_tuple")) {
      int i = 1;

      // Add checks that the index has tuple type of the right shape.
      CallExpr* checkCall = new CallExpr("_check_tuple_var_decl",
                                         init->copy(),
                                         new_IntSymbol(call->numActuals()));
      insertAfter->insertAfter(checkCall);
      insertAfter = checkCall;

      for_actuals(actual, call) {
        if (UnresolvedSymExpr* use = toUnresolvedSymExpr(actual)) {
          if (!strcmp(use->unresolved, "chpl__tuple_blank")) {
            i++;
            continue;
          }
        }

        CallExpr* call = new CallExpr(init->copy(), new_IntSymbol(i));
        insertAfter = destructureIndicesAfter(insertAfter, actual,
                                              call, coforall);
        i++;
      }
    } else {
      INT_FATAL("Unexpected call type");
    }
  } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(indices)) {
    VarSymbol* var = new VarSymbol(sym->unresolved);
    DefExpr* def = new DefExpr(var);
    insertAfter->insertAfter(def);
    CallExpr* move = new CallExpr(PRIM_MOVE, var, init);
    def->insertAfter(move);
    insertAfter = move;
    var->addFlag(FLAG_INDEX_VAR);
    if (coforall)
      var->addFlag(FLAG_COFORALL_INDEX_VAR);
    var->addFlag(FLAG_INSERT_AUTO_DESTROY);
  } else if (SymExpr* sym = toSymExpr(indices)) {
    // BHARSH TODO: I think this should be a PRIM_ASSIGN. I've seen a case
    // where 'sym' becomes a reference.
    CallExpr* move = new CallExpr(PRIM_MOVE, sym->symbol(), init);
    insertAfter->insertAfter(move);
    insertAfter = move;
    sym->symbol()->addFlag(FLAG_INDEX_VAR);
    if (coforall)
      sym->symbol()->addFlag(FLAG_COFORALL_INDEX_VAR);
    sym->symbol()->addFlag(FLAG_INSERT_AUTO_DESTROY);
  } else {
    INT_FATAL("Unexpected");
  }
  return insertAfter;
}


// builds body of for expression iterator
Expr*
buildForLoopExpr(Expr* indices, Expr* iteratorExpr, Expr* expr, Expr* cond, bool maybeArrayType, bool zippered) {
  if (zippered) zipToTuple(iteratorExpr);
  return new LoopExpr(indices, iteratorExpr, cond, expr, /*forall=*/ false, zippered, maybeArrayType);
}

Expr*
buildForallLoopExpr(Expr* indices, Expr* iteratorExpr, Expr* expr, Expr* cond, bool maybeArrayType, bool zippered) {
  if (zippered) zipToTuple(iteratorExpr);
  return new LoopExpr(indices, iteratorExpr, cond, expr, /*forall=*/ true, zippered, maybeArrayType);
}

//
// This is a helper function that takes a chpl_buildArrayRuntimeType(...)
// CallExpr and converts it into a forall loop expression.  See the
// commit messages of r20820 and the commit that added this comment
// for (a few) more details.
//
Expr* buildForallLoopExprFromArrayType(CallExpr* buildArrTypeCall,
                                           bool recursiveCall) {
  // Is this a call to chpl__buildArrayRuntimeType?
  UnresolvedSymExpr* ursym = toUnresolvedSymExpr(buildArrTypeCall->baseExpr);
  if (ursym && strcmp(ursym->unresolved, "chpl__buildArrayRuntimeType") == 0) {
    // If so, let's process it...

    // [i in 1..10] <type expr using 'i'>;
    //
    // That pattern is currently not valid, and is expected to trigger an
    // error message during resolution.
    Expr* indices = NULL;
    if (buildArrTypeCall->numActuals() >= 3) {
      indices = buildArrTypeCall->get(3)->remove();
    }

    Expr* EltExpr = buildArrTypeCall->get(2)->remove();
    Expr* DomExpr = buildArrTypeCall->get(1)->remove();

    // if the element type is itself an array, we need to do this same
    // conversion to forall loops recursively
    if (CallExpr* EltExprAsCall = toCallExpr(EltExpr)) {
      EltExpr = buildForallLoopExprFromArrayType(EltExprAsCall, true);
    }
    return buildForallLoopExpr(indices, DomExpr, EltExpr, NULL, true);
  } else {
    // if we get something other than a "build array runtime type" call...
    if (recursiveCall) {
      // ...we're in the base case if this was a recursive call
      return buildArrTypeCall;
    } else {
      // ...or something is wrong if we're not
      INT_FATAL("buildForallLoopExprFromArrayType() wasn't called with a call to chpl__buildArrayRuntimeType as expected");
      return NULL;
    }
  }
}

static CallExpr* makeUnmanagedNew(Expr* typeArg, Expr* arg) {
  return new CallExpr(PRIM_NEW,
                      new CallExpr(typeArg, arg,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))));
}

static void adjustMinMaxReduceOp(Expr* reduceOp) {
  if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(reduceOp)) {
    if (!strcmp(sym->unresolved, "max"))
      sym->unresolved = astr("MaxReduceScanOp");
    else if (!strcmp(sym->unresolved, "min"))
      sym->unresolved = astr("MinReduceScanOp");
  }
}

void addTaskIntent(CallExpr* ti, ShadowVarSymbol* svar) {
  Expr* ovar = new UnresolvedSymExpr(svar->name);
  if (Expr* ri = svar->reduceOpExpr()) {
    // This is a reduce intent. NB 'intent' is undefined.
    adjustMinMaxReduceOp(ri);
    ti->insertAtTail(ri);
    ti->insertAtTail(ovar);
  } else {
    ArgSymbol* tiMark = tiMarkForForallIntent(svar);
    INT_ASSERT(tiMark != NULL);
    ti->insertAtTail(tiMark);
    ti->insertAtTail(ovar);
  }
}

static CallExpr* copyByrefVars(CallExpr* byrefVarsSource) {
  if (!byrefVarsSource) return NULL;
  return byrefVarsSource->copy();
}

static void
addByrefVars(BlockStmt* target, CallExpr* byrefVarsSource) {
  // nothing to do if there is no 'ref' clause
  if (!byrefVarsSource) return;

  //
  // 'byrefVars' will contain a CallExpr, whose "arguments"
  // are variables listed in the with(ref...) clause
  // of the enclosing begin/cobegin/coforall.
  // This list is processed during createTaskFunctions().
  //
  // Could set byrefVars->parentExpr/Symbol right here.
  target->byrefVars = byrefVarsSource;

  // Note: the UnresolvedSymExprs in byrefVars
  // will be automatically resolved in resolve().
}


// Build up a "lowered" coforall loop. We lower coforalls into for-loops with
// explicit fork-join task creation via an EndCount.
static BlockStmt* buildLoweredCoforall(Expr* indices,
                                       VarSymbol* iterator,
                                       CallExpr* byref_vars,
                                       BlockStmt* body,
                                       bool zippered,
                                       bool bounded) {

  BlockStmt* taskBlk = new BlockStmt();
  taskBlk->insertAtHead(body);

  VarSymbol* coforallCount = newTempConst("_coforallCount");
  coforallCount->addFlag(FLAG_END_COUNT);
  VarSymbol* numTasks = newTemp("numTasks");
  VarSymbol* useLocalEndCount = gTrue;
  VarSymbol* countRunningTasks = gTrue;

  BlockStmt* onBlock = findStmtWithTag(PRIM_BLOCK_ON, body);
  // For remote coforalls (e..g. coforall indices in iterator do on indices) we
  // just do a remote fork instead of creating a task locally. Do not count
  // running tasks locally, and use network atomic EndCounts if available
  if (onBlock) {
    onBlock->blockInfoGet()->primitive = primitives[PRIM_BLOCK_COFORALL_ON];
    // Note: gNil is here so error handling can be added by compiler
    // in parallel pass.
    onBlock->insertAtTail(new CallExpr("_downEndCount", coforallCount, gNil));
    addByrefVars(onBlock, byref_vars);
    taskBlk->blockTag = BLOCK_SCOPELESS;
    useLocalEndCount = gFalse;
    countRunningTasks = gFalse;
  } else {
    taskBlk->blockInfoSet(new CallExpr(PRIM_BLOCK_COFORALL));
    taskBlk->insertAtTail(new CallExpr("_downEndCount", coforallCount, gNil));
    addByrefVars(taskBlk, byref_vars);
  }

  BlockStmt* block = ForLoop::buildCoforallLoop(indices,
                                                new SymExpr(iterator),
                                                taskBlk,
                                                zippered);
  if (bounded) {
    if (!onBlock) { block->insertAtHead(new CallExpr("chpl_resetTaskSpawn", numTasks)); }
    block->insertAtHead(new CallExpr("_upEndCount", coforallCount, countRunningTasks, numTasks));
    block->insertAtHead(new CallExpr(PRIM_MOVE, numTasks, new CallExpr(".", iterator,  new_CStringSymbol("size"))));
    block->insertAtHead(new DefExpr(numTasks));
    block->insertAtTail(new DeferStmt(new CallExpr("_endCountFree", coforallCount)));
    block->insertAtTail(new CallExpr("_waitEndCount", coforallCount, countRunningTasks, numTasks));
  } else {
    taskBlk->insertBefore(new CallExpr("_upEndCount", coforallCount, countRunningTasks));
    block->insertAtTail(new DeferStmt(new CallExpr("_endCountFree", coforallCount)));
    block->insertAtTail(new CallExpr("_waitEndCount", coforallCount, countRunningTasks));
  }

  block->insertAtHead(new CallExpr(PRIM_MOVE, coforallCount, new CallExpr("_endCountAlloc", useLocalEndCount)));
  block->insertAtHead(new DefExpr(coforallCount));
  return block;
}


// Remove an extra level of BlockStmt to simplify pattern matching later
// in compilation. Ex. test/parallel/taskPar/taskIntents/ri-coforall+on.chpl
static void removeWrappingBlock(BlockStmt*& block) {
  if (block->length() == 1)
    if (block->isRealBlockStmt())
      if (BlockStmt* nested = toBlockStmt(block->body.head))
        block = (BlockStmt*)nested->remove();
}

// Build up AST for coforalls. For something like:
//
//     coforall indices in iterator with (byref_vars) { body(); }
//
// This effectively builds up:
//
//     var tmpIter = iterator;
//     param bounded = isBoundedRange(tmpIter) || isDomain(tmpIter) || isArray(tmpIter);
//     param useLocalEndCount, countRunningTasks = !bodyContainsOnStmt();
//     if bounded {
//       var numTasks = tmpIter.size;
//       var _coforallCount = _endCountAlloc(useLocalEndCount);
//       // only bump EndCount once, instead of once per task
//       _upEndCount(_coforallCount, countRunningTasks, numTasks);
//       for indices in tmpIter {
//         /* PRIM_BLOCK_COFORALL (byref_vars) */ {
//           body();
//           _downEndCount(_coforallCount, nil);
//         }
//       }
//       _waitEndCount(_coforallCount, countRunningTasks, numTasks);
//       _endCountFree(_coforallCount);
//     } else {
//       var _coforallCount = _endCountAlloc(useLocalEndCount);
//       for indices in tmpIter {
//         _upEndCount(_coforallCount, countRunningTasks);
//         /* PRIM_BLOCK_COFORALL (byref_vars) */ {
//           body();
//           _downEndCount(_coforallCount, nil);
//         }
//       }
//       _waitEndCount(_coforallCount, countRunningTasks);
//       _endCountFree(_coforallCount);
//     }
//
// For coforall+ons:
//
//     coforall indices in iterator do on indices{ body(); }
//
// there are some minor differences. We use network atomics for the EndCount if
// they're available, we won't manipulate here.runningTaskCount, and we'll use
// PRIM_BLOCK_COFORALL_ON instead of PRIM_BLOCK_COFORALL so that we just do
// remote-forks instead of creating any tasks locally.
BlockStmt* buildCoforallLoopStmt(Expr* indices,
                                 Expr* iterator,
                                 CallExpr* byref_vars,
                                 BlockStmt* body,
                                 bool zippered)
{
  removeWrappingBlock(body); // may update 'body'
  checkControlFlow(body, "coforall statement");

  // insert temporary index when elided by user
  if (!indices)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");
  checkIndices(indices);
  if (zippered) zipToTuple(iterator);

  SET_LINENO(body);

  VarSymbol* tmpIter = newTemp("tmpIter");
  tmpIter->addFlag(FLAG_EXPR_TEMP);
  tmpIter->addFlag(FLAG_MAYBE_REF);
  tmpIter->addFlag(FLAG_NO_COPY);

  BlockStmt* coforallBlk = new BlockStmt();
  coforallBlk->insertAtTail(new DefExpr(tmpIter));
  coforallBlk->insertAtTail(new CallExpr(PRIM_MOVE, tmpIter, iterator));

  BlockStmt* vectorCoforallBlk = buildLoweredCoforall(indices, tmpIter, copyByrefVars(byref_vars), body->copy(), zippered, /*bounded=*/true);
  BlockStmt* nonVectorCoforallBlk = buildLoweredCoforall(indices, tmpIter, byref_vars, body, zippered, /*bounded=*/false);

  VarSymbol* isRngDomArr = newTemp("isRngDomArr");
  isRngDomArr->addFlag(FLAG_MAYBE_PARAM);
  coforallBlk->insertAtTail(new DefExpr(isRngDomArr));

  coforallBlk->insertAtTail(new CallExpr(PRIM_MOVE, isRngDomArr,
                            new CallExpr("||", new CallExpr("isBoundedRange", tmpIter),
                            new CallExpr("||", new CallExpr("isDomain", tmpIter), new CallExpr("isArray", tmpIter)))));

  coforallBlk->insertAtTail(new CondStmt(new SymExpr(isRngDomArr),
                                         vectorCoforallBlk,
                                         nonVectorCoforallBlk));
  return coforallBlk;
}


BlockStmt* buildParamForLoopStmt(const char* index, Expr* range, BlockStmt* stmts) {
  VarSymbol* indexVar = new VarSymbol(index);

  return ParamForLoop::buildParamForLoop(indexVar, range, stmts);
}

BlockStmt*
buildAssignment(Expr* lhs, Expr* rhs, const char* op) {
  INT_ASSERT(op != NULL);
  return buildChapelStmt(new CallExpr(op, lhs, rhs));
}

BlockStmt*
buildAssignment(Expr* lhs, Expr* rhs, PrimitiveTag op) {
  return buildChapelStmt(new CallExpr(op, lhs, rhs));
}

BlockStmt* buildLAndAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = new BlockStmt();
  VarSymbol* ltmp = newTemp();

  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIM_MOVE, ltmp, new CallExpr(PRIM_ADDR_OF, lhs)));
  stmt->insertAtTail(new CallExpr("=",       ltmp, new CallExpr("&&", ltmp, rhs)));

  return stmt;
}


BlockStmt* buildLOrAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = new BlockStmt();
  VarSymbol* ltmp = newTemp();

  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIM_MOVE, ltmp, new CallExpr(PRIM_ADDR_OF, lhs)));
  stmt->insertAtTail(new CallExpr("=",       ltmp, new CallExpr("||", ltmp, rhs)));

  return stmt;
}


BlockStmt* buildSelectStmt(Expr* selectCond, BlockStmt* whenstmts) {
  BlockStmt* block = new BlockStmt();
  CondStmt* otherwise = NULL;
  CondStmt* top = NULL;
  CondStmt* condStmt = NULL;

  VarSymbol* tmp = newTemp();

  tmp->addFlag(FLAG_MAYBE_PARAM);
  tmp->addFlag(FLAG_MAYBE_TYPE);
  tmp->addFlag(FLAG_EXPR_TEMP);

  block->insertAtTail(new DefExpr(tmp));
  block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, selectCond));

  for_alist(stmt, whenstmts->body) {
    CondStmt* when = toCondStmt(stmt);
    if (!when)
      INT_FATAL("error in buildSelectStmt");
    CallExpr* conds = toCallExpr(when->condExpr);
    if (!conds || !conds->isPrimitive(PRIM_WHEN))
      INT_FATAL("error in buildSelectStmt");
    if (conds->numActuals() == 0) {
      if (otherwise)
        USR_FATAL(selectCond, "Select has multiple otherwise clauses");
      otherwise = when;
    } else {
      Expr* expr = NULL;
      for_actuals(whenCond, conds) {
        whenCond->remove();
        if (!expr)
          expr = new CallExpr("==", tmp, whenCond);
        else
          expr = new CallExpr("||", expr, new CallExpr("==",
                                                   tmp,
                                                   whenCond));
      }
      if (!condStmt) {
        condStmt = new CondStmt(new CallExpr("_cond_test", expr), when->thenStmt);
        top = condStmt;
      } else {
        CondStmt* next = new CondStmt(new CallExpr("_cond_test", expr), when->thenStmt);
        condStmt->elseStmt = new BlockStmt(next);
        condStmt = next;
      }
    }
  }
  // TODO: Is it OK to just have an 'otherwise' ?
  if (!condStmt) {
    USR_FATAL(selectCond, "Select has no when clauses");
  }
  if (otherwise) {
    condStmt->elseStmt = otherwise->thenStmt;
  }

  block->insertAtTail(top);
  return block;
}


static void
buildReduceScanPreface1(FnSymbol* fn, Symbol* data, Symbol* eltType,
                       Expr* opExpr, Expr* dataExpr, bool zippered=false) {
  adjustMinMaxReduceOp(opExpr);
  eltType->addFlag(FLAG_MAYBE_TYPE);
  fn->insertAtTail(new DefExpr(eltType));

  if( !zippered ) {
    fn->insertAtTail("{TYPE 'move'(%S, 'typeof'(chpl__initCopy(iteratorIndex(_getIterator(%S)))))}", eltType, data);
  } else {
    fn->insertAtTail("{TYPE 'move'(%S, 'typeof'(chpl__initCopy(iteratorIndex(_getIteratorZip(%S)))))}", eltType, data);
  }
}

static void
buildReduceScanPreface2(BlockStmt* fn, Symbol* eltType, Symbol* globalOp,
                        Expr* opExpr)
{
  fn->insertAtTail(new DefExpr(globalOp));

  NamedExpr* newArg = new NamedExpr("eltType", new SymExpr(eltType));
  CallExpr* newCall = makeUnmanagedNew(opExpr, newArg);
  CallExpr* move = new CallExpr(PRIM_MOVE, globalOp, newCall);
  fn->insertAtTail(move);
}

static void
buildReduceScanPreface2(FnSymbol* fn, Symbol* eltType, Symbol* globalOp,
                        Expr* opExpr)
{
  buildReduceScanPreface2(fn->body, eltType, globalOp, opExpr);
}

CallExpr* buildReduceExpr(Expr* opExpr, Expr* dataExpr, bool zippered) {
// vass todo: since this holds, no need to pass zippered to PRIM_REDUCE below.
  INT_ASSERT(zippered == (isCallExpr(dataExpr) &&
                          toCallExpr(dataExpr)->isPrimitive(PRIM_ZIP)));

  adjustMinMaxReduceOp(opExpr);
  return new CallExpr(PRIM_REDUCE, opExpr, dataExpr,
                      zippered ? gTrue : gFalse);
}


CallExpr* buildScanExpr(Expr* opExpr, Expr* dataExpr, bool zippered) {
  static int uid = 1;

  FnSymbol* fn = new FnSymbol(astr("chpl__scan", istr(uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  fn->addFlag(FLAG_FN_RETURNS_ITERATOR);

  // data will hold the reduce-d expression as an argument
  // we'll store dataExpr in the call to the chpl__scan function.
  ArgSymbol* data = new ArgSymbol(INTENT_BLANK, "chpl_toScan", dtAny);
  fn->insertFormalAtTail(data);

  if (zippered) zipToTuple(dataExpr);
  VarSymbol* eltType = newTemp("chpl_eltType");
  VarSymbol* globalOp = newTempConst();

  buildReduceScanPreface1(fn, data, eltType, opExpr, dataExpr, zippered);
  buildReduceScanPreface2(fn, eltType, globalOp, opExpr);

  if( !zippered ) {
    fn->insertAtTail("'return'(chpl__scanIterator(%S, %S))", globalOp, data);
  } else {
    fn->insertAtTail("'return'(chpl__scanIteratorZip(%S, %S))", globalOp, data);
  }

  return new CallExpr(new DefExpr(fn), dataExpr);
}


static void
backPropagateInitsTypes(BlockStmt* stmts) {
  Expr* init = NULL;
  Expr* type = NULL;
  DefExpr* prev = NULL;
  for_alist_backward(stmt, stmts->body) {
    if (DefExpr* def = toDefExpr(stmt)) {
      if (def->init || def->exprType) {
        init = def->init;
        type = def->exprType;
      } else {
        if (type)
          prev->exprType =
            new CallExpr(PRIM_TYPEOF, new UnresolvedSymExpr(def->sym->name));
        if (init) {
          if (init->isNoInitExpr()) {
            prev->init = init->copy();
          } else if (type) {
            prev->init = new CallExpr("chpl__readXX",
                                      new UnresolvedSymExpr(def->sym->name));
          } else
            prev->init = new UnresolvedSymExpr(def->sym->name);
        }
        def->init = init;
        def->exprType = type;
      }
      prev = def;
    } else
      INT_FATAL(stmt, "expected DefExpr in backPropagateInitsTypes");
  }
}


std::set<Flag>* buildVarDeclFlags(Flag flag1, Flag flag2) {
  // this will be deleted in buildVarDecls()
  std::set<Flag>* flags = new std::set<Flag>();

  if (flag1 != FLAG_UNKNOWN) {
    flags->insert(flag1);
  }
  if (flag2 != FLAG_UNKNOWN) {
    flags->insert(flag2);
  }

  return flags;
}


// look up cfgname and mark it as used if we find it
static Expr* lookupConfigValHelp(const char* cfgname, VarSymbol* var) {
  Expr* configInit = NULL;
  configInit = getCmdLineConfig(cfgname);
  if (configInit) {
    if (VarSymbol* conflictingVar = isUsedCmdLineConfig(cfgname)) {
      USR_FATAL_CONT(var, "ambiguous config name (%s)", cfgname);
      USR_PRINT(conflictingVar, "also defined here");
      USR_PRINT(conflictingVar, "(disambiguate using -s<modulename>.%s...)", cfgname);
    } else {
      useCmdLineConfig(cfgname, var);
    }
  }
  return configInit;
}

// first try looking up cfgname;
// if it fails, try looking up currentModuleName.cfgname
static Expr* lookupConfigVal(VarSymbol* var) {
  extern bool parsingPrivate;
  const char* cfgname = var->name;
  Expr* configInit = lookupConfigValHelp(astr(currentModuleName, ".", cfgname), var);
  // only public configs can be matched in an unqualified manner
  if (!parsingPrivate) {
    if (Expr* unqualConfigInit = lookupConfigValHelp(astr(cfgname), var)) {
      if (configInit == NULL) {
        configInit = unqualConfigInit;
      } else {
        // we may want to have the latter flag "win", but that would require
        // storing ordering information about the order in which the flags
        // were passed, which we don't currently maintain.  A job for a rainy
        // day?
        USR_FATAL_CONT(var, "config set ambiguously via '-s%s' and '-s%s.%s'",
                       cfgname, currentModuleName, cfgname);
      }
    }
  }
  return configInit;
}

// take care of any config param, const, vars, overriding the expression
// in the source code with what was provided on the command-line
static void handleConfigVals(VarSymbol* var, DefExpr* defExpr, Expr* stmt) {
  if (Expr *configInit = lookupConfigVal(var)) {
    // config var initialized on the command line
    // drop the original init expression on the floor
    if (Expr* a = toExpr(configInit))
      defExpr->init = a;
    else if (Symbol* a = toSymbol(configInit))
      defExpr->init = new SymExpr(a);
    else
      INT_FATAL(stmt, "DefExpr initialized with bad exprType config ast");
  }
}


//
// This helper function will return the string literal that a
// cnameExpr evaluates to if it is one; otherwise, the expression
// should be resolved at param resolution time.
//
static const char* cnameExprToString(Expr* cnameExpr) {
  if (SymExpr* se = toSymExpr(cnameExpr))
    if (VarSymbol* v = toVarSymbol(se->symbol()))
      if (v->isImmediate())
        if (v->immediate->const_kind == CONST_KIND_STRING)
          return v->immediate->v_string;
  return NULL;
}

BlockStmt* buildVarDecls(BlockStmt* stmts, const char* docs,
                         std::set<Flag>* flags, Expr* cnameExpr) {
  bool firstvar = true;
  const char* cname = NULL;

  if (cnameExpr != NULL) {
    cname = cnameExprToString(cnameExpr);
    if (cname == NULL) {
      USR_FATAL_CONT(cnameExpr, "at present, external variables can only be renamed using string literals");
    }
  }

  for_alist(stmt, stmts->body) {
    if (DefExpr* defExpr = toDefExpr(stmt)) {
      if (VarSymbol* var = toVarSymbol(defExpr->sym)) {
        // Store the user-provided cname, if there was one
        if (cname)
          var->cname = cname;

        // Attach any flags provided to the variable
        if (flags) {
          if (flags->count(FLAG_EXTERN) && flags->count(FLAG_PARAM))
            USR_FATAL(var, "external params are not supported");

          if (cnameExpr != NULL && !firstvar)
            USR_FATAL_CONT(var, "external symbol renaming can only be applied to one symbol at a time");

          for (std::set<Flag>::iterator it = flags->begin(); it != flags->end(); ++it) {
            var->addFlag(*it);
          }
        }

        if (var->hasFlag(FLAG_CONFIG)) {
          handleConfigVals(var, defExpr, stmt);
        }

        var->doc = docs;
        firstvar = false;
        continue;
      }
    }
    INT_FATAL(stmt, "Major error in setVarSymbolAttributes");
  }
  backPropagateInitsTypes(stmts);
  //
  // If blockInfo is set, this is a tuple variable declaration.
  // Add checks that the expression on the right is a tuple and that
  // the tuple size matches the number of variables. If not, issue
  // compilerErrors. blockInfo has the form:
  // call("_check_tuple_var_decl", rhsTuple, numVars)
  //
  if (stmts->blockInfoGet()) {
    INT_ASSERT(stmts->blockInfoGet()->isNamed("_check_tuple_var_decl"));
    CallExpr* checkCall = stmts->blockInfoGet();
    SymExpr* tuple = toSymExpr(checkCall->get(1));
    tuple->symbol()->defPoint->insertAfter(checkCall);
    stmts->blockInfoSet(NULL);
  }

  // Add a PRIM_END_OF_STATEMENT.
  if (fDocs == false) {
    CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
    stmts->insertAtTail(end);
  }

  // this was allocated in buildVarDeclFlags()
  if (flags)
    delete flags;

  return stmts;
}

static
AggregateType* installInternalType(AggregateType* ct, AggregateType* dt) {
  // Hook the string type in the modules
  // to avoid duplication with dtString created in initPrimitiveTypes().
  // gatherWellKnownTypes runs too late to help.

  // grab the existing symbol from the placeholder "dtString"
  ct->addSymbol(dt->symbol);
  *dt = *ct;

  // These fields get overwritten with `ct` by the assignment.
  // These fields are set to `this` by the AggregateType constructor
  // so they should still be `dtString`. Fix them back up.
  dt->fields.parent   = dt;
  dt->inherits.parent = dt;

  gAggregateTypes.remove(gAggregateTypes.index(ct));

  delete ct;

  return dt;
}

DefExpr* buildClassDefExpr(const char*  name,
                           const char*  cname,
                           AggregateTag tag,
                           Expr*        inherit,
                           BlockStmt*   decls,
                           Flag         externFlag,
                           const char*  docs) {
  bool isExtern = externFlag == FLAG_EXTERN;
  AggregateType* ct = NULL;
  TypeSymbol* ts = NULL;

  ct = new AggregateType(tag);

  // Hook the string type in the modules
  // to avoid duplication with dtString created in initPrimitiveTypes().
  // gatherWellKnownTypes runs too late to help.
  if (strcmp("_string", name) == 0) {
    ct = installInternalType(ct, dtString);
    ts = ct->symbol;
  } else if (strcmp("_bytes", name) == 0) {
    ct = installInternalType(ct, dtBytes);
    ts = ct->symbol;
  } else if (strcmp("_locale", name) == 0) {
    ct = installInternalType(ct, dtLocale);
    ts = ct->symbol;
  } else if (strcmp("_object", name) == 0) {
    ct = installInternalType(ct, dtObject);
    ts = ct->symbol;
  } else if (strcmp("_owned", name) == 0) {
    ct = installInternalType(ct, dtOwned);
    ts = ct->symbol;
  } else if (strcmp("_shared", name) == 0) {
    ct = installInternalType(ct, dtShared);
    ts = ct->symbol;
  } else {
    ts = new TypeSymbol(name, ct);
  }

  INT_ASSERT(ct);

  DefExpr*    def = new DefExpr(ts);

  // add FLAG_EXTERN if this is extern before adding declarations to
  // the class in order to be able to flag the case of declaring an
  // extern field in a non-extern class
  if (isExtern) {
    if (cname) {
      ts->cname = astr(cname);
    }

    ts->addFlag(FLAG_EXTERN);
    ts->addFlag(FLAG_NO_OBJECT);
    ct->defaultValue=NULL;

    if (inherit != NULL)
      USR_FATAL_CONT(inherit,
                     "External types do not currently support inheritance");
  }

  ct->addDeclarations(decls);

  if (inherit != NULL) {
    ct->inherits.insertAtTail(inherit);
  }

  ct->doc = docs;

  return def;
}


//
// If an argument has intent 'INTENT_TYPE', this function sets up the
// ArgSymbol the way downstream passes expect it to be.
//
void setupTypeIntentArg(ArgSymbol* arg) {
  arg->intent = INTENT_BLANK;
  arg->addFlag(FLAG_TYPE_VARIABLE);
  if (arg->typeExpr == NULL)
    arg->type = dtAny;
}


DefExpr*
buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable) {
  ArgSymbol* arg = new ArgSymbol(tag, ident, dtUnknown, type, init, variable);
  if (arg->intent == INTENT_TYPE) {
    setupTypeIntentArg(arg);
  } else if (!type && !init)
    arg->type = dtAny;
  return new DefExpr(arg);
}


//
// create a single argument and store the tuple-grouped args in the
// variable argument list; these will be moved out of the variable
// argument list in the call to destructureTupleGroupedArgs when the
// formals are added to the formals list in the function (in
// buildFunctionFormal)
//
DefExpr*
buildTupleArgDefExpr(IntentTag tag, BlockStmt* tuple, Expr* type, Expr* init) {
  ArgSymbol* arg = new ArgSymbol(tag, "chpl__tuple_arg_temp", dtUnknown,
                                 type, init, tuple);
  arg->addFlag(FLAG_TEMP);
  if (arg->intent != INTENT_BLANK)
    USR_FATAL(tuple, "intents on tuple-grouped arguments are not yet supported");
  if (!type)
    arg->type = dtTuple;
  return new DefExpr(arg);
}


//
// Destructure tuple function arguments.  Add to the function's where
// clause to match the shape of the tuple being destructured.
//
static void
destructureTupleGroupedArgs(FnSymbol* fn, BlockStmt* tuple, Expr* base) {
  int i = 0;
  for_alist(expr, tuple->body) {
    i++;
    if (DefExpr* def = toDefExpr(expr)) {
      def->init = new CallExpr(base->copy(), new_IntSymbol(i));
      if (!strcmp(def->sym->name, "chpl__tuple_blank")) {
        def->remove();
      } else {
        fn->insertAtHead(def->remove());
      }
    } else if (BlockStmt* inner = toBlockStmt(expr)) {
      destructureTupleGroupedArgs(fn, inner, new CallExpr(base->copy(), new_IntSymbol(i)));
    }
  }

  Expr* where =
    new CallExpr("&&",
      new CallExpr("isTuple", base->copy()),
      new CallExpr("==", new_IntSymbol(i),
        new CallExpr(".", base->copy(), new_CStringSymbol("size"))));

  if (fn->where) {
    where = new CallExpr("&&", fn->where->body.head->remove(), where);
    fn->where->body.insertAtHead(where);
  } else {
    fn->where = new BlockStmt(where);
  }
}

FnSymbol* buildLambda(FnSymbol *fn) {
  static unsigned int nextId = 0;
  char buffer[100];

  /*
   * The snprintf function is used to prevent a buffer overflow from occurring.
   * Technically, an overflow can only occur if Chapel is compiled on a machine
   * where an unsigned integer can represent numbers larger than 10^86, but it
   * is better to guard against this behavior then leaving someone wondering
   * why we didn't.
   */
  if (snprintf(buffer, 100, "chpl_lambda_%i", nextId++) >= 100) {
    INT_FATAL("Too many lambdas.");
  }

  if (!fn) {
    fn = new FnSymbol(astr(buffer));
  } else {
    fn->name = astr(buffer);
    fn->cname = fn->name;
  }

  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  return fn;
}

BlockStmt* buildExternExportFunctionDecl(Flag externOrExport, Expr* paramCNameExpr, BlockStmt* blockFnDef) {
  DefExpr* def = toDefExpr(blockFnDef->body.tail);
  INT_ASSERT(def);
  FnSymbol* fn = toFnSymbol(def->sym);
  INT_ASSERT(fn);

  const char* cname = "";
  // Look for a string literal we can use
  if (paramCNameExpr != NULL) {
    const char* cnameStr = cnameExprToString(paramCNameExpr);
    if (cnameStr) {
      cname = cnameStr;
    }
  }

  if (externOrExport == FLAG_EXTERN) {
    fn->addFlag(FLAG_LOCAL_ARGS);
    fn->addFlag(FLAG_EXTERN);

    // extern functions with no declared return type will return void
    if (fn->retExprType == NULL)
      fn->retType = dtVoid;
  }
  if (externOrExport == FLAG_EXPORT) {
    fn->addFlag(FLAG_LOCAL_ARGS);
    fn->addFlag(FLAG_EXPORT);
  }
  if (fn->isIterator())
  {
    USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
  }

  // Handle non-trivial param names that need to be resolved,
  // but don't do this under chpldoc
  if (cname[0] != '\0') {
    // The user explicitly named this function (controls mangling).
    fn->cname = cname;
  } else if (paramCNameExpr && cname[0] == '\0' && fDocs == false) {
    // cname should be set based upon param
    DefExpr* argDef = buildArgDefExpr(INTENT_BLANK,
                                      astr_chpl_cname,
                                      new SymExpr(dtString->symbol),
                                      paramCNameExpr, NULL);
    fn->insertFormalAtTail(argDef);
  }

  return blockFnDef;
}

// Replaces the dummy function name "_" with the real name, sets the 'this'
// intent tag. For methods, it also adds a method tag and "this" declaration.
// receiver is typically an UnresolvedSymExpr("class_name") in order
// to declare a method outside of a record/class.
FnSymbol*
buildFunctionSymbol(FnSymbol*   fn,
                    const char* name,
                    IntentTag   thisTag,
                    Expr*       receiver)
{
  fn->cname   = fn->name = astr(name);
  fn->thisTag = thisTag;

  if ((fn->name[0] == '~' && fn->name[1] != '\0') ||
      (fn->name == astrDeinit))
    fn->addFlag(FLAG_DESTRUCTOR);

  if (receiver)
  {
    ArgSymbol* arg = new ArgSymbol(thisTag,
                                   "this",
                                   dtUnknown,
                                   receiver);
    fn->_this = arg;
    if (thisTag == INTENT_TYPE) {
      setupTypeIntentArg(arg);
    }

    arg->addFlag(FLAG_ARG_THIS);
    fn->insertFormalAtHead(new DefExpr(arg));

    ArgSymbol* mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);

    fn->setMethod(true);
    fn->insertFormalAtHead(new DefExpr(mt));
  }

  return fn;
}

BlockStmt*
buildFunctionDecl(FnSymbol*   fn,
                  RetTag      optRetTag,
                  Expr*       optRetType,
                  bool        optThrowsError,
                  Expr*       optWhere,
                  Expr*       optLifetimeConstraints,
                  BlockStmt*  optFnBody,
                  const char* docs)
{
  fn->retTag = optRetTag;

  if (optRetType)
    fn->retExprType = new BlockStmt(optRetType, BLOCK_TYPE);

  if (optThrowsError)
  {
    fn->throwsErrorInit();
  }

  if (optWhere)
  {
    fn->where = new BlockStmt(optWhere);
  }

  if (optLifetimeConstraints)
  {
    fn->lifetimeConstraints = new BlockStmt(optLifetimeConstraints);
  }

  if (optFnBody) {
    if (fn->hasFlag(FLAG_EXTERN))
      USR_FATAL_CONT(fn, "Extern functions cannot have a body.");

    if (fn->body->length() == 0) {
      // Copy the statements from optFnBody to the function's
      // body to preserve line numbers
      for_alist(expr, optFnBody->body) {
        fn->body->insertAtTail(expr->remove());
      }
    } else {
      fn->insertAtTail(optFnBody);
    }
  } else {
    fn->addFlag(FLAG_NO_FN_BODY);
  }

  fn->doc = docs;

  return buildChapelStmt(new DefExpr(fn));
}

void applyPrivateToBlock(BlockStmt* block) {
  for_alist(stmt, block->body) {
    if (DefExpr* defExpr = toDefExpr(stmt)) {
      defExpr->sym->addFlag(FLAG_PRIVATE);
    }
  }
}

static
DefExpr* buildForwardingExprFnDef(Expr* expr) {
  // Put expr into a method and return the DefExpr for that method.
  // This way, we can work with the rest of the compiler that
  // assumes that 'this' is an ArgSymbol.
  static int delegate_counter = 0;
  const char* name = astr("chpl_forwarding_expr", istr(++delegate_counter));
  if (UnresolvedSymExpr* usex = toUnresolvedSymExpr(expr))
    name = astr(name, "_", usex->unresolved);
  FnSymbol* fn = new FnSymbol(name);

  fn->addFlag(FLAG_INLINE);
  fn->addFlag(FLAG_MAYBE_REF);
  fn->addFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS);
  fn->addFlag(FLAG_COMPILER_GENERATED);

  fn->body->insertAtTail(new CallExpr(PRIM_RETURN, expr));

  DefExpr* def = new DefExpr(fn);

  return def;
}


// handle syntax like
//    var instance:someType;
//    forwarding instance;
BlockStmt* buildForwardingStmt(Expr* expr) {
  return buildChapelStmt(new ForwardingStmt(buildForwardingExprFnDef(expr)));
}

// handle syntax like
//    var instance:someType;
//    forwarding instance only foo;
BlockStmt* buildForwardingStmt(Expr* expr, std::vector<PotentialRename*>* names, bool except) {
  std::set<const char*> namesSet;
  std::map<const char*, const char*> renameMap;

  // Catch the 'except *' case and turn it into 'only <nothing>'.  This
  // case will have a single UnresolvedSymExpr named "".
  if (except && names->size() == 1) {
    PotentialRename* listElem = (*names)[0];
    if (UnresolvedSymExpr* name = toUnresolvedSymExpr(listElem->elem)) {
      if (name->unresolved[0] == '\0') {
        except = false;
      }
    }
  }

  // Iterate through the list of names to exclude when using mod
  for_vector(PotentialRename, listElem, *names) {
    switch (listElem->tag) {
      case PotentialRename::SINGLE:
        if (UnresolvedSymExpr* name = toUnresolvedSymExpr(listElem->elem)) {
          namesSet.insert(name->unresolved);
        } else {
          // Currently we expect only unresolved sym exprs
          useListError(listElem->elem, except);
        }
        break;
      case PotentialRename::DOUBLE:
        std::pair<Expr*, Expr*>* elem = listElem->renamed;
        // Need to check that we aren't renaming in an 'except' list
        if (except) {
          USR_FATAL(elem->first, "cannot rename in 'except' list");
        }
        UnresolvedSymExpr* old_name = toUnresolvedSymExpr(elem->first);
        UnresolvedSymExpr* new_name = toUnresolvedSymExpr(elem->second);
        if (old_name != NULL && new_name != NULL) {
          renameMap[new_name->unresolved] = old_name->unresolved;
        } else {
          useListError(elem->first, except);
        }
        break;
    }

  }

  DefExpr* fnDef = buildForwardingExprFnDef(expr);
  ForwardingStmt* ret = new ForwardingStmt(fnDef,
                                       &namesSet,
                                       except,
                                       &renameMap);
  return buildChapelStmt(ret);
}


// handle syntax like
//    forwarding var instance:someType;
// by translating it into
//    var instance:someType;
//    forwarding instance;
BlockStmt* buildForwardingDeclStmt(BlockStmt* stmts) {
  for_alist(stmt, stmts->body) {
    if (DefExpr* defExpr = toDefExpr(stmt)) {
      if (VarSymbol* var = toVarSymbol(defExpr->sym)) {
        // Append a ForwardingStmt
        BlockStmt* toAppend = buildForwardingStmt(new UnresolvedSymExpr(var->name));

        // Remove the END_OF_STATEMENT marker, not used for fields
        Expr* last = stmts->body.last();
        if (last && isEndOfStatementMarker(stmts->body.last()))
          last->remove();

        for_alist(tmp, toAppend->body) {
          stmts->insertAtTail(tmp->remove());
        }
      } else {
        INT_FATAL("case not handled in buildForwardingDeclStmt");
      }
    }
  }
  return stmts;
}

FnSymbol*
buildFunctionFormal(FnSymbol* fn, DefExpr* def) {
  if (!fn)
    fn = new FnSymbol("_");
  if (!def)
    return fn;
  ArgSymbol* arg = toArgSymbol(def->sym);
  INT_ASSERT(arg);

  int countFormals = fn->numFormals();

  fn->insertFormalAtTail(def);
  if (!strcmp(arg->name, "chpl__tuple_arg_temp")) {
    destructureTupleGroupedArgs(fn, arg->variableExpr, new SymExpr(arg));
    arg->variableExpr = NULL;
    // append countFormals to the argument name so it is unique
    arg->name = astr(arg->name, istr(countFormals));
  }
  return fn;
}

// builds a local statement with a conditional, where the `then` block
// is local and `else` block is not
BlockStmt* buildLocalStmt(Expr* condExpr, Expr *stmt) {
  return buildIfStmt(new CallExpr("_cond_test", condExpr),
      buildLocalStmt(stmt->copy()), stmt);
}

// builds an unconditional local statement. Used by the conditional
// overload and parser
BlockStmt* buildLocalStmt(Expr* stmt) {
  BlockStmt* block = buildChapelStmt();

  if (!requireWideReferences()) {
    block->insertAtTail(stmt);
    return block;
  }

  BlockStmt* body = toBlockStmt(stmt);

  //
  // detect on-statement directly inside local statement
  // i.e., we want "local on {} ", not "local { on {} }"
  //
  BlockStmt* onBlock = toBlockStmt(body->body.tail);
  if (body->blockTag == BLOCK_SCOPELESS &&
      onBlock != NULL &&
      onBlock->isBlockType(PRIM_BLOCK_ON)) {
    // On-statement directly inside scopeless local block

    CallExpr* call = toCallExpr(onBlock->blockInfoGet());
    SymExpr* head = toSymExpr(call->argList.head);
    if (head->symbol() == gTrue) {
      // avoiding 'local local on'
      onBlock = NULL;
    }
  } else {
    // not an on-block
    onBlock = NULL;
  }
  if (onBlock) {
    CallExpr* call = toCallExpr(onBlock->blockInfoGet());

    // first argument of a primitive on is a param bool that distinguishes
    // between a local-on and a regular on-statement.
    call->argList.head->replace(new SymExpr(gTrue));
    return body;
  } else {
    BlockStmt* localBlock = new BlockStmt(stmt);
    localBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_LOCAL));
    block->insertAtTail(localBlock);
    return block;
  }
}


static Expr* extractLocaleID(Expr* expr) {
  // If the on <x> expression is a primitive_on_locale_num, we just
  // return the primitive.
  // PRIM_ON_LOCAL_NUM is now passed through to codegen,
  // but we don't want to wrap it in PRIM_WIDE_GET_LOCALE.
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_ON_LOCALE_NUM)) {
      // Can probably use some semantic checks, like the number of args being 1 or 2, etc.
      return expr;
    }
  }

  // Otherwise, we need to wrap the expression in a primitive to query
  // the locale ID of the expression
  // TODO: Review all clients of this routine and see whether they expect the whole
  // locale ID or just the node ID.  Split this routine into extractLocaleID()
  // and extractNodeID().
  // The current implementation expects localeID == nodeID, so we return
  // just the node portion of the localeID (so this is really extractNodeID()).
  return new CallExpr(PRIM_WIDE_GET_LOCALE, expr);
}


BlockStmt*
buildOnStmt(Expr* expr, Expr* stmt) {
  checkControlFlow(stmt, "on statement");

  CallExpr* onExpr = new CallExpr(PRIM_DEREF, extractLocaleID(expr));

  BlockStmt* body = toBlockStmt(stmt);

  //
  // detect begin statement directly inside on-statement
  //
  BlockStmt* beginBlock = findStmtWithTag(PRIM_BLOCK_BEGIN, body);

  // If the locale model doesn't require outlined on functions and this is a
  // --local compile, then we take the on-expression, execute it to evaluate
  // it for side effects, and then evaluate the body directly.
  if (!requireOutlinedOn()) {
    BlockStmt* block = new BlockStmt(stmt);
    Symbol* tmp = newTempConst();
    block->insertAtHead(new CallExpr(PRIM_MOVE, tmp, onExpr)); // evaluate the expression for side effects
    block->insertAtHead(new DefExpr(tmp));
    block->blockInfoSet(new CallExpr(PRIM_BLOCK_ELIDED_ON, gFalse, tmp));
    return buildChapelStmt(block);
  }

  if (beginBlock) {
    // OPTIMIZATION: If "on x" is immediately followed by a "begin", then collapse
    // remote_fork (node) {
    //   branch /*local*/ { foo(); }
    // } wait;
    // to
    // remote_fork (node) { foo(); } // no wait();

    // Execute the construct "on x begin ..." asynchronously.
    Symbol* tmp = newTempConst();
    body->insertAtHead(new CallExpr(PRIM_MOVE, tmp, onExpr));
    body->insertAtHead(new DefExpr(tmp));
    beginBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_BEGIN_ON, gFalse, tmp));
    // If there are beginBlock->byrefVars, they will be preserved.
    return body;
  } else {
    // Otherwise, wait for the "on" statement to complete before proceeding.
    BlockStmt* block = buildChapelStmt();
    Symbol* tmp = newTempConst();
    block->insertAtTail(new DefExpr(tmp));
    block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, onExpr));
    BlockStmt* onBlock = new BlockStmt(stmt);
    onBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_ON, gFalse, tmp));
    block->insertAtTail(onBlock);
    return block;
  }
}


BlockStmt*
buildBeginStmt(CallExpr* byref_vars, Expr* stmt) {
  checkControlFlow(stmt, "begin statement");

  BlockStmt* body = toBlockStmt(stmt);

  //
  // detect on-statement directly inside begin statement
  //
  BlockStmt* onBlock = findStmtWithTag(PRIM_BLOCK_ON, body);

  // Note: parallel might be looking for the argument to _downEndCount
  if (onBlock) {
    body->insertAtHead(new CallExpr("_upDynamicEndCount", gFalse));
    onBlock->insertAtTail(new CallExpr("_downDynamicEndCount", gNil));
    onBlock->blockInfoGet()->primitive = primitives[PRIM_BLOCK_BEGIN_ON];
    addByrefVars(onBlock, byref_vars);
    return body;
  } else {
    BlockStmt* block = new BlockStmt();
    VarSymbol* endCount = newTempConst("_endCount");
    endCount->addFlag(FLAG_END_COUNT);
    block->insertAtTail(new DefExpr(endCount));
    block->insertAtTail(new CallExpr(PRIM_MOVE, endCount, new CallExpr(PRIM_GET_DYNAMIC_END_COUNT)));
    block->insertAtTail(new CallExpr("_upEndCount", endCount));
    BlockStmt* beginBlock = new BlockStmt();
    beginBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_BEGIN));
    addByrefVars(beginBlock, byref_vars);
    beginBlock->insertAtHead(stmt);
    beginBlock->insertAtTail(new CallExpr("_downEndCount", endCount, gNil));
    block->insertAtTail(beginBlock);
    return block;
  }
}


BlockStmt*
buildSyncStmt(Expr* stmt) {
  checkControlFlow(stmt, "sync statement");
  BlockStmt* block = new BlockStmt();
  VarSymbol* endCountSave = newTempConst("_endCountSave");
  endCountSave->addFlag(FLAG_END_COUNT);
  block->insertAtTail(new DefExpr(endCountSave));
  block->insertAtTail(new CallExpr(PRIM_MOVE, endCountSave, new CallExpr(PRIM_GET_DYNAMIC_END_COUNT)));
  VarSymbol* endCount = newTempConst("_endCount");
  endCount->addFlag(FLAG_END_COUNT);
  block->insertAtTail(new DefExpr(endCount));
  block->insertAtTail(new CallExpr(PRIM_MOVE, endCount, new CallExpr("_endCountAlloc", /* forceLocalTypes= */gFalse)));
  block->insertAtTail(new CallExpr(PRIM_SET_DYNAMIC_END_COUNT, endCount));

  // Note that a sync statement can contain arbitrary code,
  // including code that throws. As a result, we need to take
  // care call chpl_waitDynamicEndCount even if such errors are thrown.

  // This code takes the approach of wrapping the sync body with
  //
  //   try {
  //      orig-body
  //   } catch e {
  //      chpl_save_task_error(e);
  //      e = nil; // don't delete error
  //   }

  // The result is that an error within a sync block will be reported
  // in a TaskErrors group. It is that way because there could also be
  // errors from waited-for tasks.
  BlockStmt* saveError = new BlockStmt();

  const char* ename = "chpl_sync_error";

  saveError->insertAtTail(new CallExpr("chpl_save_task_error_owned",
                                       endCount,
                                       new UnresolvedSymExpr(ename)));

  BlockStmt* catches = new BlockStmt();
  catches->insertAtTail(CatchStmt::build(ename, saveError));

  BlockStmt* body = toBlockStmt(stmt);
  body->blockTag = BLOCK_NORMAL; // or at least, not scopeless
  INT_ASSERT(body);

  TryStmt* t = new TryStmt(/* try! */ false, body, catches,
                           /* isSyncTry */ true);

  block->insertAtTail(t);

  // chpl_waitDynamicEndCount might throw, but we need to clean up the
  // end counts either way.

  BlockStmt* cleanup = new BlockStmt();

  cleanup->insertAtTail(new CallExpr("_endCountFree", new CallExpr(PRIM_GET_DYNAMIC_END_COUNT)));
  cleanup->insertAtTail(new CallExpr(PRIM_SET_DYNAMIC_END_COUNT, endCountSave));

  block->insertAtTail(new DeferStmt(cleanup));
  block->insertAtTail(new CallExpr(astr_chpl_waitDynamicEndCount));
  return block;
}


BlockStmt*
buildCobeginStmt(CallExpr* byref_vars, BlockStmt* block) {
  BlockStmt* outer = block;

  checkControlFlow(block, "cobegin statement");

  if (block->blockTag == BLOCK_SCOPELESS) {
    block = toBlockStmt(block->body.only());
    INT_ASSERT(block);
    block->remove();
  }

  if (block->length() < 2) {
    USR_WARN(outer, "cobegin has no effect if it contains fewer than 2 statements");
    // dropping byref_vars, if any
    return buildChapelStmt(block);
  }

  VarSymbol* cobeginCount = newTempConst("_cobeginCount");
  cobeginCount->addFlag(FLAG_END_COUNT);
  VarSymbol* numTasks = new_IntSymbol(block->length());

  for_alist(stmt, block->body) {
    BlockStmt* beginBlk = new BlockStmt();
    beginBlk->blockInfoSet(new CallExpr(PRIM_BLOCK_COBEGIN));
    beginBlk->astloc = stmt->astloc;
    // the original byref_vars is dead - will be clean_gvec-ed
    addByrefVars(beginBlk, copyByrefVars(byref_vars));
    stmt->insertBefore(beginBlk);
    beginBlk->insertAtHead(stmt->remove());
    beginBlk->insertAtTail(new CallExpr("_downEndCount", cobeginCount, gNil));
  }

  block->insertAtHead(new CallExpr("_upEndCount", cobeginCount, /*countRunningTasks=*/gTrue, numTasks));
  block->insertAtHead(new CallExpr(PRIM_MOVE, cobeginCount, new CallExpr("_endCountAlloc", /* forceLocalTypes= */gTrue)));
  block->insertAtHead(new DefExpr(cobeginCount));
  block->insertAtTail(new DeferStmt(new CallExpr("_endCountFree", cobeginCount)));
  block->insertAtTail(new CallExpr("_waitEndCount", cobeginCount, /*countRunningTasks=*/gTrue, numTasks));

  block->astloc = cobeginCount->astloc; // grab the location of 'cobegin' kw
  return block;
}


BlockStmt*
buildGotoStmt(GotoTag tag, const char* name) {
  return buildChapelStmt(new GotoStmt(tag, name));
}

BlockStmt* buildPrimitiveStmt(PrimitiveTag tag, Expr* e1, Expr* e2) {
  return buildChapelStmt(new CallExpr(tag, e1, e2));
}


BlockStmt* buildDeleteStmt(CallExpr* exprlist) {
  INT_ASSERT(exprlist->isPrimitive(PRIM_ACTUALS_LIST));
  return new BlockStmt(new CallExpr("chpl__delete", exprlist), BLOCK_SCOPELESS);
}

BlockStmt*
buildAtomicStmt(Expr* stmt) {
  static bool atomic_warning = false;

  if (!atomic_warning) {
    atomic_warning = true;
    USR_WARN(stmt, "atomic statement is ignored (not implemented)");
  }
  return buildChapelStmt(new BlockStmt(stmt));
}


CallExpr* buildPreDecIncWarning(Expr* expr, char sign) {
  if (sign == '+') {
    USR_WARN(expr, "++ is not a pre-increment");
    return new CallExpr("+", new CallExpr("+", expr));
  } else if (sign == '-') {
    USR_WARN(expr, "-- is not a pre-decrement");
    return new CallExpr("-", new CallExpr("-", expr));
  } else {
    INT_FATAL(expr, "Error in parser");
  }
  return NULL;
}

BlockStmt* convertTypesToExtern(BlockStmt* blk) {
  for_alist(node, blk->body) {
    if (DefExpr* de = toDefExpr(node)) {
      if (!de->init) {
        Symbol* vs = de->sym;
        PrimitiveType* pt = new PrimitiveType(NULL);

        TypeSymbol* ts = new TypeSymbol(vs->name, pt);
        if (VarSymbol* theVs = toVarSymbol(vs)) {
          ts->doc = theVs->doc;
        }
        DefExpr* newde = new DefExpr(ts);

        de->replace(newde);
        de = newde;
      }
      de->sym->addFlag(FLAG_EXTERN);
    } else {
      INT_FATAL("Got non-DefExpr in type_alias_decl_stmt");
    }
  }
  return blk;
}

BlockStmt* handleConfigTypes(BlockStmt* blk) {
  for_alist(node, blk->body) {
    if (DefExpr* defExpr = toDefExpr(node)) {
      if (VarSymbol* var = toVarSymbol(defExpr->sym)) {
        var->addFlag(FLAG_CONFIG);
        if (Expr *configInit = lookupConfigVal(var)) {
          // config var initialized on the command line
          // drop the original init expression on the floor
          if (Expr* a = toExpr(configInit))
            defExpr->init = a;
          else if (Symbol* a = toSymbol(configInit))
            defExpr->init = new SymExpr(a);
          else
            INT_FATAL(node, "Type alias initialized to invalid exprType");
        }
      }
    } else if (BlockStmt* innerBlk = toBlockStmt(node)) {
      // recursively handle multiple defs in a single statement
      handleConfigTypes(innerBlk);
    } else {
      INT_FATAL("Got non-DefExpr/BlockStmt in type_alias_decl_stmt");
    }
  }
  return blk;
}

// Attempt to find a stmt with a specific PrimitiveTag in a blockStmt
//
// For the case we're interested in we anticipate that the parser
// has constructed the following structure
//
//   BlockStmt
//     Scopeless BlockStmt
//       DefExpr  to define a tmp var
//       CallExpr to implement a move to the tmp var
//       BlockStmt with blockInfo
//
// So
//   1) We're trying to test the tail position of a blockStmt
//   2) We may need to step down one or more levels of blockStmt
//
// Finally BlockStmt is currently overloaded to represent a number of
// Stmts e.g. Loops, OnStmts, LocalStmts etc.
// We need to take care to discriminate among these

static BlockStmt* findStmtWithTag(PrimitiveTag tag, BlockStmt* blockStmt) {
  BlockStmt* retval = NULL;

  while (blockStmt != NULL && retval == NULL) {
    BlockStmt* tail = toBlockStmt(blockStmt->body.tail);

    // This is the stmt we're looking for
    if (tail != NULL && tail->isBlockType(tag)) {
      retval    = tail;

    // Stop if the current blockStmt is not of length 1
    } else if (blockStmt->length() != 1) {
      blockStmt = NULL;

    // Stop if the tail is not a "real" BlockStmt (e.g. a Loop etc)
    } else if (tail == NULL ||
               (tail->isRealBlockStmt() == false &&
                !tail->isBlockType(PRIM_BLOCK_ELIDED_ON))) {
      blockStmt = NULL;

    // Step in to the block and try again
    } else {
      blockStmt = tail;

    }
  }

  return retval;
}

Expr* tryExpr(Expr* e)
{
  return new CallExpr(PRIM_TRY_EXPR, e);
}

Expr* tryBangExpr(Expr* e)
{
  return new CallExpr(PRIM_TRYBANG_EXPR, e);
}

Expr* convertAssignmentAndWarn(Expr* a, const char* op, Expr* b)
{
  if (0 == strcmp("=", op)) {
    USR_FATAL_CONT(a, "Assignment is illegal in a conditional");
    USR_PRINT(a, "Use == to check for equality in a conditional");
  } else {
    USR_FATAL_CONT(a, "Assignment operation %s is illegal in a conditional",
                   op);
  }

  // Either way, continue compiling with ==
  return new CallExpr("==", a, b);
}

void redefiningReservedTypeError(const char* name)
{
  USR_FATAL_CONT(buildErrorStandin(),
                 "attempt to redefine reserved type '%s'", name);
}

void redefiningReservedWordError(const char* name)
{
  USR_FATAL_CONT(buildErrorStandin(),
                 "attempt to redefine reserved word '%s'", name);
}
